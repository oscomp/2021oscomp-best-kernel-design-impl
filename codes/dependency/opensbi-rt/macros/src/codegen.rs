use crate::syntax::{EntryConfig, Mode};
use proc_macro2::Literal;
use proc_macro2::TokenStream;
use quote::quote;

pub fn boot_page_content(entry_config: &EntryConfig, mode: Mode) -> TokenStream {
    match mode {
        // in Sv32, virtual page number contain 10 bits
        Mode::Sv32 => {
            let pte = (0..1024)
                .map(|idx| entry_config[idx])
                .map(Literal::usize_unsuffixed);
            quote!( #( #pte , )* )
        }
        // in Sv39 and Sv48, virtual page number contain 9 bits
        Mode::Sv39 | Mode::Sv48 => {
            let pte = (0..512)
                .map(|idx| entry_config[idx])
                .map(Literal::usize_unsuffixed);
            quote!( #( #pte , )* )
        }
    }
}

/*

    Generated like:

    0, 0, 0, /* and all 512 (1024) usize page table entries */, 0,

*/
