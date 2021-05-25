#include <asm.h>
#include <csr.h>

ENTRY(get_current_cpu_id)
  csrr a0, CSR_MHARTID
  jr ra
ENDPROC(get_current_cpu_id)

ENTRY(read_satp)
    csrr a0, CSR_SATP
    ret
ENDPROC(read_satp)

ENTRY(read_sip)
  csrr a0, CSR_SIP
  jr ra
ENDPROC(read_sip)

ENTRY(read_sie)
  csrr a0, CSR_SIE
  jr ra
ENDPROC(read_sie)

ENTRY(direct_set_satp)
  csrw CSR_SATP, a0
  ret
ENDPROC(direct_set_satp)

ENTRY(read_gp)
  mv a0, gp
  ret
ENDPROC(read_gp)

ENTRY(get_a2)
  mv a0, a2
  ret
ENDPROC(get_a2)