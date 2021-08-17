#include "types.h"
#include "fs/stat.h"
#include "fs/fcntl.h"
#include "sysinfo.h"
#include "sched/signal.h"

// struct stat;
struct rtcdate;
struct sysinfo;

#define WEXITSTATUS(s) (((s) & 0xff00) >> 8)

#define PROT_NONE 0
#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4
#define PROT_GROWSDOWN 0X01000000
#define PROT_GROWSUP 0X02000000

#define MAP_FILE 0
#define MAP_SHARED 0x01
#define MAP_PRIVATE 0X02
#define MAP_FIXED 0x10
#define MAP_ANONYMOUS 0x20

// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe2(int*, int);
static inline int pipe(int *fdarr) {
    return pipe2(fdarr, 0);
}
int write(int fd, const void *buf, int len);
int read(int fd, void *buf, int len);
int close(int fd);
int kill(int pid, int sig);
// int exec(char*, char**);
int execve(char *name, char *argv[], char *envp[]);
static inline int exec(char *name, char *argv[]) {
    return execve(name, argv, NULL);
}
int openat(int fd, const char *filename, int flag, int mode);
static inline int open(const char *filename, int flag) {
    return openat(AT_FDCWD, filename, flag, 0600);
}
int fstat(int fd, struct kstat*);
int mkdirat(int fd, const char *dirname, int mode);
static inline int mkdir(const char *dirname) {
    return mkdirat(AT_FDCWD, dirname, 0400);
}
int chdir(const char *dirname);
int dup(int fd);
int dup3(int oldfd, int newfd, int flag);
static inline int dup2(int oldfd, int newfd) {
    return oldfd == newfd ? newfd : dup3(oldfd, newfd, 0);
}
int getpid(void);
char* sbrk(int size);
char* brk(void *target);
int sleep(int ticks);
int uptime(void);
int test_proc(int);
// int dev(int, short, short);
char *getcwd(char *buf, uint size);
int unlinkat(int fd, char *filename, int flag);
static inline int unlink(char *filename) {
    return unlinkat(AT_FDCWD, filename, 0);
}
static inline int remove(char *filename) {
    return unlinkat(AT_FDCWD, filename, 0);
}
static inline int rmdir(char *dirname) {
    return unlinkat(AT_FDCWD, dirname, AT_REMOVEDIR);
}
int trace();
int sysinfo(struct sysinfo *);
int renameat2(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags);
static inline int rename(const char *old, const char *new) {
    return renameat2(AT_FDCWD, old, AT_FDCWD, new, 0);
}
int mount(char *dev, char *dir, char *fstype, uint64 flag, void *data);
int getdents(int fd, struct dirent *buf, unsigned long len);
int umount(char *dir, uint64 flag);
int uname(void *buf);
void *mmap(void *start, int len, int prot, int flags, int fd, int off);
int munmap(void *start, int len);
void sync(void);

void rt_sigaction(int signum, struct sigaction const *act, struct sigaction *oldact, int len);
void rt_sigprocmask(int how, __sigset_t *set, __sigset_t *oldset);

// ulib.c
int stat(const char*, struct kstat*);
char* strcpy(char*, const char*);
char* strcat(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...);
void printf(const char*, ...);
void putc(int ch, int fd);
void putchar(int ch);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);
