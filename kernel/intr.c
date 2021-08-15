#include "types.h"
#include "param.h"
#include "sched/proc.h"
#include "intr.h"
#include "printf.h"

// push_off/pop_off are like intr_off()/intr_on() except that they are matched:
// it takes two pop_off()s to undo two push_off()s.  Also, if interrupts
// are initially off, then push_off, pop_off leaves them off.

void
push_off(void)
{
	int old = intr_get();

	intr_off();
	struct cpu *c = mycpu();
	//printf("\e[32mpush_off()\e[0m: cpuid(): %d\n", cpuid());
	if (c->noff == 0)
		c->intena = old;
	c->noff += 1;
}

void
pop_off(void)
{
	struct cpu *c = mycpu();

	//printf("\e[31mpop_off()\e[0m: cpuid(): %d\n", cpuid());
	if(intr_get())
		panic("pop_off - interruptible");
	if(c->noff < 1) {
		//printf("c->noff = %d\n", c->noff);
		panic("pop_off");
	}
	//printf("c->noff: %d\n", c->noff);
	//printf("c: %x\n", c);
	c->noff -= 1;
	if(c->noff == 0 && c->intena)
		intr_on();
}
