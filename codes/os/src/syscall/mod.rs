const SYSCALL_GETCWD: usize = 17;
const SYSCALL_DUP: usize = 23;
const SYSCALL_DUP3:usize = 24;
const SYSCALL_MKDIRAT: usize = 34;
const SYSCALL_UMOUNT2: usize = 39;
const SYSCALL_MOUNT: usize = 40;
const SYSCALL_CHDIR: usize = 49;
const SYSCALL_OPENAT: usize = 56;
const SYSCALL_CLOSE: usize = 57;
const SYSCALL_PIPE: usize = 59;
const SYSCALL_GETDENTS64: usize = 61;
const SYSCALL_READ: usize = 63;
const SYSCALL_WRITE: usize = 64;
const SYSCALL_FSTAT:usize = 80;
const SYSCALL_EXIT: usize = 93;
const SYSCALL_YIELD: usize = 124;
const SYSCALL_GET_TIME: usize = 169;
const SYSCALL_GETPID: usize = 172;
const SYSCALL_FORK: usize = 220;
const SYSCALL_EXEC: usize = 221;
const SYSCALL_WAITPID: usize = 260;

// Not standard POSIX sys_call
const SYSCALL_LS: usize = 500;

mod fs;
mod process;

use fs::*;
use process::*;

//use crate::fs::Dirent;

pub fn syscall(syscall_id: usize, args: [usize; 6]) -> isize {
    match syscall_id {
        SYSCALL_GETCWD=> sys_getcwd(args[0] as *mut u8, args[1] as usize),
        SYSCALL_DUP=> sys_dup(args[0]),
        SYSCALL_DUP3=> sys_dup3(args[0] as usize, args[1] as usize),
        SYSCALL_MKDIRAT=> sys_mkdir(args[0] as isize, args[1] as *const u8, args[2] as u32),
        SYSCALL_UMOUNT2=> sys_umount(args[0] as *const u8, args[1] as usize),
        SYSCALL_MOUNT=> sys_mount(args[0] as *const u8, args[1] as *const u8, args[2] as *const u8, args[3] as usize, args[4] as *const u8),
        SYSCALL_CHDIR=> sys_chdir(args[0] as *const u8),
        //SYSCALL_OPEN => sys_open(args[0] as *const u8, args[1] as u32),
        SYSCALL_OPENAT=> sys_open_at(args[0] as isize, args[1] as *const u8, args[2] as u32, args[3] as u32),
        SYSCALL_CLOSE => sys_close(args[0]),
        SYSCALL_PIPE => sys_pipe(args[0] as *mut usize),
        SYSCALL_GETDENTS64 => sys_getdents64(args[0] as isize, args[1] as *mut u8, args[2] as usize),
        SYSCALL_READ => sys_read(args[0], args[1] as *const u8, args[2]),
        SYSCALL_WRITE => sys_write(args[0], args[1] as *const u8, args[2]),
        SYSCALL_FSTAT=>sys_fstat(args[0] as isize, args[1] as *mut u8),
        SYSCALL_EXIT => sys_exit(args[0] as i32),
        SYSCALL_YIELD => sys_yield(),
        SYSCALL_GET_TIME => sys_get_time(),
        SYSCALL_GETPID => sys_getpid(),
        SYSCALL_FORK => sys_fork(),
        SYSCALL_EXEC => sys_exec(args[0] as *const u8, args[1] as *const usize),
        SYSCALL_WAITPID => sys_waitpid(args[0] as isize, args[1] as *mut i32),
        SYSCALL_LS => sys_ls(args[0] as *const u8),
        _ => panic!("Unsupported syscall_id: {}", syscall_id),
    }
}

