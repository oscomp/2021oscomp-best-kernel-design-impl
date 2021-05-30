#include <stdio.h>
#include <type.h>
#include <qemu.h>

typedef int __attribute__((__mode__(SI))) int32_t;
int main()
{

    #ifdef K210
    /* for K210 do all test */
    int32_t result;
    while(result = sys_test(NULL)){
        ;
    }

    #else

    /* for qemu singal test*/

    sys_test("uname");

    #endif

    while (1);

}