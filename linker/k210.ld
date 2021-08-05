OUTPUT_ARCH(riscv)
ENTRY(_entry)

BASE_ADDRESS = 0x80020000;

SECTIONS
{
    /* Load the kernel at this address: "." means the current address */
    . = BASE_ADDRESS;

    kernel_start = .;

    . = ALIGN(4K);

    text_start = .;
    .text : {
        *(.text .text.*)
        . = ALIGN(0x1000);
        /* _trampoline = .; */
        *(trampsec)
        . = ALIGN(0x1000);
        /* ASSERT(. - _trampoline == 0x1000, "error: trampoline larger than one page"); */
        PROVIDE(etext = .);
    }

    . = ALIGN(4K);
    rodata_start = .;
    .rodata : {
        srodata = .;
        *(.rodata .rodata.*)
        erodata = .;
    }

    . = ALIGN(4K);
    data_start = .;
    .data : {
        sdata = .;
        *(.data .data.*)
        edata = .;
    }

    . = ALIGN(4K);
    bss_start = .;
    .bss : {
	    *(.bss.stack)
        sbss_clear = .;
        *(.sbss .bss .bss.*)
        ebss_clear = .;
    }
    
    .init_array : ALIGN(8) {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
        PROVIDE_HIDDEN ( metal_constructors_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.metal.init_array.*)));
        KEEP (*(.metal.init_array));
        PROVIDE_HIDDEN ( metal_constructors_end = .);
    }
    
    . = ALIGN(4K);
    PROVIDE(end = .);
    . = ALIGN(4K);
    PROVIDE(end = .);
}