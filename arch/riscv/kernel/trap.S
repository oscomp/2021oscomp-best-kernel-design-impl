#include <asm.h>
#include <csr.h>

ENTRY(setup_exception)
  /* TODO: 
   * save exception_handler_entry into STVEC
   * enable global exceptions */

   la t0, exception_handler_entry
   addi t0, t0, 3
   li t1, 0xfffffffffffffffc
   and t0, t0, t1

   csrw CSR_STVEC, t0
   
   jr ra
ENDPROC(setup_exception)

ENTRY(clear_interrupt)
  csrw CSR_SIP, zero
  jr ra
ENDPROC(clear_interrupt)
