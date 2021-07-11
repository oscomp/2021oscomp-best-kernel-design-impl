#ifndef __DEBUG_sysproc 
#undef DEBUG 
#endif 

#define __module_name__ 	"sysproc"

#include "include/types.h"
#include "include/riscv.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/spinlock.h"
#include "include/proc.h"
#include "include/syscall.h"
#include "include/timer.h"
#include "include/pm.h"
#include "include/vm.h"
#include "include/usrmm.h"
#include "include/string.h"
#include "include/printf.h"
#include "include/debug.h"

extern int execve(char *path, char **argv, char **envp);

uint64
sys_exec(void)
{
  char path[MAXPATH];
  uint64 argv;

  if(argstr(0, path, MAXPATH) < 0 || argaddr(1, &argv) < 0){
    return -1;
  }

  return execve(path, (char **)argv, 0);
}

uint64
sys_execve(void)
{
  char path[MAXPATH];
  uint64 argv, envp;

  if(argstr(0, path, MAXPATH) < 0 || argaddr(1, &argv) < 0 || argaddr(2, &envp)){
    return -1;
  }

  return execve(path, (char **)argv, (char **)envp);
}

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  // since exit never return, we print the trace-info here
  if (myproc()->tmask/* & (1 << (SYS_exit - 1))*/) {
    printf(")\n");
  }
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64 
sys_getppid(void) {
	struct proc *p = myproc();
	int pid;

	__debug_assert("sys_getppid", NULL != p->parent, 
			"NULL == p->parent\n");
	pid = p->parent->pid;

	return pid;
}

uint64
sys_fork(void)
{
  extern int fork_cow();
  return fork_cow();
  // return fork();
}

uint64 
sys_clone(void) {
	uint64 flag, stack;
	if (argaddr(0, &flag) < 0) 
		return -1;
	if (argaddr(1, &stack) < 0) 
		return -1;
	
	return clone(flag, stack);
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  // since wait suspends the proc, we print the left trace-info here
  // and when coming back, we re-print the leading trace-info for a clear view
  struct proc *pr = myproc();
  int mask = pr->tmask;// & (1 << (SYS_wait - 1));
  if (mask) {
    printf(") ...\n");
  }
  __debug_info("sys_wait", "p = %p\n", p);
  int ret = wait4(-1, p, 0);
  if (mask) {
    printf("pid %d: return from wait(0x%x", pr->pid, p);
  }
  return ret;
}

uint64 sys_wait4(void) {
	int pid;
	uint64 status, options;

	// extract syscall args 
	if (argint(0, &pid) < 0) 
		return -1;
	if (argaddr(1, &status) < 0) 
		return -1;
	if (argaddr(2, &options) < 0) 
		return -1;

	struct proc *pr = myproc();
	int mask = pr->tmask;
	int ret = wait4(pid, status, options);
	if (mask) {
		printf("pid %d: return from wait4(%p", pr->pid, status);
	}

	return ret;
}

// yield has no arg
uint64 sys_sched_yield(void) {
	yield();
	return 0;
}

uint64
sys_sbrk(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  
  struct proc *p = myproc();
  struct seg *heap = getseg(p->segment, HEAP);
  uint64 addr = heap->addr + heap->sz;
  if (n < 0) {
    if (growproc(n) < 0)  // growproc takes care of p->sz
      return -1;
  } else {                // lazy page allocation
    uint64 boundary = heap->next == NULL ? MAXUVA : heap->next->addr;
    if (addr + n > boundary - PGSIZE) {
      return -1;
    }
    heap->sz += n;
  }
  return addr;
}

uint64
sys_brk(void)
{
  uint64 addr;
  if(argaddr(0, &addr) < 0)
    return -1;
  
  struct proc *p = myproc();
  struct seg *heap = getseg(p->segment, HEAP);
  if (addr == 0) {
    return heap->addr + heap->sz;
  } else if (addr < heap->addr) {
    return -1;
  }
  int n = addr - (heap->addr + heap->sz);
  if (n < 0) {
    if (growproc(n) < 0)  // growproc takes care of p->sz
      return -1;
  } else {                // lazy page allocation
    uint64 boundary = heap->next == NULL ? MAXUVA : heap->next->addr;
    if (addr > boundary - PGSIZE) {
      return -1;
    }
    heap->sz += n;
  }
  return heap->addr + heap->sz;
  return 0;
}

uint64
sys_sleep(void)
{
  int n;
  int ret = 0;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  struct proc *p = myproc();
  int mask = p->tmask;// & (1 << (SYS_sleep - 1));
  if (mask) {
    printf(") ...\n");
  }
  acquire(&p->lk);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(p->killed){
      ret = -1;
      break;
    }
    sleep(&ticks, &p->lk);
  }
  release(&p->lk);

  if (mask) {
    printf("pid %d: return from sleep(%d", p->pid, n);
  }
  return ret;
}

uint64 sys_nanosleep(void) {
	uint64 addr_sec, addr_usec;

	if (argaddr(0, &addr_sec) < 0) 
		return -1;
	if (argaddr(1, &addr_usec) < 0) 
		return -1;

	struct proc *p = myproc();
	uint64 sec, usec;
	if (copyin2((char*)&sec, addr_sec, sizeof(sec)) < 0) 
		return -1;
	if (copyin2((char*)&usec, addr_usec, sizeof(usec)) < 0) 
		return -1;
	int n = sec * 20 + usec / 50000000;

	int mask = p->tmask;
	if (mask) {
		printf(") ...\n");
	}
	acquire(&p->lk);
	uint64 tick0 = ticks;
	while (ticks - tick0 < n) {
		if (p->killed) {
			return -1;
		}
		sleep(&ticks, &p->lk);
	}
	release(&p->lk);

	return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_trace(void)
{
  int mask;
  if(argint(0, &mask) < 0) {
    return -1;
  }
  myproc()->tmask = mask;
  return 0;
}

// uint64
// sys_getuid(void)
// {
//   return 0;
// }