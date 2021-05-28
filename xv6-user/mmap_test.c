#include "user.h"

int main(){
    trace(1);
    int fd = open("README", O_RDWR);
    
    struct kstat st;

    if (fstat(fd, &st) < 0)
        exit(0);

    char * start = mmap(0, st.size, 3, 1, fd, 0);
    write(1, start, st.size);
    printf("start writing\n");
    for (int i = 0; i < st.size; i++) {
        start[i] = 'x';
    }
    printf("write done\n");
    int ret = munmap(start, st.size);
    if (ret < 0) {
        fprintf(2, "munmap fail\n");
    }
    
    // this should raise a page fault
    start[0] = 1;

    exit(0);
}
