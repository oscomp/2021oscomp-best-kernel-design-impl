#include "user.h"

int
countfree()
{
    int fds[2];
    if (pipe(fds) < 0) {
        printf("pipe() failed in countfree()\n");
        exit(1);
    }
    int pid = fork();
    if (pid < 0) {
        printf("fork failed in countfree()\n");
        exit(1);
    }
    if (pid == 0) {
        close(fds[0]);
        while (1) {
            uint64 a = (uint64) sbrk(4096);
            if (a == 0xffffffffffffffff)
                break;

            // modify the memory to make sure it's really allocated.
            *(char *)(a + 4096 - 1) = 1;

            // report back one more page.
            if (write(fds[1], "x", 1) != 1) {
                printf("write() failed in countfree()\n");
                exit(1);
            }
        }
        exit(0);
    }

    close(fds[1]);
    int n = 0;
    while (1) {
        char c;
        int cc = read(fds[0], &c, 1);
        if (cc < 0) {
            printf("read() failed in countfree()\n");
            exit(1);
        }
        if (cc == 0)
            break;
        n += 1;
    }

    close(fds[0]);
    wait((int*)0);
    return n;
}


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
    if ((int64)start < 0) {
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
        fprintf(2, "fs/stat fd %d fail\n", fd);
        exit(1);
    }
    char *addr;
    unsigned len = stat.size + 4096;
    addr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if ((int64)addr < 0) {
        fprintf(2, "mmap out of file size fail!\n");
        exit(1);
    }
    if (fork() == 0) {
        int data = addr[len];
        printf("get %d\n", data);
        exit(0);
    }
    int xstatus;
    wait(&xstatus);
    if (WEXITSTATUS(xstatus) != (char)-1) {
        fprintf(2, "access mmap page out of file size success!\n");
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
        // printf("get %c\n", start[0]);
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
    if ((int64)addr >= 0) {
        fprintf(2, "writable-map on read-only fd success!\n");
        exit(1);
    }
    addr = mmap(NULL, 1, PROT_READ, MAP_SHARED, fd, 0);
    if ((int64)addr < 0) {
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

void anonsharedrw(char *testname)
{
    int pid, i;
    char *addr;
    addr = mmap(NULL, 0x2000, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if ((int64)addr < 0) {
        fprintf(2, "mmap fail\n");
        exit(1);
    }
    pid = fork();
    if (pid < 0) {
        fprintf(2, "%s: fork fail!\n", testname);
        exit(1);
    }
    if (pid == 0) {
        /**
         * The child writes to the second page, where the parent
         * did not touch (not mapped). Then the child exit()s,
         * to see if the page still remains to let the parent read.
         */
        for (i = 0; i < 128; i++)
            addr[i + 0x1000] = i;
        
        sleep(5);
        for (i = 0; i < 128; i++)
            if (addr[i] != i) {
                fprintf(2, "can't read what the parent wrote!\n");
                exit(1);
            }
        
        munmap(addr, 0x2000);   // to see if this affect the parent
        exit(0);
    } else {
        for (i = 0; i < 128; i++)
            addr[i] = i;
        wait(NULL);
        for (i = 0; i < 128; i++)
            if (addr[i + 0x1000] != i) {
                fprintf(2, "can't read what the child wrote!\n");
                exit(1);
            }
    }
}

void shareprivate(char *testname)
{
    int pid, fd, i;
    char *addr, ch;

    fd = open("README", O_RDWR);
    if (fd < 0) {
        fprintf(2, "%s: can't open file\n", testname);
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        fprintf(2, "%s: fork fail!\n", testname);
        exit(1);
    }

    if (pid == 0) {
        pid = fork();
        if (pid < 0) {
            fprintf(2, "%s: fork fail!\n", testname);
            exit(1);
        }

        addr = mmap(NULL, 0x1000, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
        if ((int64)addr < 0) {
            fprintf(2, "mmap fail\n");
            exit(1);
        }

        if (pid == 0) {
            i = 0;
            ch = '0';
        } else {
            i = 1;
            ch = '1';
        }
        for (; i < 0x1000; i += 2)      // write data
            addr[i] = ch;

        sleep(10);

        for (i = 0; i < 0x1000; i++) {  // check what all the procs wrote
            if (addr[i] != (i % 2) + '0') {
                fprintf(2, "%s: read wrong data\n", testname);
                exit(1);
            }
        }
        munmap(addr, 0x1000);
        if (pid > 0)
            wait(NULL);
    } else {
        // to see if we can steal some data from the shared mapping (we should)
        addr = mmap(NULL, 0x1000, PROT_WRITE|PROT_READ, MAP_PRIVATE, fd, 0);
        if ((int64)addr < 0) {
            fprintf(2, "mmap fail\n");
            exit(1);
        }
        sleep(5);
        for (i = 0; i < 0x1000; i++) {
            if (addr[i] != (i % 2) + '0') {
                fprintf(2, "%s: can't read the right data\n", testname);
                exit(1);
            }
        }
        // now see if we can mess up the shared mapping (we shouldn't)
        for (i = 0; i < 0x1000; i++)
            addr[i] = 'x';

        wait(NULL);

        // now the children's work should be carried to the file, try reading
        i = read(fd, addr, 0x1000);
        if (i < 0) {
            fprintf(2, "%s: can't read from file\n", testname);
            exit(1);
        }
        for (i-- ; i >= 0; i--) {
            if (addr[i] != (i % 2) + '0') {
                fprintf(2, "%s: can't read the right data from file\n", testname);
                exit(1);
            }
        }
    }
    close(fd);
    exit(0);
}

void ruinheap(char *testname)
{
    char *brkpoint, *addr, *old;
    old = brk(NULL);

    addr = mmap(old, 0x1000, PROT_NONE, MAP_ANONYMOUS|MAP_PRIVATE|MAP_FIXED, -1, 0);
    if ((int64)addr < 0) {
        fprintf(2, "%s: mmap fail\n", testname);
        exit(1);
    }
    if (addr != old) {
        fprintf(2, "%s: mmap not fixed! %p against %p\n", testname, addr, old);
        exit(1);
    }

    // this shall fail
    brkpoint = brk(old + 0x1000);
    if (brkpoint != old) {
        fprintf(2, "%s: brk success! break the mmap region!\n", testname);
        exit(1);
    }
    
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "%s: fork fail\n", testname);
        exit(1);
    }
    if (pid == 0) {
        printf("%s: read PROT_NONE mmap success! get %d\n", testname, addr[0]);
        exit(0);
    }
    int status;
    wait(&status);
    if (WEXITSTATUS(status) == 0) {
        fprintf(2, "%s: the child survived\n", testname);
        exit(1);
    }

    // take away the obstacle
    munmap(addr, 0x1000);
    addr = old + 0x1000;
    brkpoint = brk(addr);
    if (brkpoint != addr) {
        fprintf(2, "%s: brk fail!\n", testname);
        exit(1);
    }
    old[0] = 0; // can we survive?

    addr = mmap(old, 0x2000, PROT_NONE, MAP_ANONYMOUS|MAP_PRIVATE|MAP_FIXED, -1, 0);
    if ((int64)addr < 0) {
        fprintf(2, "%s: mmap fail\n", testname);
        exit(1);
    }
    if (addr != old) {
        fprintf(2, "%s: mmap not fixed! %p against %p\n", testname, addr, old);
        exit(1);
    }

    if (brk(brkpoint - 0x1000) != brkpoint ||
        brk(brkpoint + 0x2000) != brkpoint)
    {
        fprintf(2, "%s: heap is still alive!!\n", testname);
        exit(1);
    }
    exit(0);
}

void mmaploop(char *testname)
{
    int fd = open("mmap_junk", O_CREATE|O_TRUNC|O_RDWR);
    if (fd < 0) {
        fprintf(2, "%s: open fail!\n", testname);
        exit(1);
    }

    char *addr = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    if ((int64)addr < 0) {
        fprintf(2, "%s: mmap buf fail with %d!\n", testname, (int64)addr);
        exit(1);
    }
    for (int i = 0; i < 128; i++) {
        if (write(fd, addr, 0x1000) != 0x1000) {
            fprintf(2, "%s: write fail!\n", testname);
            exit(1);
        }
    }
    munmap(addr, 0x1000);

    for (int i = 0; i < 8; i++) {
        addr = mmap(NULL, 128 * 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        for (int j = 0; j < 128 * 0x1000; j += 0x1000)
            *(addr + j) = '0';
        munmap(addr, 128 * 0x1000);
    }
    close(fd);
    unlink("mmap_junk");
    exit(0);
}

void mmapswap(char *testname)
{
    int fd = open("mmap_swap", O_CREATE|O_TRUNC|O_RDWR);
    if (fd < 0) {
        fprintf(2, "%s: open fail!\n", testname);
        exit(1);
    }

    uint64 const filesz = 8 * 1024 * 1024;    // 8M
    char *addr = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    if ((int64)addr < 0) {
        fprintf(2, "%s: mmap buf fail with %d!\n", testname, (int64)addr);
        exit(1);
    }
    for (int i = 0; i < filesz / 0x1000; i++) {
        if (write(fd, addr, 0x1000) != 0x1000) {
            fprintf(2, "%s: write fail!\n", testname);
            exit(1);
        }
    }
    munmap(addr, 0x1000);

    addr = mmap(NULL, filesz, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    for (uint64 j = 0; j < filesz; j += 0x1000)
        *(uint64 *)(addr + j) = j;
    for (uint64 j = 0; j < filesz; j += 0x2000)
        if (*(uint64 *)(addr + j) != j) {
            fprintf(2, "%s: wrong data at offset %p\n", testname, j);
            exit(1);
        }
    munmap(addr, filesz);
    close(fd);
    unlink("mmap_swap");
    exit(0);
}

int run(void f(char *), char *s) {
    int pid;
    int xstatus;

    if ((pid = fork()) < 0) {
        printf("runtest: fork error\n");
        exit(1);
    }
    if (pid == 0) {
        f(s);
        exit(0);
    }
    printf("test %s: ", s);
    wait(&xstatus);
    if(WEXITSTATUS(xstatus) != 0) 
        printf("FAILED\n", s);
    else
        printf("OK\n", s);

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
        { anonsharedrw, "anonsharedrw" },
        { shareprivate, "shareprivate" },
        { ruinheap, "ruinheap" },
        { mmaploop, "mmaploop" },
        // { mmapswap, "mmapswap" },
        { 0, 0 },
    };

    printf("mmaptests starting\n");
    int free0 = countfree();
    int free1 = 0;
    int fail = 0;
    for (struct test *t = tests; t->s != 0; t++) {
        if ((testname == 0) || strcmp(t->s, testname) == 0) {
            if (!run(t->f, t->s))
                fail = 1;
        }
    }
    if (fail) {
        printf("SOME TESTS FAILED\n");
        exit(1);
    } else if ((free1 = countfree()) < free0) {
        printf("FAILED -- lost some free pages %d (out of %d)\n", free1, free0);
        exit(1);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    exit(0);
}