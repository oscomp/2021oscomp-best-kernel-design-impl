#include <stdio.h>

typedef int __attribute__((__mode__(SI))) int32_t;
int main()
{
    while(1){
        for (int32_t i = 0; i < 1000; ++i)
        {
            printf("%d\n", i);
            sys_move_cursor(1,i % 20);
        }
    }
}