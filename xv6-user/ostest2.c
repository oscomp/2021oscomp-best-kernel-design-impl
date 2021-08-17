#include "types.h"
#include "fs/file.h"
#include "fs/fcntl.h"
#include "fs/stat.h"
#include "user.h"

int exec(char *name, char *argv[]);
int fork(void);

char *argv[] = {
	"busybox", 
	"sh", 
	"lmbench_testcode.sh", 
	0
};

int main(void) {
	open("/dev/console", O_RDWR);
	dup(0);
	dup(0);

	int pid = fork();
	if (0 == pid) {
		exec("busybox", argv);
	}
	else {
		while (1) {
			wait(0);
		}
	}

	exit(0);

	return 0;
}