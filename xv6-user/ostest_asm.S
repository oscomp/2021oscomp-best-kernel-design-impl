#include "kernel/include/sysnum.h"

.global exec
exec:
 li a7, SYS_exec
 ecall
 ret

.global fork
fork:
 li a7, SYS_fork
 ecall
 ret

.global wait
wait:
 li a7, SYS_wait
 ecall
 ret

.global dev
dev:
 li a7, SYS_dev
 ecall
 ret

.global dup
dup:
 li a7, SYS_dup
 ecall
 ret

.global exit
exit:
 li a7, SYS_exit
 ecall
 ret
