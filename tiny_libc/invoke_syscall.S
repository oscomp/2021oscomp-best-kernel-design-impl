#include <asm.h>

ENTRY(invoke_syscall)
  /* TODO: */
  mv a7, a0
  mv a0, a1
  mv a1, a2
  mv a2, a3
  ecall
  ret
ENDPROC(invoke_syscall)
