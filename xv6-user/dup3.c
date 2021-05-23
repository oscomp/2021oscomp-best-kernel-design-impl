#include "user.h"

int main()
{
    int fd = dup3(1, 100, 0);
    if (fd < 0) {
        fprintf(2, "dup3 fail!\n");
    } else {
        if (write(fd, "dup3 success!\n", 15) < 0) {
            fprintf(2, "dup3 write to fd 100 fail!\n");
        }
        fprintf(fd, "write to fd 100, get %d\n", fd);
    }
    exit(0);
}