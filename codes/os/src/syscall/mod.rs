const SYSCALL_WAITPID: usize = 7;
const SYSCALL_DUP: usize = 24;
const SYSCALL_CHDIR: usize = 49;
const SYSCALL_OPEN: usize = 56;
const SYSCALL_CLOSE: usize = 57;
const SYSCALL_PIPE: usize = 59;
const SYSCALL_READ: usize = 63;
const SYSCALL_WRITE: usize = 64;
const SYSCALL_EXIT: usize = 93;
const SYSCALL_YIELD: usize = 124;
const SYSCALL_GET_TIME: usize = 169;
const SYSCALL_GETPID: usize = 172;
const SYSCALL_GETPPID: usize = 173;
const SYSCALL_SBRK: usize = 213;
const SYSCALL_BRK: usize = 214;
const SYSCALL_FORK: usize = 220;
const SYSCALL_EXEC: usize = 221;
const SYSCALL_WAIT4: usize = 260;

// Not standard POSIX sys_call
const SYSCALL_LS: usize = 500;
const SYSCALL_SHUTDOWN: usize = 501;

mod fs;
mod process;

pub use fs::*;
use process::*;
use crate::sbi::shutdown;

pub fn syscall(syscall_id: usize, args: [usize; 3]) -> isize {
    match syscall_id {
        SYSCALL_DUP=> sys_dup(args[0]),
        //SYSCALL_CHDIR=> sys_chdir(args[0] as *const u8),
        SYSCALL_OPEN => sys_open(args[0] as *const u8, args[1] as u32),
        SYSCALL_CLOSE => sys_close(args[0]),
        SYSCALL_PIPE => sys_pipe(args[0] as *mut usize),
        SYSCALL_READ => sys_read(args[0], args[1] as *const u8, args[2]),
        SYSCALL_WRITE => sys_write(args[0], args[1] as *const u8, args[2]),
        SYSCALL_EXIT => sys_exit(args[0] as i32),
        SYSCALL_YIELD => sys_yield(),
        SYSCALL_GET_TIME => sys_get_time(),
        SYSCALL_SBRK => sys_sbrk(args[0] as isize, args[1]) as isize,
        SYSCALL_BRK => sys_brk(args[0]),
        SYSCALL_GETPID => sys_getpid(),
        SYSCALL_GETPPID => sys_getppid(),
        SYSCALL_FORK => sys_fork(),
        SYSCALL_EXEC => sys_exec(args[0] as *const u8, args[1] as *const usize),
        SYSCALL_WAIT4 => sys_wait4(args[0] as isize, args[1] as *mut i32, args[2] as isize),
        SYSCALL_WAITPID => sys_waitpid(args[0] as isize, args[1] as *mut i32),
        //SYSCALL_LS => sys_ls(args[0] as *const u8),
        SYSCALL_SHUTDOWN => {shutdown();0}
        _ => panic!("Unsupported syscall_id: {}", syscall_id),
    }
}

