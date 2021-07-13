extern int main();
extern int exit() __attribute__((noreturn));

__attribute__((naked)) void _start(void)
{
	asm("mv a0, sp");
	asm("tail __start_main");
}

__attribute__((noreturn)) void __start_main(long *p)
{
	int argc = p[0];
	char **argv = (void *)(++p);
	while (*p++ != 0);
	exit(main(argc, argv, p));
}
