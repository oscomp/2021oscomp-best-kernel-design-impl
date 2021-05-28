#include "kernel/include/sysnum.h"
#include "kernel/include/fcntl.h"

#define NULL ((void*)0)


int exec(char *name, char *argv[]);
int fork(void);
int wait(int*);
int openat(int fd, const char *filename, int flag, int mode);
int dup(int);
int exit(int) __attribute__((noreturn));

char *test_files[] = {
	"brk", 
	"chdir", 
	"clone", 
	"close", 
	"dup2", 
	"dup", 
	"execve", 
	"exit", 
	"fork", 
	"fstat", 
	"getcwd", 
	"getdents", 
	"getpid", 
	"getppid", 
	"gettimeofday",
	"mkdir_", 
	"mmap",
	"mount", 
	"munmap",
	"openat", 
	"open", 
	"pipe", 
	"read", 
	"times",
	"umount", 
	"uname", 
	"unlink", 
	"wait", 
	"waitpid", 
	"write", 
	"yield", 
};
int const test_file_num = sizeof(test_files) / sizeof(char const*);

const char *cons = "/dev/console";

void _entry(void) __attribute__((naked));
void _entry(void) {
	openat(AT_FDCWD, cons, O_RDWR, 0666);
	dup(0);  // stdout
	dup(0);  // stderr

	for (int i = 0; i < test_file_num; i ++) {
		if (fork() == 0) {
			exec(test_files[i], NULL);
		}
		else {
			wait(NULL);
		}
	}
	for (;;)
		;
	
	exit(0);
}
