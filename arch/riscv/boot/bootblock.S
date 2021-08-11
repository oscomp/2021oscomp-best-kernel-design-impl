#include <asm/sbidef.h>
#include <asm/sbiasm.h>

// 1. kernel address (jmp here to start kernel main!)
.equ boot_kernel, 0x80020000

.text
.global main

main:
	// Load the global pointer
	.option push
  	.option norelax
  	la gp, __global_pointer$
  	.option pop 

	// fence on all memory and I/O
	fence

	// print a \n to avoid system output error...
	li a0, 10
	SBI_CALL SBI_CONSOLE_PUTCHAR

	// call BIOS print string "It's bootblock!"
	la a0, msg
	SBI_CALL SBI_CONSOLE_PUTSTR

	csrr a0, CSR_MHARTID
	j boot_kernel

// while(1) --> stop here
stop:
	j stop
	nop

.data

msg: .string "It's Boot Loader...\n\r"

