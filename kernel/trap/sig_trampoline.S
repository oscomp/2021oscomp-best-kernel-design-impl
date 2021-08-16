# Trampoline for signal handling 

#include "sysnum.h"
	.section sigtrampsec

	.globl sig_trampoline
sig_trampoline: 

	.globl sig_handler
# void sig_handler(int signum, __sig_handler_t handler);
sig_handler: 
	jalr a1

	li a7, SYS_rt_sigreturn 
	ecall

# Terminate the proc directly, and this is the default handler 
# for any signal that's not set by user proc 
	.globl default_sigaction
# void term_handler(int signum);
default_sigaction: 
	# exit(-1);
	li a0, -1
	li a7, SYS_exit
	ecall