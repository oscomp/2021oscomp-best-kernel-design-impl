#include <stdio.h>
#include <type.h>
#include <qemu.h>

typedef int __attribute__((__mode__(SI))) int32_t;
int main()
{
    /* for loop */

    // while(1){
    //     for (int32_t i = 0; i < 1000; ++i)
    //     {
    //         printf("%d\n", i);
    //         sys_move_cursor(1,i % 20);
    //     }
    // }

    #ifdef K210
    /* for K210 do all test */
    int32_t result;
    while(result = sys_test(NULL)){
        // printf("result : %d\n", result);
        // sys_move_cursor(1,1);
        ;
    }
    // // printf("Success1\n");
    // while (1);

    #else

    /* for qemu singal test*/
    /* systest(filename) */

    sys_test("gettimeofday");
    // printf("Success1!\n\r");
    // sys_test("loop");    
    // printf("Success2!\n\r");

    #endif

    
    while (1);

}