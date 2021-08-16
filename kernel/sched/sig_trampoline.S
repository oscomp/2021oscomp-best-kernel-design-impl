# Trampoline for signal handlers
# All codes below are running in User Mode 
# We use place it with general Trampoline 

#include "sysnum.h"

	.section trampsec
	.globl sig_handler
# void sig_handler(int signum, __sig_handler_t handler);
sig_handler: 
	jalr a1

	li a7, SYS_sigreturn 
	ecall

# Terminate the proc directly, and this is the default handler 
# for any signal that's not set by user proc 
	.section tramsec
	.globl term_handler
# void term_handler(int signum);
term_handler: 
	# exit(-1);
	li a0, -1
	li a7, SYS_exit
	ecall