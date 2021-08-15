// Handling Traps 

#ifndef __DEBUG_trap
#undef  DEBUG
#endif

#define __module_name__ 	"trap"

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "hal/riscv.h"
#include "sync/spinlock.h"
#include "sched/proc.h"
#include "sbi.h"
#include "hal/plic.h"
#include "trap.h"
#include "syscall.h"
#include "printf.h"
#include "console.h"
#include "timer.h"
#include "hal/disk.h"
#include "mm/vm.h"
#include "utils/debug.h"

// Interrupt flag: set 1 in the Xlen - 1 bit
#define INTERRUPT_FLAG    0x8000000000000000L

// Supervisor interrupt number
#define INTR_SOFTWARE    (0x1 | INTERRUPT_FLAG)
#define INTR_TIMER       (0x5 | INTERRUPT_FLAG)
#define INTR_EXTERNAL    (0x9 | INTERRUPT_FLAG)

// Supervisor exception number
#define EXCP_INST_ACCESS  0x1
#define EXCP_LOAD_ACCESS  0x5
#define EXCP_STORE_ACCESS 0x7
#define EXCP_ENV_CALL     0x8
#define EXCP_INST_PAGE    0xc // 12
#define EXCP_LOAD_PAGE    0xd // 13
#define EXCP_STORE_PAGE   0xf // 15

#ifndef QEMU 
	#define is_page_fault(scause) \
		(EXCP_LOAD_ACCESS == (scause) || EXCP_STORE_ACCESS == (scause)) 
#else 
	#define is_page_fault(scause) \
		(EXCP_LOAD_PAGE == (scause) || EXCP_STORE_PAGE == (scause)) 
#endif 

extern char trampoline[], uservec[], userret[];
extern void kernelvec(); // in kernelvec.S, calls kerneltrap().

int handle_intr(uint64 scause);
int handle_excp(uint64 scause);

// set up to take exceptions and traps while in the kernel.
void
trapinithart(void)
{
	w_stvec((uint64)kernelvec);
	w_sstatus(r_sstatus() | SSTATUS_SIE);
	// enable supervisor-mode timer interrupts.
	w_sie(r_sie() | SIE_SEIE | SIE_SSIE | SIE_STIE);
	set_next_timeout();

	__debug_info("trapinithart", "init\n");
}

//
// handle an interrupt, exception, or system call from user space.
// called from trampoline.S
//
void
usertrap(void)
{
	__debug_assert("usertrap", 0 == (r_sstatus() & SSTATUS_SPP), 
			"not from user mode\n");

	// send interrupts and exceptions to kerneltrap(),
	// since we're now in the kernel.
	w_stvec((uint64)kernelvec);

	protect_usr_mem();  // since we turned on this when leaving S-mode

	struct proc *p = myproc();

	uint64 timestamp = readtime();
	p->proc_tms.ikstmp = timestamp;
	p->proc_tms.utime += timestamp - p->proc_tms.okstmp;

	// save user program counter.
	p->trapframe->epc = r_sepc();

	uint64 cause = r_scause();
	__debug_info("usertrap", "cause %d\n", cause);
	if (cause == EXCP_ENV_CALL) {
		// system call
		if(p->killed)
			exit(-1);
		// sepc points to the ecall instruction,
		// but we want to return to the next instruction.
		p->trapframe->epc += 4;
		// an interrupt will change sstatus &c registers,
		// so don't enable until done with those registers.
		intr_on();
		syscall();
	} 
	else if (0 == handle_intr(cause)) {
		// handle interrupt 
		if (yield()) {
			p->ivswtch += 1;
		}
	}
	else if (0 == handle_excp(cause)) {
		// handle exception 
	}
	else {
		printf("\nusertrap(): unexpected scause %p pid=%d %s\n", cause, p->pid, p->name);
		printf("            sepc=%p stval=%p\n", r_sepc(), r_stval());
		trapframedump(p->trapframe);
		p->killed = 1;
	}
   
	if(p->killed)
		exit(-1);

	__debug_info("usertrap", "%d: enter usertrapret\n", cause);
	usertrapret();
} 
   
// 
// return to user space
//
void
usertrapret(void) {
	struct proc *p = myproc();

	// we're about to switch the destination of traps from
	// kerneltrap() to usertrap(), so turn off interrupts until
	// we're back in user space, where usertrap() is correct.
	intr_off();

	uint64 timestamp = readtime();
	p->proc_tms.okstmp = timestamp;
	p->proc_tms.stime += timestamp - p->proc_tms.ikstmp;

	// send syscalls, interrupts, and exceptions to trampoline.S
	w_stvec(TRAMPOLINE + (uservec - trampoline));

	// set up trapframe values that uservec will need when
	// the process next re-enters the kernel.
	p->trapframe->kernel_satp = r_satp();         // kernel page table
	p->trapframe->kernel_sp = p->kstack + PGSIZE;	// process's kernel stack 
	p->trapframe->kernel_trap = (uint64)usertrap;
	p->trapframe->kernel_hartid = r_tp();         // hartid for cpuid()

	// set up the registers that trampoline.S's sret will use
	// to get to user space.
	 
	// set S Previous Privilege mode to User.
	unsigned long x = r_sstatus();
	x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
	x |= SSTATUS_SPIE; // enable interrupts in user mode
	w_sstatus(x);
	 
	// set S Exception Program Counter to the saved user pc.
	w_sepc(p->trapframe->epc);
	 
	// tell trampoline.S the user page table to switch to.
	// printf("[usertrapret]p->pagetable: %p\n", p->pagetable);
	uint64 satp = MAKE_SATP(p->pagetable);
	 
	permit_usr_mem(); // it's odd that without this the hart will stuck in u-mode on k210
	 
	// jump to trampoline.S at the top of memory, which 
	// switches to the user page table, restores user registers,
	// and switches to user mode with sret.
	uint64 fn = TRAMPOLINE + (userret - trampoline);
	// ((void (*)(uint64,uint64))fn)(TRAPFRAME, satp);
	((void (*)(uint64, uint64))fn)((uint64)(p->trapframe), satp);
}

// interrupts and exceptions from kernel code go here via kernelvec,
// on whatever the current kernel stack is.
void 
kerneltrap() {
	uint64 sepc = r_sepc();
	uint64 sstatus = r_sstatus();
	uint64 scause = r_scause();

	struct proc *p = myproc();

	// the kernel might be accessing user space when this trap was happening
	// the pre-state is stored in the var 'sstatus' as above, which will be restored before leaving
	protect_usr_mem();

	__debug_assert("kerneltrap", (0 != (sstatus & SSTATUS_SPP)), 
			"not from supervisor mode\n");
	__debug_assert("kerneltrap", 0 == intr_get(), 
			"interrupts enable\n");

	//__debug_info("kerneltrap", "enter %d\n", scause);
	if (0 == handle_intr(scause)) {
		// handle interrupt
	}
	else if (0 == handle_excp(scause)) {
		// handle exception 
	}
	else if (p && is_page_fault(scause) && PGSIZE <= r_stval() && r_stval() < MAXUVA) {
		// This case may happen when kernel is accessing user's lazy-allocated page.
		// The handler should allocate a real one, but failed for lack of memory.
		//p->killed = 1;
		sepc = kern_pgfault_escape(r_stval());
		__debug_warn("kerneltrap", "sepc=%p stval=%p escape=%p pid=%d\n", 
				r_sepc(), r_stval(), sepc, p->pid);
	}
	else {
		// unsupported trap 
		printf("\nscause %p\n", scause);
		printf("sepc=%p stval=%p hart=%d\n", r_sepc(), r_stval(), r_tp());
		if (p != NULL) {
			printf("pid: %d, name: %s\n", p->pid, p->name);
		}
		panic("kerneltrap");
	}
	// the yield() may have caused some traps to occur,
	// so restore trap registers for use by kernelvec.S's sepc instruction.
	w_sepc(sepc);
	w_sstatus(sstatus);
}

// check if it's an interrupt, and handle it 
// returns 
int handle_intr(uint64 scause) {
	#ifdef DEBUG 
	static int cnt = 0;
	#endif 

	if (INTR_TIMER == scause) {
		#ifdef DEBUG 
		if (100 == cnt) {
			__debug_info("handle_intr", "tick\n");
			cnt = 0;
		}
		else {
			cnt ++;
		}
		#endif 
		timer_tick();
		proc_tick();
		return 0;
	}
	#ifdef QEMU 
	else if (INTR_EXTERNAL == scause) 
	#else 
	else if (INTR_SOFTWARE == scause && sbi_xv6_is_ext().value) 
	#endif 
	{
		int irq = plic_claim();
		int c;
		switch (irq) {
		case UART_IRQ: 
			// keyboard input 
			c = sbi_console_getchar();
			if (-1 != c) 
				consoleintr(c);
			break;
		case DISK_IRQ: 
			// disk read/write complete 
			__debug_info("handle_intr", "disk\n");
			disk_intr();
			break;
		default: 
			__debug_warn("handle_intr", "unexpected interrupt irq = %d\n", irq);
			break;
		}

		if (irq) plic_complete(irq);

		#ifndef QEMU 
		w_sip(r_sip() & ~2);	// clear pending bit 
		{
			struct sbiret res = sbi_xv6_set_ext();
			if (SBI_SUCCESS != res.error) {
				panic("sbi_xv6_set_ext() failed\n");
			}
		}
		#endif 

		// send software interrupts to other harts to inform them 
		// for (int i = 0; i < NCPU; i ++) {
		// 	if (cpuid() != i) {
		// 		sbi_send_ipi(1 << i, 0);
		// 	}
		// }

		return 0;
	}
	else if (INTR_SOFTWARE == scause) {		// the real software interrupt
		sbi_clear_ipi();

		return 0;
	}
	else {
		// to be honest, on k210 software interrupts may be used for IPI, 
		// but as it is not yet supported, handle this as an unsupported one 
		return -1;
	}
}

// be noticed that syscall is not handled here 
int handle_excp(uint64 scause) {
	// later implementation may handle more cases, such as lazy allocation, mmap, etc.
	switch (scause) {
	case EXCP_STORE_PAGE: 
	#ifndef QEMU 
	case EXCP_STORE_ACCESS: 
	#endif 
		return handle_page_fault(1, r_stval());
	case EXCP_LOAD_PAGE: 
	#ifndef QEMU 
	case EXCP_LOAD_ACCESS: 
	#endif 
		return handle_page_fault(0, r_stval());
	case EXCP_INST_PAGE:
	#ifndef QEMU
	case EXCP_INST_ACCESS:
	#endif
		return handle_page_fault(2, r_stval());
	// unsupported exception 
	default: return -1;
	}
}

void trapframedump(struct trapframe *tf)
{
  printf("a0: %p\t", tf->a0);
  printf("a1: %p\t", tf->a1);
  printf("a2: %p\t", tf->a2);
  printf("a3: %p\n", tf->a3);
  printf("a4: %p\t", tf->a4);
  printf("a5: %p\t", tf->a5);
  printf("a6: %p\t", tf->a6);
  printf("a7: %p\n", tf->a7);
  printf("t0: %p\t", tf->t0);
  printf("t1: %p\t", tf->t1);
  printf("t2: %p\t", tf->t2);
  printf("t3: %p\n", tf->t3);
  printf("t4: %p\t", tf->t4);
  printf("t5: %p\t", tf->t5);
  printf("t6: %p\t", tf->t6);
  printf("s0: %p\n", tf->s0);
  printf("s1: %p\t", tf->s1);
  printf("s2: %p\t", tf->s2);
  printf("s3: %p\t", tf->s3);
  printf("s4: %p\n", tf->s4);
  printf("s5: %p\t", tf->s5);
  printf("s6: %p\t", tf->s6);
  printf("s7: %p\t", tf->s7);
  printf("s8: %p\n", tf->s8);
  printf("s9: %p\t", tf->s9);
  printf("s10: %p\t", tf->s10);
  printf("s11: %p\t", tf->s11);
  printf("ra: %p\n", tf->ra);
  printf("sp: %p\t", tf->sp);
  printf("gp: %p\t", tf->gp);
  printf("tp: %p\t", tf->tp);
  printf("epc: %p\n", tf->epc);
}
