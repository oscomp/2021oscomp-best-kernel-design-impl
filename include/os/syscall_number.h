/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                       System call related processing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#ifndef OS_SYSCALL_NUMBER_H_
#define OS_SYSCALL_NUMBER_H_
#define IGNORE 0
#define NUM_SYSCALLS 512

/* define */
#define SYS_getcwd 17
#define SYSCALL_WRITE 20
#define SYSCALL_CURSOR 22
#define SYS_dup 23
#define SYS_dup3 24

#define SYS_mkdirat 34
#define SYS_chdir 49
#define SYS_openat 56
#define SYS_close 57
#define SYSCALL_TESTDISK 58
#define SYSCALL_TEST 59

#define SYS_getdents64 61
#define SYS_read 63
#define SYS_write 64

#define SYS_fstat 80

#define SYS_exit 93

#define SYS_nanosleep 101

#define SYS_sched_yield 124
#define SYS_times 153
#define SYS_uname 160
#define SYS_gettimeofday 169
#define SYS_getpid 172
#define SYS_getppid 173

#define SYS_brk 214
#define SYS_clone 220
#define SYS_execve 221

#define SYS_mmap 222

#define SYS_wait4 260

#endif
