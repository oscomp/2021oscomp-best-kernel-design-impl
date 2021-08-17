//
// Console input and output, to the uart.
// Reads are line at a time.
// Implements special input characters:
//   newline -- end of line
//   control-h -- backspace
//   control-u -- kill line
//   control-d -- end of file
//   control-p -- print process list
//
#include <stdarg.h>

#ifndef __DEBUG_console
#undef DEBUG
#endif

#define __module_name__ 	"console"


#include "types.h"
#include "param.h"
#include "sync/spinlock.h"
#include "sync/sleeplock.h"
#include "fs/file.h"
#include "memlayout.h"
#include "hal/riscv.h"
#include "sched/proc.h"
#include "sbi.h"
#include "mm/kmalloc.h"
#include "mm/pm.h"
#include "fs/buf.h"
#include "printf.h"
#include "mm/vm.h"
#include "utils/debug.h"
#include "fs/buf.h"
#include "errno.h"

#define BACKSPACE 0x100
#define C(x)  ((x)-'@')  // Control-x

void consputc(int c) {
	if(c == BACKSPACE){
		// if the user typed backspace, overwrite with a space.
		sbi_console_putchar('\b');
		sbi_console_putchar(' ');
		sbi_console_putchar('\b');
	} else {
		sbi_console_putchar(c);
	}
}

struct {
	struct spinlock lock;
	
	// input
#define INPUT_BUF 128
	char buf[INPUT_BUF];
	uint r;  // Read index
	uint w;  // Write index
	uint e;  // Edit index
} cons;

//
// user write()s to the console go here.
//
int
consolewrite(int user_src, uint64 src, int n)
{
	int m, tot;
	char outbuf[INPUT_BUF];

	// acquire(&cons.lock);	// use inode's lock instead
	for (tot = 0; tot < n; tot += m, src += m) {
		m = n - tot;  // left count
		if (m > INPUT_BUF) {
			m = INPUT_BUF;
		}
		if (either_copyin_nocheck(outbuf, user_src, src, m) == -1)
			break;
		for (int i = 0; i < m; i++) {
			sbi_console_putchar(outbuf[i]);
		}
	}
	// release(&cons.lock);

	return tot;
}

int consolewritev(struct inode *ip, struct iovec *iovecs, int count, uint off)
{
	int tot = 0;
	char outbuf[INPUT_BUF];

	// acquire(&cons.lock);	// use inode's lock instead
	for (int i = 0; i < count; i++) {
		uint64 addr = (uint64)iovecs[i].iov_base;
		uint64 n = iovecs[i].iov_len;
		if (!rangeinseg(addr, addr + n))
			return -EFAULT;

		int m, j;
		for (j = 0; j < n; j += m, tot += m) {
			m = n - j;  // left count
			if (m > INPUT_BUF)
				m = INPUT_BUF;
			if (copyin_nocheck(outbuf, addr + j, m) < 0)
				break;
			for (int c = 0; c < m; c++) {
				sbi_console_putchar(outbuf[c]);
			}
		}
		if (j < n)
			break;
	}
	// release(&cons.lock);
	return tot;
}

//
// user read()s from the console go here.
// copy (up to) a whole input line to dst.
// user_dist indicates whether dst is a user
// or kernel address.
//
int
consoleread(int user_dst, uint64 dst, int n)
{
	int c = 0;
	char inbuf[INPUT_BUF];
	uint tot = 0;

	while (n > 0) {
		// wait until interrupt handler has put some
		// input into cons.buffer.
		acquire(&cons.lock);
		while (cons.r == cons.w) {
			if (myproc()->killed == SIGTERM) {
				release(&cons.lock);
				return -1;
			}
			sleep(&cons.r, &cons.lock);
		}

		int m = n < INPUT_BUF ? n : INPUT_BUF;
		int j;
		for (j = 0; j < m && cons.r < cons.w; ) {
			c = inbuf[j++] = cons.buf[cons.r++ % INPUT_BUF];
			if (c == '\n')
				break;
			if (c == C('D')) {
				if (tot > 0) {	// Save ^D for the next time, 
					cons.r--;	// to make sure caller gets a 0-byte result.
				}
				j--;
				break;
			}
		}
		release(&cons.lock);
		
		// copy the input byte to the user-space buffer.
		if (either_copyout_nocheck(user_dst, dst, inbuf, j) < 0)
			break;
		dst += j;
		n -= j;
		tot += j;
		if (c == '\n' || c == C('D'))
			break;
	}

	return tot;
}

int consolereadv(struct inode *ip, struct iovec *iovecs, int count, uint off)
{
	uint tot = 0;
	int c = 0;
	char inbuf[INPUT_BUF];

	for (int i = 0; i < count; i++) {
		uint64 dst = (uint64)iovecs[i].iov_base;
		uint64 n = iovecs[i].iov_len;
		if (!rangeinseg(dst, dst + n))
			return -EFAULT;

		while (n > 0) {
			// wait until interrupt handler has put some
			// input into cons.buffer.
			acquire(&cons.lock);
			while (cons.r == cons.w) {
				if (myproc()->killed == SIGTERM) {
					release(&cons.lock);
					return -1;
				}
				sleep(&cons.r, &cons.lock);
			}

			int m = n < INPUT_BUF ? n : INPUT_BUF;
			int j;
			for (j = 0; j < m && cons.r < cons.w; ) {
				c = inbuf[j++] = cons.buf[cons.r++ % INPUT_BUF];
				if (c == '\n')
					break;
				if (c == C('D')) {
					if (tot > 0) {	// Save ^D for the next time, 
						cons.r--;	// to make sure caller gets a 0-byte result.
					}
					j--;
					break;
				}
			}
			release(&cons.lock);

			// copy the input byte to the user-space buffer.
			if (copyout_nocheck(dst, inbuf, j) < 0) {
				count = 0; // break the out-level loop
				break;
			}
			dst += j;
			n -= j;
			tot += j;
			if (c == '\n' || c == C('D')) {
				count = 0;
				break;
			}
		}
	}
	return tot;
}


//
// the console input interrupt handler.
// uartintr() calls this for input character.
// do erase/kill processing, append to cons.buf,
// wake up consoleread() if a whole line has arrived.
//
void
consoleintr(int c)
{
	acquire(&cons.lock);

	switch(c){
	case C('P'):  // Print process list.
		procdump();
		break;
	case C('E'):  // Print file-entry list.
		rootfs_print();
		break;
	case C('K'):  // Print mem-alloc list.
		kmview();
		break;
	case C('B'):  // Print mem-alloc list.
		bprint();
		break;
	case C('Q'):  // Print physical memory status
		pm_dump();
		break;
	case C('U'):  // Kill line.
		while(cons.e != cons.w &&
					cons.buf[(cons.e-1) % INPUT_BUF] != '\n'){
			cons.e--;
			consputc(BACKSPACE);
		}
		break;
	case C('H'): // Backspace
	case '\x7f':
		if(cons.e != cons.w){
			cons.e--;
			consputc(BACKSPACE);
		}
		break;
	default:
		if(c != 0 && cons.e-cons.r < INPUT_BUF){
			#ifndef QEMU
			if (c == '\r') break;     // on k210, "enter" will input \n and \r
			#else
			c = (c == '\r') ? '\n' : c;
			#endif
			// echo back to the user.
			consputc(c);

			// store for consumption by consoleread().
			cons.buf[cons.e++ % INPUT_BUF] = c;

			if(c == '\n' || c == C('D') || cons.e == cons.r+INPUT_BUF){
				// wake up consoleread() if a whole line (or end-of-file)
				// has arrived.
				cons.w = cons.e;
				wakeup(&cons.r);
			}
		}
		break;
	}
	
	release(&cons.lock);
}

void
consoleinit(void)
{
	initlock(&cons.lock, "cons");

	cons.e = cons.w = cons.r = 0;
	
	// connect read and write system calls
	// to consoleread and consolewrite.
	// devsw[CONSOLE].read = consoleread;
	// devsw[CONSOLE].write = consolewrite;
}

#include "fs/fs.h"

int __consoleread(struct inode *ip, int usr, uint64 dst, uint off, uint n)
{
	return consoleread(usr, dst, n);
}

int __consolewrite(struct inode *ip, int usr, uint64 dst, uint off, uint n)
{
	return consolewrite(usr, dst, n);
}

extern int dummy_file_readdir(struct inode *, struct dirent *, uint);

struct file_op console_op = {
	.read = __consoleread,
	.write = __consolewrite,
	.readdir = dummy_file_readdir,
	.readv = consolereadv,
	.writev = consolewritev,
};