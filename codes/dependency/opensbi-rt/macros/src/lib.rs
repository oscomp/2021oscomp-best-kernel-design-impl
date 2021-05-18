//! Internal implementation details of `riscv-sbi-rt`.
//!
//! Do not use this crate directly.

#![deny(missing_docs)]

use proc_macro::TokenStream;
use proc_macro2::Span;
use quote::quote;
use rand::{Rng, SeedableRng};
use std::collections::HashSet;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::time::{SystemTime, UNIX_EPOCH};
use syn::{
    parse, spanned::Spanned, AttrStyle, Attribute, FnArg, Ident, Item, ItemFn, ItemStatic,
    ReturnType, Stmt, Type, Visibility,
};

mod codegen;
mod syntax;
use syntax::Mode;

/// Attribute to declare the entry point of the supervisor program
///
/// The specified function will be called by the runtime's init function,
/// after a heap allocator is created. See `init` function in `src/runtime.rs`
/// for details.
///
/// Users should provide specified function with signature `fn main(usize, usize)`.
/// For example, you may consider using `fn main(hartid: usize, dtb_pa: usize)`.
///
/// # Examples
///
/// - Simple entry point
///
/// ```ignore
/// #[entry]
/// fn main(hartid: usize, dtb_pa: usize) {
///     println!("Hello, RISC-V SBI!");
///     println!("hartid={}, dtb_pa={:#x}", hartid, dtb_pa);
/// }
/// ```
// Ref: https://github.com/rust-embedded/riscv-rt/blob/master/macros/src/lib.rs
#[proc_macro_attribute]
pub fn entry(args: TokenStream, input: TokenStream) -> TokenStream {
    let f: ItemFn = syn::parse(input).expect("`#[entry]` must be applied to a function");

    // check the function signature

    let valid_input_types = if f.sig.inputs.len() == 2 {
        let mut param_1_is_usize = false;
        if let FnArg::Typed(pat_type) = &f.sig.inputs[0] {
            if let Type::Path(type_path) = pat_type.ty.as_ref() {
                if type_path.path.segments.len() == 1
                    && type_path.path.segments[0].ident == "usize"
                    && type_path.path.segments[0].arguments.is_empty()
                {
                    param_1_is_usize = true;
                }
            }
        }
        let mut param_2_is_usize = false;
        if let FnArg::Typed(pat_type) = &f.sig.inputs[1] {
            if let Type::Path(type_path) = pat_type.ty.as_ref() {
                if type_path.path.segments.len() == 1
                    && type_path.path.segments[0].ident == "usize"
                    && type_path.path.segments[0].arguments.is_empty()
                {
                    param_2_is_usize = true;
                }
            }
        }
        param_1_is_usize && param_2_is_usize
    } else {
        false
    };

    let valid_signature = f.sig.constness.is_none()
        && f.vis == Visibility::Inherited
        && f.sig.abi.is_none()
        && valid_input_types
        && f.sig.generics.params.is_empty()
        && f.sig.generics.where_clause.is_none()
        && f.sig.variadic.is_none()
        && match f.sig.output {
            ReturnType::Default => true,
            ReturnType::Type(_, ref ty) => match ty.as_ref() {
                Type::Tuple(tuple) => tuple.elems.is_empty(),
                _ => false,
            },
        };

    if !valid_signature {
        return parse::Error::new(
            f.span(),
            "`#[entry]` function must have signature `fn main(hartid: usize, dtb_pa: usize)`",
        )
        .to_compile_error()
        .into();
    }

    if !args.is_empty() {
        return parse::Error::new(Span::call_site(), "This attribute accepts no arguments")
            .to_compile_error()
            .into();
    }

    let attrs = f.attrs;
    let generated_name = random_ident();
    let unsafety = f.sig.unsafety;
    let inputs = f.sig.inputs;
    let stmts = f.block.stmts;

    quote!(
        #[export_name = "main"]
        #(#attrs)*
        pub #unsafety fn #generated_name(#inputs) {
            #(#stmts)*
        }
    )
    .into()
}

/// Attribute to declare a supervisor-level interrupt trap handler
///
/// Handle RISC-V defined supervisor level interrupts.
/// Supported interrupts includes:
///
/// - UserSoft
/// - SupervisorSoft
/// - UserTimer
/// - SupervisorTimer
/// - UserExternal
/// - SupervisorExternal
///
/// If you want to handle other interrupts that's not implemented in the RISC-V standard,
/// you should provide a `DefaultHandler`, instead of using this attribute. (todo)
///
/// # Usage
///
/// By using a `#[interrupt] fn Name(...`, you override the default handler for interrupt
/// with the given `Name`. These handlers must have a signature like `[unsafe] fn() [-> !]`.
/// Additionally, it's possible to state these handlers by declaring `static mut` variables
/// at the beginning of the function body. These variables would be safe to access from the
/// function body.
///
/// If this interrupt handler is not overriden by custom `#[interrupt]` functions, the runtime
/// would trigger `DefaultHandler` instead.
///
/// # Example
///
/// - Handle a supervisor timer, print message every 100 * 100000 clocks.
/// ```ignore
/// static INTERVAL: u64 = 100000;
///
/// #[interrupt]
/// fn SupervisorTimer() {
///     static mut TICKS: usize = 0;
///
///     sbi::legacy::set_timer(time::read64().wrapping_add(INTERVAL));
///
///     *TICKS += 1;
///     if *TICKS % 100 == 0 {
///         println!("100 ticks~");
///     }
/// }
/// ```
// Ref: https://docs.rs/cortex-m-rt-macros/0.1.8/src/cortex_m_rt_macros/lib.rs.html
#[proc_macro_attribute]
pub fn interrupt(args: TokenStream, input: TokenStream) -> TokenStream {
    let mut f: ItemFn = syn::parse(input).expect("`#[interrupt]` must be applied to a function");

    if !args.is_empty() {
        return parse::Error::new(Span::call_site(), "This attribute accepts no arguments")
            .to_compile_error()
            .into();
    }

    let fspan = f.span();
    let ident = f.sig.ident.clone();
    let ident_s = ident.to_string();

    let valid_signature = f.sig.constness.is_none()
        && f.vis == Visibility::Inherited
        && f.sig.abi.is_none()
        && f.sig.inputs.is_empty()
        && f.sig.generics.params.is_empty()
        && f.sig.generics.where_clause.is_none()
        && f.sig.variadic.is_none()
        && match f.sig.output {
            ReturnType::Default => true,
            ReturnType::Type(_, ref ty) => match **ty {
                Type::Tuple(ref tuple) => tuple.elems.is_empty(),
                Type::Never(..) => true,
                _ => false,
            },
        };

    if !valid_signature {
        return parse::Error::new(
            fspan,
            "`#[interrupt]` handlers must have signature `[unsafe] fn() [-> !]`",
        )
        .to_compile_error()
        .into();
    }

    let (statics, stmts) = match extract_static_muts(f.block.stmts.iter().cloned()) {
        Err(e) => return e.to_compile_error().into(),
        Ok(x) => x,
    };

    f.sig.ident = Ident::new(
        &format!("__riscv_sbi_rt_{}", f.sig.ident),
        Span::call_site(),
    );
    f.sig.inputs.extend(statics.iter().map(|statik| {
        let ident = &statik.ident;
        let ty = &statik.ty;
        let attrs = &statik.attrs;
        syn::parse::<FnArg>(quote!(#[allow(non_snake_case)] #(#attrs)* #ident: &mut #ty).into())
            .unwrap()
    }));
    f.block.stmts = core::iter::once(
        syn::parse2(quote! {{
            extern crate riscv_sbi_rt;

            // Check that this interrupt actually exists
            // Ref: https://docs.rs/cortex-m-rt-macros/0.1.8/src/cortex_m_rt_macros/lib.rs.html
            riscv_sbi_rt::trap::interrupt::#ident;
        }})
        .unwrap(),
    )
    .chain(stmts)
    .collect();

    let tramp_ident = Ident::new(&format!("{}_trampoline", f.sig.ident), Span::call_site());
    let ident = &f.sig.ident;

    let resource_args = statics
        .iter()
        .map(|statik| {
            let (ref cfgs, ref attrs) = extract_cfgs(statik.attrs.clone());
            let ident = &statik.ident;
            let ty = &statik.ty;
            let expr = &statik.expr;
            quote! {
                #(#cfgs)*
                {
                    #(#attrs)*
                    static mut #ident: #ty = #expr;
                    &mut #ident
                }
            }
        })
        .collect::<Vec<_>>();

    quote!(
        #[doc(hidden)]
        #[export_name = #ident_s]
        pub unsafe extern "C" fn #tramp_ident() {
            #ident(
                #(#resource_args),*
            )
        }

        #f
    )
    .into()
}

static CALL_COUNT: AtomicUsize = AtomicUsize::new(0);

// Creates a random identifier
// Ref: https://github.com/rust-embedded/riscv-rt/blob/master/macros/src/lib.rs
fn random_ident() -> Ident {
    let secs = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .as_secs();

    let count: u64 = CALL_COUNT.fetch_add(1, Ordering::SeqCst) as u64;
    let mut seed: [u8; 16] = [0; 16];

    for (i, v) in seed.iter_mut().take(8).enumerate() {
        *v = ((secs >> (i * 8)) & 0xFF) as u8
    }

    for (i, v) in seed.iter_mut().skip(8).enumerate() {
        *v = ((count >> (i * 8)) & 0xFF) as u8
    }

    let mut rng = rand::rngs::SmallRng::from_seed(seed);
    Ident::new(
        &(0..16)
            .map(|i| {
                if i == 0 || rng.gen() {
                    (b'a' + rng.gen::<u8>() % 25) as char
                } else {
                    (b'0' + rng.gen::<u8>() % 10) as char
                }
            })
            .collect::<String>(),
        Span::call_site(),
    )
}

/// Attribute to mark which function will be called at the beginning of supervisor entry point.
///
/// The function must have the signature of `unsafe fn()`.
///
/// The `pre_init` function defined will be called _before_ the static variables are initialized.
/// Any access of static variables will result in undefined behavior.
///
/// # Examples
///
/// ```
/// # use riscv_sbi_rt_macros::pre_init;
/// #[pre_init]
/// unsafe fn before_main() {
///     // do something here
/// }
///
/// # fn main() {}
/// ```
// Ref: https://docs.rs/cortex-m-rt/0.6.12/cortex_m_rt/attr.pre_init.html
#[proc_macro_attribute]
pub fn pre_init(args: TokenStream, input: TokenStream) -> TokenStream {
    let f: ItemFn = syn::parse(input).expect("`#[pre_init]` must be applied to a function");

    // check the function signature
    let valid_signature = f.sig.constness.is_none()
        && f.vis == Visibility::Inherited
        && f.sig.unsafety.is_some()
        && f.sig.abi.is_none()
        && f.sig.inputs.is_empty()
        && f.sig.generics.params.is_empty()
        && f.sig.generics.where_clause.is_none()
        && f.sig.variadic.is_none()
        && match f.sig.output {
            ReturnType::Default => true,
            ReturnType::Type(_, ref ty) => match **ty {
                Type::Tuple(ref tuple) => tuple.elems.is_empty(),
                _ => false,
            },
        };

    if !valid_signature {
        return parse::Error::new(
            f.span(),
            "`#[pre_init]` function must have signature `unsafe fn()`",
        )
        .to_compile_error()
        .into();
    }

    if !args.is_empty() {
        return parse::Error::new(Span::call_site(), "This attribute accepts no arguments")
            .to_compile_error()
            .into();
    }

    // XXX should we blacklist other attributes?
    let attrs = f.attrs;
    let ident = f.sig.ident;
    let block = f.block;

    quote!(
        #[export_name = "__pre_init"]
        #(#attrs)*
        pub unsafe fn #ident() #block
    )
    .into()
}

// Extracts `static mut` vars from the beginning of the given statements
fn extract_static_muts(
    stmts: impl IntoIterator<Item = Stmt>,
) -> Result<(Vec<ItemStatic>, Vec<Stmt>), parse::Error> {
    let mut istmts = stmts.into_iter();

    let mut seen = HashSet::new();
    let mut statics = vec![];
    let mut stmts = vec![];
    while let Some(stmt) = istmts.next() {
        match stmt {
            Stmt::Item(Item::Static(var)) => {
                if var.mutability.is_some() {
                    if seen.contains(&var.ident) {
                        return Err(parse::Error::new(
                            var.ident.span(),
                            format!("the name `{}` is defined multiple times", var.ident),
                        ));
                    }

                    seen.insert(var.ident.clone());
                    statics.push(var);
                } else {
                    stmts.push(Stmt::Item(Item::Static(var)));
                }
            }
            _ => {
                stmts.push(stmt);
                break;
            }
        }
    }

    stmts.extend(istmts);

    Ok((statics, stmts))
}

fn extract_cfgs(attrs: Vec<Attribute>) -> (Vec<Attribute>, Vec<Attribute>) {
    let mut cfgs = vec![];
    let mut not_cfgs = vec![];

    for attr in attrs {
        if eq(&attr, "cfg") {
            cfgs.push(attr);
        } else {
            not_cfgs.push(attr);
        }
    }

    (cfgs, not_cfgs)
}

/// Returns `true` if `attr.path` matches `name`
fn eq(attr: &Attribute, name: &str) -> bool {
    attr.style == AttrStyle::Outer && attr.path.is_ident(name)
}

/// Init an Sv39 boot page before entering real start address.
///
/// ```no_run
/// #[cfg(target_pointer_width = "64")]
/// riscv_sbi_rt::boot_page_sv39! {
///     (0xffffffff_80000000 => 0x00000000_80000000, rwx);
///     (0xffffffff_00000000 => 0x00000000_00000000, rwx);
///     (0x00000000_80000000 => 0x00000000_80000000, rwx);
/// }
/// ```
#[proc_macro]
pub fn boot_page_sv39(item: TokenStream) -> TokenStream {
    let entry_config = match syntax::parse(item.into(), Mode::Sv39) {
        Err(e) => return e.to_compile_error().into(),
        Ok(x) => x,
    };

    let boot_page_content = codegen::boot_page_content(&entry_config, Mode::Sv39);

    quote!(
        #[repr(align(4096))]
        #[repr(C)]
        struct __BootPage([usize; 512]);
        #[export_name = "_boot_page"]
        static __BOOT_PAGE: __BootPage = __BootPage([ #boot_page_content ]);
        extern { fn _abs_start() -> !; }
        global_asm!("
    .section .init
    .globl _start
_start: 
    la t1, _boot_page
    srli t1, t1, 12
    li t0, 8 << 60
    or t0, t0, t1
    csrw satp, t0
    sfence.vma
    
    .option push
    .option norelax
1:
    auipc ra, %pcrel_hi(1f)
    ld ra, %pcrel_lo(1b)(ra)
    jr ra
    .align  3
1:
    .dword _abs_start
.option pop
        ");
    )
    .into()
}

/// Init an Sv48 boot page before entering real start address.
#[proc_macro]
pub fn boot_page_sv48(item: TokenStream) -> TokenStream {
    let entry_config = match syntax::parse(item.into(), Mode::Sv48) {
        Err(e) => return e.to_compile_error().into(),
        Ok(x) => x,
    };

    let boot_page_content = codegen::boot_page_content(&entry_config, Mode::Sv48);

    quote!(
        #[repr(align(4096))]
        #[repr(C)]
        struct __BootPage([usize; 512]);
        #[export_name = "_boot_page"]
        static __BOOT_PAGE: __BootPage = __BootPage([ #boot_page_content ]);
        extern { fn _abs_start() -> !; }
        global_asm!("
    .section .init
    .globl _start
_start: 
    la t1, _boot_page
    srli t1, t1, 12
    li t0, 9 << 60
    or t0, t0, t1
    csrw satp, t0
    sfence.vma
    
    .option push
    .option norelax
1:
    auipc ra, %pcrel_hi(1f)
    ld ra, %pcrel_lo(1b)(ra)
    jr ra
    .align  3
1:
    .dword _abs_start
.option pop
        ");
    )
    .into()
}

// There should be sv57, sv64 here in the future

/// Init an Sv32 boot page before entering real start address.
///
/// ```no_run
/// #[cfg(target_pointer_width = "32")]
/// riscv_sbi_rt::boot_page_sv32! {
///     // On Sv32, physical address can be up to 34 bits
///     (0x80400000 => 0x3fffff000, rwx);
///     (0x00400000 => 0x00000000, rwx);
/// }
/// ```
#[proc_macro]
pub fn boot_page_sv32(item: TokenStream) -> TokenStream {
    let entry_config = match syntax::parse(item.into(), Mode::Sv32) {
        Err(e) => return e.to_compile_error().into(),
        Ok(x) => x,
    };

    let boot_page_content = codegen::boot_page_content(&entry_config, Mode::Sv32);

    quote!(
        #[repr(align(4096))]
        #[repr(C)]
        struct __BootPage([usize; 1024]);
        #[export_name = "_boot_page"]
        static __BOOT_PAGE: __BootPage = __BootPage([ #boot_page_content ]);
        extern { fn _abs_start() -> !; }
        global_asm!("
    .section .init
    .globl _start
_start: 
    la t1, _boot_page
    srli t1, t1, 12
    li t0, 1 << 31
    or t0, t0, t1
    csrw satp, t0
    sfence.vma
    
    .option push
    .option norelax
1:
    auipc ra, %pcrel_hi(1f)
    lw ra, %pcrel_lo(1b)(ra)
    jr ra
    .align  2
1:
    .word _abs_start
.option pop
        ");
    )
    .into()
}

// if you need boot_page_bare, you don't include any macro in this crate.
