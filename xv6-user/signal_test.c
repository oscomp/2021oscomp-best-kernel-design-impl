#include "user.h"

void handler(int sig) {
	printf("hello world sig = %d\n", sig);
	exit(0);
}

int main(int argc, char *argv[]) {
	int sig = 34;

	struct sigaction sigact;
		sigact.__sigaction_handler.sa_handler = handler;
		for (int i = 0; i < SIGSET_LEN; i ++) {
			sigact.sa_mask.__val[i] = 0;
		}

	rt_sigaction(sig, &sigact, NULL, 8);

	int pid = fork();
	if (0 == pid) {
		while (1) 
			;

		printf("error child exit!\n");
	}
	else {
		kill(pid, sig);
		int child = wait(NULL);

		printf("child %d\n", child);

		exit(0);
	}
}