
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "hal/riscv.h"
#include "sbi.h"
#include "hal/plic.h"
#include "sched/proc.h"
#include "printf.h"

//
// the riscv Platform Level Interrupt Controller (PLIC).
//

// retrhelo: 
// It's wierd that the interrupt behaves differently on Maix Bit and 
// Maix M1. On M1 we detect Supervisor Interrupt, but on Bit we failed to. 
// Both platforms are said using K210 as CPU, but the implementations seem 
// to be different. No idea whether it's on purpose or not, due to the lack 
// of official documents support. 
// However, these two platforms seems to use a similiar/same RISC-V spec, 
// Illegal Instruction Exception occuring at `sfence.vma` exists on both. 

void plicinit(void) {
	writed(1, PLIC_V + DISK_IRQ * sizeof(uint32));
	writed(1, PLIC_V + UART_IRQ * sizeof(uint32));

	// #ifdef DEBUG 
	// printf("plicinit\n");
	// #endif 
}

void
plicinithart(void)
{
	int hart = cpuid();
	#ifdef QEMU
	// set uart's enable bit for this hart's S-mode. 
	*(uint32*)PLIC_SENABLE(hart)= (1 << UART_IRQ) | (1 << DISK_IRQ);
	// set this hart's S-mode priority threshold to 0.
	*(uint32*)PLIC_SPRIORITY(hart) = 0;
	#else
	uint32 *hart_m_enable = (uint32*)PLIC_MENABLE(hart);
	*(hart_m_enable) = readd(hart_m_enable) | (1 << DISK_IRQ);
	uint32 *hart0_m_int_enable_hi = hart_m_enable + 1;
	*(hart0_m_int_enable_hi) = readd(hart0_m_int_enable_hi) | (1 << (UART_IRQ % 32));

	// Sometimes on k210 the PLIC may not be initialized correctly, 
	// and may enable external interrupts on Supervisor as described above. 
	// Disable Supervisor External Interrupts on PLIC.
	uint32 *hart_s_enable = (uint32*)PLIC_SENABLE(hart);
	uint32 *hart_s_enable_hi = hart_s_enable + 1;
	*(hart_s_enable) = 0;
	*(hart_s_enable_hi) = 0;
	#endif
}

// ask the PLIC what interrupt we should serve.
int
plic_claim(void)
{
	int hart = cpuid();
	int irq;
	#ifndef QEMU
	irq = *(uint32*)PLIC_MCLAIM(hart);
	#else
	irq = *(uint32*)PLIC_SCLAIM(hart);
	#endif
	return irq;
}

// tell the PLIC we've served this IRQ.
void
plic_complete(int irq)
{
	int hart = cpuid();
	#ifndef QEMU
	*(uint32*)PLIC_MCLAIM(hart) = irq;
	#else
	*(uint32*)PLIC_SCLAIM(hart) = irq;
	#endif
}

