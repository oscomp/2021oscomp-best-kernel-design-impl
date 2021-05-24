#include <stdio.h>
#include <type.h>

int main()
{
    for (int32_t i = 0; i < 1000; ++i)
    {
        printf("%d\n", i);
        sys_move_cursor(1,i % 20);
    }
}
