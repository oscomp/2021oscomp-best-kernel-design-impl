#include <asm.h>

ENTRY(_start)
    /* TODO: */
    /* Load the global pointer */
  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop

  /* for convinience */
  j clear_bss_done

    /* clear bss */
  la t3, __bss_start
  la t4, __BSS_END__
  ble t4, t3, clear_bss_done
clear_bss:
  sb zero, (t3)
  add t3, t3, 1
  blt t3, t4, clear_bss
clear_bss_done:
    /* call main */
  call main
    /* call sys_exit*/
  call sys_exit

loop:
  j loop
END(_start)
