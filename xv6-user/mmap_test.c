#include "user.h"

int main(){
    trace(1);
    int fd = open("README", O_RDWR);
    
    struct kstat st;

    if (fstat(fd, &st) < 0)
        exit(0);

    char * start = mmap(0, st.size, 3, 1, fd, 0);
    write(1, start, st.size);
    exit(0);
}