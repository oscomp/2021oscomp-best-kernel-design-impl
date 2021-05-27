#include "kernel/include/sysnum.h"

#define NULL ((void*)0)

#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_APPEND  0x004
#define O_CREATE  0x040
#define O_TRUNC   0x400
#define O_DIRECTORY 0x0200000
#define CONSOLE 1

int exec(char *name, char *argv[]);
int fork(void);
int wait(int*);
int dev(int, short, short);
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
	/*"gettimeofday", */
	"mkdir_", 
	/*"mmap", */
	"mount", 
	/*"munmap", */
	"openat", 
	"open", 
	"pipe", 
	"read", 
	/*"times", */
	"umount", 
	"uname", 
	"unlink", 
	"wait", 
	"waitpid", 
	"write", 
	"yield", 
};
int const test_file_num = sizeof(test_files) / sizeof(char const*);

void _entry(void) __attribute__((naked));
void _entry(void) {
	dev(O_RDWR, CONSOLE, 0);
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
