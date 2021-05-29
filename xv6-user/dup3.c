#include "user.h"

int main(int argc, char *argv[])
{
    int fd;
    
    if (argc <= 1) {
        // fd = dup3(1, 100, 0);
        fd = dup3(1, 100, FD_CLOEXEC);
        if (fd < 0) {
            fprintf(2, "dup3 fail!\n");
            exit(1);
        }
        if (write(fd, "dup3 success!\n", 15) < 0) {
            fprintf(2, "dup3 write to fd 100 fail!\n");
            exit(1);
        }
        fprintf(fd, "write to fd 100, get %d\n", fd);
        fprintf(fd, "now test cloexe\n");

        char *argvs[] = { "./bin/dup3", "100" };
        exec("./bin/dup3", argvs);
        fprintf(2, "exec fail!\n");
        exit(1);
    }
    else {
        fd = atoi(argv[1]);
        printf("get fd %d\n", fd);
        if (write(fd, "is this fd still exists?\n", 26) >= 0) {
            fprintf(2, "fd %d still exists!\n", fd);
        }
        else {
            printf("fail to write, OK\n");
        }
    }
    exit(0);
}