#include <stdio.h>
#include <type.h>

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

    /* for K210 do all test */

    // while(sys_test(NULL))
    //     ;

    /* for qemu singal test*/
    /* systest(filename) */
    sys_test("loop");
    printf("Success1!\n\r");
    sys_test("loop");    
    printf("Success2!\n\r");
    while(1);
}