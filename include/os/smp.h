#ifndef SMP_H
#define SMP_H

#define NR_CPUS 2
extern void smp_init();
extern void wakeup_other_hart();
extern void lock_kernel();
extern void unlock_kernel();
extern int get_current_cpu_id();

#endif