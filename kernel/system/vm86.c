#include <os/system.h>

/* success return 0, error return -1 */
int do_vm86(unsigned long fn, void *v86)
{
    debug();
    // log2(0, "fn is %d, v86 is %lx", fn, v86);
    return 0;
}