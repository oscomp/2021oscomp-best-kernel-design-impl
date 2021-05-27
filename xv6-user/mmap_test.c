#include "user.h"

int main(){
    int fd = open("README", O_RDWR);
    char * start = mmap(0, 4096, 6, 0, fd, 0);
    write(1, start, 10);
    exit(0);
}