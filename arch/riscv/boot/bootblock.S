#include <asm/sbidef.h>
#include <asm/sbiasm.h>

// #define BOOT_LOADER_SIG_OFFSET 0x1fe
// #define OS_SIZE_LOC (BOOT_LOADER_SIG_OFFSET - 2)
// #define OS_SIZE_LOC 0x1fc

.equ os_size_loc, 0x502001fc

// 1. kernel address (move kernel to here ~)
.equ kernel, 0x50200000

// 2. kernel main address (jmp here to start kernel main!)
.equ kernel_main, 0x50200000
.equ sector_size, 0x200

//TO BE CHANGED
.equ bootloader_copy, 0x50300000
.equ bootloader_offset, 0x100000
.equ os_size_loc_copy, 0x503001fc

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

	fence
	// copy bootloader
	li a0, bootloader_copy
	li a1, 0x1
	li a2, 0x0
	SBI_CALL SBI_SD_READ
	fence.i

	// jump to 'skip' in the copy of bootloader
	la t0, skip
	li t1, bootloader_offset
	add t0,t0, t1
	jr t0


skip:	
	
	// load kernel
	li a1, os_size_loc_copy
	lh a1, (a1)

	li a0, kernel
	li a2, 0x1
	SBI_CALL SBI_SD_READ
	fence.i

	// jump to kernel to start 
	li a0, kernel_main
	jr a0

// while(1) --> stop here
stop:
	j stop
	nop

.data

msg: .string "It's Boot Loader...\n\r"

