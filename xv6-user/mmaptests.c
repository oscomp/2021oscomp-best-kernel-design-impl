#include "user.h"

char *openmmap(char *file, uint64 *len)
{
    int fd;
    struct kstat stat;
    char *start;

    if ((fd = open(file, O_RDWR)) < 0) {
        return NULL;
    }
    if (fstat(fd, &stat) < 0) {
        close(fd);
        return NULL;
    }
    *len = stat.size;
    start = mmap(NULL, stat.size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (start == MAP_FAILED) {
        return NULL;
    }
    return start;
}

void sharewirte(char *testname)
{
    int pid;
    char *start;
    uint64 len;

    pid = fork();
    if (pid < 0) {
        fprintf(2, "fork fail\n");
        exit(1);
    }
    else if (pid == 0) {
        start = openmmap("README", &len);
        if (start == NULL) {
            fprintf(2, "mmap fail\n");
            exit(1);
        }
        for (int i = 5; i < 10; i++) {
            start[i] = '0' + i;
        }
        munmap(start, len);
    } else {
        start = openmmap("README", &len);
        if (start == NULL) {
            fprintf(2, "mmap fail\n");
            exit(1);
        }
        for (int i = 0; i < 5; i++) {
            start[i] = '0' + i;
        }
        munmap(start, len);
        wait(NULL);
        int fd = open("README", O_RDONLY);
        if (fd < 0) {
            fprintf(2, "open README fail\n");
            exit(1);
        }
        char buf[11];
        buf[10] = 0;
        if (read(fd, buf, 10) != 10) {
            fprintf(2, "read README fail\n");
            exit(1);
        }
        close(fd);
        if (strcmp(buf, "0123456789") != 0) {
            fprintf(2, "read unexpected: %s\n", buf);
            exit(1);
        }
    }
    exit(0);
}

void outoflen(char *testname)
{
    int fd;
    struct kstat stat;
    if ((fd = open("README", O_RDWR)) < 0) {
        fprintf(2, "open README fail\n");
        exit(1);
    }
    if (fstat(fd, &stat) < 0) {
        fprintf(2, "stat fd %d fail\n", fd);
        exit(1);
    }
    char *addr;
    addr = mmap(NULL, stat.size + 1, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr != MAP_FAILED) {
        fprintf(2, "mmap out of len success!\n");
        exit(1);
    }
    exit(0);
}


void sharedrw(char *testname)
{
    int pid;
    char *start;
    uint64 len;
    char ch = '@';

    pid = fork();
    if (pid < 0) {
        fprintf(2, "fork fail\n");
        exit(1);
    }
    else if (pid == 0) {
        start = openmmap("README", &len);
        if (start == NULL) {
            fprintf(2, "mmap fail\n");
            exit(1);
        }
        start[0] = ch;
        munmap(start, len);
    } else {
        start = openmmap("README", &len);
        if (start == NULL) {
            fprintf(2, "mmap fail\n");
            exit(1);
        }
        int count = 10;
        while (count-- && start[0] != ch) {
            sleep(3);
        }
        printf("get %c\n", start[0]);
        munmap(start, len);
    }
    exit(0);
}


void wrongprot(char *testname)
{
    int fd;
    if ((fd = open("README", O_RDONLY)) < 0) {
        fprintf(2, "open README fail\n");
        exit(1);
    }
    char *addr;
    addr = mmap(NULL, 1, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr != MAP_FAILED) {
        fprintf(2, "writable-map on read-only fd success!\n");
        exit(1);
    }
    addr = mmap(NULL, 1, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        fprintf(2, "mmap fail\n");
        exit(1);
    }
    if (fork() == 0) {
        addr[0] = 'x'; // this should raise a page fault
        exit(0);
    }
    int xstatus;
    wait(&xstatus);
    if (WEXITSTATUS(xstatus) != (char)-1) {
        fprintf(2, "write to read-only mmap success!\n");
        exit(1);
    }
    exit(0);
}


int run(void f(char *), char *s) {
    int pid;
    int xstatus;

    printf("testing %s\n", s);
    if ((pid = fork()) < 0) {
        printf("runtest: fork error\n");
        exit(1);
    }
    if (pid == 0) {
        f(s);
        exit(0);
    }
    wait(&xstatus);
    if(WEXITSTATUS(xstatus) != 0) 
        printf("test %s: FAILED\n", s);
    else
        printf("test %s: OK\n", s);

    return WEXITSTATUS(xstatus) == 0;
}


int main(int argc, char *argv[]){
    // trace(1);
    char *testname = NULL;
    if (argc > 1)
        testname = argv[1];

    struct test {
        void (*f)(char *);
        char *s;
    } tests[] = {
        { sharewirte, "sharewirte" },
        { outoflen, "outoflen" },
        { wrongprot, "wrongprot" },
        { sharedrw, "sharedrw" },
        { 0, 0 },
    };

      int fail = 0;
    for (struct test *t = tests; t->s != 0; t++) {
        if((testname == 0) || strcmp(t->s, testname) == 0) {
            if(!run(t->f, t->s))
                fail = 1;
        }
    }
    if(!fail)
        printf("ALL TESTS PASSED\n");
    else
        printf("SOME TESTS FAILED\n");
    exit(0);
}