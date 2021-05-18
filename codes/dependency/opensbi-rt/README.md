# riscv-sbi-rt

[![Actions Status](https://github.com/rcore-os/riscv-sbi-rt/workflows/CI/badge.svg)](https://github.com/rcore-os/riscv-sbi-rt/actions)

A mininal runtime / startup for Supervisor Binary Interface (SBI) on RISC-V.

## Features

- [x] Minimal S-level SBI entry runtime & pre-init
- [x] Handling traps (interrupts and exceptions)
- [x] Friendly macros and compile time checks
- [x] Preparation for frame and page system
- [x] Support for switching between contexts
- [x] Prepare for user mode, support for system calls
- [x] Proper support for systems without paging system
- [x] Nice document and design pattern

todo:

- [ ] Optional legacy privilege spec v1.9.1 (for K210) support

## Example

Write four lines to build a minimum operating system kernel:

```rust
// src/main.rs

#[riscv_sbi_rt::entry]
fn main(hartid: usize, dtb_pa: usize) {
    riscv_sbi::println!("Hello, OpenSBI!");
}
```

Another four lines for a trap handler:

```rust
#[interrupt]
fn SupervisorSoft() {
    println!("SupervisorSoft!");
}
```

Add serveral additional lines to build with an Sv39 initial boot page:

```rust
#[cfg(target_pointer_width = "64")]
riscv_sbi_rt::boot_page_sv39! {
    (0xffffffff_80000000 => 0x00000000_80000000, rwx);
    (0xffffffff_00000000 => 0x00000000_00000000, rwx);
    (0x00000000_80000000 => 0x00000000_80000000, rwx);
}
```

Customize memory areas in your linker script:

```rust
MEMORY {
    /* Virtual address mapped memory area */
    DRAM : ORIGIN = 0xffffffff80000000, LENGTH = 128M
}

/* Use virtual address is okay if you have an initial boot page */
PROVIDE(_stext = 0xffffffff80200000);
/* Modify this to provide bigger stack for each hart */
PROVIDE(_hart_stack_size = 128K);
/* Modify this to set max hart number */
PROVIDE(_max_hart_id = 1);
/* Modify this to add frame section size */
PROVIDE(_frame_size = 16384 * 4K);

/* Map the runtime regions into memory areas */
REGION_ALIAS("REGION_TEXT", DRAM);
REGION_ALIAS("REGION_RODATA", DRAM);
REGION_ALIAS("REGION_DATA", DRAM);
REGION_ALIAS("REGION_BSS", DRAM);
REGION_ALIAS("REGION_STACK", DRAM);
REGION_ALIAS("REGION_FRAME", DRAM);
```

The addresses of memory areas are defined together with boot page macro.
For example if there's a `DRAM` with the physical base address `0x80000000`,
and there is a boot page mapper for `0xffffffff_80000000 => 0x00000000_80000000`,
then the virtual base address of `DRAM` is `0xffffffff80000000`.
