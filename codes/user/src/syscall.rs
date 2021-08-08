const SYSCALL_GETCWD: usize = 17;
const SYSCALL_DUP: usize = 23;
const SYSCALL_DUP3:usize = 24;
const SYSCALL_FCNTL:usize = 25;
const SYSCALL_IOCTL:usize = 29;
const SYSCALL_MKDIRAT: usize = 34;
const SYSCALL_UNLINKAT: usize = 35;
const SYSCALL_LINKAT: usize = 37;
const SYSCALL_UMOUNT2: usize = 39;
const SYSCALL_MOUNT: usize = 40;
const SYSCALL_FACCESSAT: usize = 48;
const SYSCALL_CHDIR: usize = 49;
const SYSCALL_OPENAT: usize = 56;
const SYSCALL_CLOSE: usize = 57;
const SYSCALL_PIPE: usize = 59;
const SYSCALL_GETDENTS64: usize = 61;
const SYSCALL_READ: usize = 63;
const SYSCALL_WRITE: usize = 64;
const SYSCALL_WRITEV: usize = 66;
const SYSCALL_SENDFILE: usize = 71;
const SYSCALL_PSELECT6: usize = 72;
const SYSCALL_READLINKAT: usize = 78;
const SYSCALL_NEW_FSTATAT: usize = 79;
const SYSCALL_FSTAT:usize = 80;
const SYSCALL_UTIMENSAT:usize = 88;
const SYSCALL_EXIT: usize = 93;
const SYSCALL_EXIT_GRUOP: usize = 94;
const SYSCALL_SET_TID_ADDRESS: usize = 96;
const SYSCALL_NANOSLEEP: usize = 101;
const SYSCALL_CLOCK_GETTIME: usize = 113;
const SYSCALL_YIELD: usize = 124;
const SYSCALL_TIMES: usize = 153;
const SYSCALL_UNAME: usize = 160;
const SYSCALL_GETRUSAGE: usize = 165;
const SYSCALL_GET_TIME_OF_DAY: usize = 169;
const SYSCALL_GETPID: usize = 172;
const SYSCALL_GETPPID: usize = 173;
const SYSCALL_GETUID: usize = 174;
const SYSCALL_GETEUID: usize = 175;
const SYSCALL_GETGID: usize = 176;
const SYSCALL_GETEGID: usize = 177;
const SYSCALL_GETTID: usize = 177;
const SYSCALL_SBRK: usize = 213;
const SYSCALL_BRK: usize = 214;
const SYSCALL_MUNMAP: usize = 215;
const SYSCALL_CLONE: usize = 220;
const SYSCALL_EXEC: usize = 221;
const SYSCALL_MMAP: usize = 222;
const SYSCALL_MPROTECT: usize = 226;
const SYSCALL_WAIT4: usize = 260;
const SYSCALL_RENAMEAT2: usize = 276;

// Not standard POSIX sys_call
const SYSCALL_LS: usize = 500;
const SYSCALL_SHUTDOWN: usize = 501;
const SYSCALL_CLEAR: usize = 502;

use alloc::string::String;
use super::TimeVal;

// pub struct TimeVal{
//     sec: u64,
//     usec: u64,
// }

fn syscall(id: usize, args: [usize; 3]) -> isize {
    let mut ret: isize;
    unsafe {
        llvm_asm!("ecall"
            : "={x10}" (ret)
            : "{x10}" (args[0]), "{x11}" (args[1]), "{x12}" (args[2]), "{x17}" (id)
            : "memory"
            : "volatile"
        );
    }
    ret
}

pub fn sys_dup(fd: usize) -> isize {
    syscall(SYSCALL_DUP, [fd, 0, 0])
}

pub fn sys_chdir(path: &str) -> isize {
    syscall(SYSCALL_CHDIR, [path.as_ptr() as usize, 0, 0])
}

pub fn sys_unlinkat(fd:i32, path: &str, flags:u32) -> isize {
    syscall(SYSCALL_UNLINKAT, [fd as usize, path.as_ptr() as usize, flags as usize])
}

pub fn sys_open(path: &str, flags: u32) -> isize {
    let path_str = String::new() + path + "\0";
    syscall(SYSCALL_OPENAT, [(-100 as isize) as usize, path_str.as_ptr() as usize, flags as usize])
}

pub fn sys_mkdir(path: &str) -> isize {
    let path_str = String::new() + path + "\0";
    syscall(SYSCALL_MKDIRAT, [(-100 as isize) as usize, path_str.as_ptr() as usize, 0])
}

pub fn sys_close(fd: usize) -> isize {
    syscall(SYSCALL_CLOSE, [fd, 0, 0])
}

pub fn sys_pipe(pipe: &mut [usize]) -> isize {
    syscall(SYSCALL_PIPE, [pipe.as_mut_ptr() as usize, 0, 0])
}

pub fn sys_read(fd: usize, buffer: &mut [u8]) -> isize {
    syscall(SYSCALL_READ, [fd, buffer.as_mut_ptr() as usize, buffer.len()])
}

pub fn sys_write(fd: usize, buffer: &[u8]) -> isize {
    syscall(SYSCALL_WRITE, [fd, buffer.as_ptr() as usize, buffer.len()])
}

pub fn sys_exit(exit_code: i32) -> ! {
    syscall(SYSCALL_EXIT, [exit_code as usize, 0, 0]);
    panic!("sys_exit never returns!");
}

pub fn sys_yield() -> isize {
    syscall(SYSCALL_YIELD, [0, 0, 0])
}

pub fn sys_get_time(time:&mut TimeVal) -> isize {
    unsafe{
        syscall(SYSCALL_GET_TIME_OF_DAY, [time as *mut TimeVal as usize, 0, 0])
    }    
}

pub fn sys_getpid() -> isize {
    syscall(SYSCALL_GETPID, [0, 0, 0])
}
pub fn sys_getppid() -> isize {
    syscall(SYSCALL_GETPPID, [0, 0, 0])
}

pub fn sys_fork() -> isize {
    //SIGCHLD=17
    syscall(SYSCALL_CLONE, [17, 0, 0])
}

pub fn sys_exec(path: &str, args: &[*const u8]) -> isize {
    // println!{"calling exec1..."}
    let ret = syscall(SYSCALL_EXEC, [path.as_ptr() as usize, args.as_ptr() as usize, 0]);
    // println!{"returned from os..."};
    ret
}

pub fn sys_waitpid(pid: isize, exit_code: *mut i32) -> isize {
    syscall(SYSCALL_WAIT4, [pid as usize, exit_code as usize, 0])
    // syscall(SYSCALL_WAITPID, [pid as usize, exit_code as usize, 0])
}

pub fn sys_wait4(pid: isize, wstatus: *mut i32, option: usize) -> isize {
    syscall(SYSCALL_WAIT4, [pid as usize, wstatus as usize, option])
}

pub fn sys_sleep(period_ms: usize)  -> isize {
    let time = TimeVal{sec:0, usec:period_ms*1000};
    syscall(SYSCALL_NANOSLEEP, [&time as *const TimeVal as usize,  &time as *const TimeVal as usize,0])
}

// Not standard POSIX sys_call
pub fn sys_ls(path:&str) -> isize {
    println!("ready for syscall ls");
    syscall(SYSCALL_LS, [path.as_ptr() as usize, 0, 0])
}
pub fn sys_shutdown() -> isize {
    syscall(SYSCALL_SHUTDOWN, [0, 0, 0])
}
pub fn sys_trap() -> isize {
    syscall(1000, [0, 0, 0])
}