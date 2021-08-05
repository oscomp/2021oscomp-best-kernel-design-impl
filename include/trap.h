#ifndef __TRAP_H
#define __TRAP_H

#include "types.h"

// per-process data for the trap handling code in trampoline.S.
// sits in a page by itself just under the trampoline page in the
// user page table. not specially mapped in the kernel page table.
// the sscratch register points here.
// uservec in trampoline.S saves user registers in the trapframe,
// then initializes registers from the trapframe's
// kernel_sp, kernel_hartid, kernel_satp, and jumps to kernel_trap.
// usertrapret() and userret in trampoline.S set up
// the trapframe's kernel_*, restore user registers from the
// trapframe, switch to the user page table, and enter user space.
// the trapframe includes callee-saved user registers like s0-s11 because the
// return-to-user path via usertrapret() doesn't return through
// the entire kernel call stack.
struct trapframe {
	/*   0 */ uint64 kernel_satp;   // kernel page table
	/*   8 */ uint64 kernel_sp;     // top of process's kernel stack
	/*  16 */ uint64 kernel_trap;   // usertrap()
	/*  24 */ uint64 epc;           // saved user program counter
	/*  32 */ uint64 kernel_hartid; // saved kernel tp
	/*  40 */ uint64 ra;
	/*  48 */ uint64 sp;
	/*  56 */ uint64 gp;
	/*  64 */ uint64 tp;
	/*  72 */ uint64 t0;
	/*  80 */ uint64 t1;
	/*  88 */ uint64 t2;
	/*  96 */ uint64 s0;
	/* 104 */ uint64 s1;
	/* 112 */ uint64 a0;
	/* 120 */ uint64 a1;
	/* 128 */ uint64 a2;
	/* 136 */ uint64 a3;
	/* 144 */ uint64 a4;
	/* 152 */ uint64 a5;
	/* 160 */ uint64 a6;
	/* 168 */ uint64 a7;
	/* 176 */ uint64 s2;
	/* 184 */ uint64 s3;
	/* 192 */ uint64 s4;
	/* 200 */ uint64 s5;
	/* 208 */ uint64 s6;
	/* 216 */ uint64 s7;
	/* 224 */ uint64 s8;
	/* 232 */ uint64 s9;
	/* 240 */ uint64 s10;
	/* 248 */ uint64 s11;
	/* 256 */ uint64 t3;
	/* 264 */ uint64 t4;
	/* 272 */ uint64 t5;
	/* 280 */ uint64 t6;

	/* 288 */ uint64 ft0;
	/* 296 */ uint64 ft1;
	/* 304 */ uint64 ft2;
	/* 312 */ uint64 ft3;
	/* 320 */ uint64 ft4;
	/* 328 */ uint64 ft5;
	/* 336 */ uint64 ft6;
	/* 344 */ uint64 ft7;
	/* 352 */ uint64 fs0;
	/* 360 */ uint64 fs1;
	/* 368 */ uint64 fa0;
	/* 376 */ uint64 fa1;
	/* 384 */ uint64 fa2;
	/* 392 */ uint64 fa3;
	/* 400 */ uint64 fa4;
	/* 408 */ uint64 fa5;
	/* 416 */ uint64 fa6;
	/* 424 */ uint64 fa7;
	/* 432 */ uint64 fs2;
	/* 440 */ uint64 fs3;
	/* 448 */ uint64 fs4;
	/* 456 */ uint64 fs5;
	/* 464 */ uint64 fs6;
	/* 472 */ uint64 fs7;
	/* 480 */ uint64 fs8;
	/* 488 */ uint64 fs9;
	/* 496 */ uint64 fs10;
	/* 504 */ uint64 fs11;
	/* 512 */ uint64 ft8;
	/* 520 */ uint64 ft9;
	/* 528 */ uint64 ft10;
	/* 536 */ uint64 ft11;

	/* 544 */ uint64 fcsr;
};

void            trapinithart(void);
void            usertrapret(void);
void            trapframedump(struct trapframe *tf);
void			floatstore(struct trapframe *tf);
void			floatload(struct trapframe *tf);

#endif
