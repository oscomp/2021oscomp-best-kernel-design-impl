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
const SYSCALL_LSEEK: usize = 62;
const SYSCALL_READ: usize = 63;
const SYSCALL_WRITE: usize = 64;
const SYSCALL_WRITEV: usize = 66;
const SYSCALL_SENDFILE: usize = 71;
const SYSCALL_PSELECT6: usize = 72;
const SYSCALL_READLINKAT: usize = 78;
const SYSCALL_NEW_FSTATAT: usize = 79;
const SYSCALL_FSTAT:usize = 80;
const SYSCALL_FSYNC:usize = 82;
const SYSCALL_UTIMENSAT:usize = 88;
const SYSCALL_EXIT: usize = 93;
const SYSCALL_EXIT_GRUOP: usize = 94;
const SYSCALL_SET_TID_ADDRESS: usize = 96;
const SYSCALL_NANOSLEEP: usize = 101;
const SYSCALL_GETITIMER: usize = 102;
const SYSCALL_SETITIMER: usize = 103;
const SYSCALL_CLOCK_GETTIME: usize = 113;
const SYSCALL_YIELD: usize = 124;
const SYSCALL_KILL: usize = 129;
const SYSCALL_SIGACTION: usize = 134;
const SYSCALL_SIGRETURN: usize = 139;
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
const SYSCALL_PRLIMIT: usize = 261;
const SYSCALL_RENAMEAT2: usize = 276;

// Not standard POSIX sys_call
const SYSCALL_LS: usize = 500;
const SYSCALL_SHUTDOWN: usize = 501;
const SYSCALL_CLEAR: usize = 502;

mod fs;
mod process;

pub use fs::*;
use process::*;
use crate::gdb_print;
use crate::monitor::*;
use crate::sbi::shutdown;
use crate::timer::get_timeval;

//use crate::fs::Dirent;

pub fn test() {
    if sys_getpid() == 1{
        let start = get_timeval();
        // println!("test: run sys_getppid 1000000 times, start {:?}",start);
        for _ in 0..1000000{
            syscall(SYSCALL_GETPPID,[0,0,0,0,0,0]);
            // unsafe{
            //     asm!(
            //         "sfence.vma",
            //     );
            // }
        }
        let end = get_timeval();
        // println!("test: run sys_getppid 1000000 times, end {:?}",end);
        println!("test: run sys_getppid + sfence.vma 1000000 times, spent {:?}",end-start);
    }
}

pub fn syscall(syscall_id: usize, args: [usize; 6]) -> isize {
    gdb_print!(SYSCALL_ENABLE,"syscall-({}) arg0 = {}, arg1 = {}\n",syscall_id, args[0] as isize, args[1] as isize);
    //if syscall_id != 64 && syscall_id != 63 && syscall_id != 61 {
    //    gdb_print!(SYSCALL_ENABLE,"syscall-({}) arg0 = {}, arg1 = {}\n",syscall_id, args[0] as isize, args[1] as isize);
    //    //println!("syscallid-{}", syscall_id);
    //} else {
    //    if args[0] != 0 && args[0] != 1 && args[0] != 2{
    //        // gdb_print!(SYSCALL_ENABLE,"syscall-({}) arg0 = {}, arg1 = {}\n",syscall_id, args[0] as isize, args[1] as isize);
    //    }
    //}
    
    // if sys_getpid() == 1{
    //     test();
    // }

    
    match syscall_id {
        SYSCALL_SIGRETURN => sys_sigreturn(),
        SYSCALL_GETCWD=> sys_getcwd(args[0] as *mut u8, args[1] as usize),
        SYSCALL_DUP=> sys_dup(args[0]),
        SYSCALL_DUP3=> sys_dup3(args[0] as usize, args[1] as usize),
        
        SYSCALL_FCNTL=> fcntl(args[0], args[1] as u32, args[3]),
        SYSCALL_IOCTL=> sys_ioctl(args[0], args[1] as u32, args[2]),
        SYSCALL_MKDIRAT=> sys_mkdir(args[0] as isize, args[1] as *const u8, args[2] as u32),
        //SYSCALL_UNLINKAT=> sys_unlinkat(args[0] as i32, args[1] as *const u8, args[2] as i32,args[3] as *const u8,args[4] as u32),
        SYSCALL_UNLINKAT=> sys_unlinkat(args[0] as i32, args[1] as *const u8, args[2] as u32),
        SYSCALL_GETITIMER => sys_getitimer(args[0] as isize, args[1] as *mut u8),
        SYSCALL_SETITIMER => sys_setitimer(args[0] as isize, args[1] as *mut usize, args[2] as *mut u8),
        SYSCALL_UMOUNT2=> sys_umount(args[0] as *const u8, args[1] as usize),
        SYSCALL_MOUNT=> sys_mount(args[0] as *const u8, args[1] as *const u8, args[2] as *const u8, args[3] as usize, args[4] as *const u8),
        /* faccessat is fake */
        SYSCALL_FACCESSAT => sys_utimensat(args[0], args[1] as *const u8, args[2], 0),
        SYSCALL_CHDIR=> sys_chdir(args[0] as *const u8),
        //SYSCALL_OPEN => sys_open(args[0] as *const u8, args[1] as u32),
        SYSCALL_OPENAT=> sys_open_at(args[0] as isize, args[1] as *const u8, args[2] as u32, args[3] as u32),
        SYSCALL_CLOSE => sys_close(args[0]),
        SYSCALL_PIPE => sys_pipe(args[0] as *mut u32, args[1] as usize),
        
        SYSCALL_GETDENTS64 => sys_getdents64(args[0] as isize, args[1] as *mut u8, args[2] as usize),
        SYSCALL_LSEEK=> sys_lseek(args[0] as usize, args[1] as isize, args[2] as i32),
        SYSCALL_READ => sys_read(args[0], args[1] as *const u8, args[2]),
        SYSCALL_WRITE => sys_write(args[0], args[1] as *const u8, args[2]),
        SYSCALL_WRITEV => sys_writev(args[0], args[1], args[2]),
        SYSCALL_SENDFILE=> sys_sendfile(args[0] as isize, args[1] as isize, args[2] as *mut usize, args[3] as usize),
        SYSCALL_PSELECT6=> {
            unsafe {
                //llvm_asm!("sfence.vma" :::: "volatile");
            }
            sys_pselect(
            args[0] as usize, args[1] as *mut u8, 
            args[2] as *mut u8, args[3] as *mut u8, 
            args[4] as *mut usize
        )},
        SYSCALL_READLINKAT=> sys_readlinkat(args[0] as isize, args[1] as *const u8, args[2] as *mut u8, args[3] as usize),
        SYSCALL_NEW_FSTATAT => sys_newfstatat(args[0] as isize, args[1] as *const u8, args[2] as *mut u8, args[3] as u32),
        SYSCALL_FSTAT=> sys_fstat(args[0] as isize, args[1] as *mut u8),
        SYSCALL_FSYNC=> 0,//panic!("not support fsync yet"),
        SYSCALL_UTIMENSAT => sys_utimensat(args[0], args[1] as *const u8, args[2], args[3] as u32),

        SYSCALL_SET_TID_ADDRESS => sys_set_tid_address(args[0] as usize),
        SYSCALL_EXIT => {
            unsafe {
                //llvm_asm!("sfence.vma" :::: "volatile");
            }
            sys_exit(args[0] as i32)
        },
        SYSCALL_EXIT_GRUOP => {
            unsafe {
                //llvm_asm!("sfence.vma" :::: "volatile");
            }
            sys_exit(args[0] as i32)
        },
        SYSCALL_NANOSLEEP => sys_sleep(args[0] as *mut u64, args[1] as *mut u64),
        SYSCALL_CLOCK_GETTIME => {
            unsafe {
                //llvm_asm!("sfence.vma" :::: "volatile");
            }
            sys_clock_get_time(args[0] as usize, args[1] as *mut u64)
        },
        SYSCALL_YIELD => sys_yield(),
        SYSCALL_KILL => sys_kill(args[0] as isize, args[1] as isize),
        SYSCALL_SIGACTION => sys_sigaction(args[0] as isize, args[1] as *mut usize, args[2] as *mut usize),
        SYSCALL_TIMES => sys_times(args[0] as *mut i64),
        SYSCALL_UNAME => sys_uname(args[0] as *mut u8),
        // SYSCALL_GETRUSAGE => sys_getrusage(args[0] as isize, args[1] as *mut u8),
        SYSCALL_GET_TIME_OF_DAY => sys_get_time_of_day(args[0] as *mut u64),
        SYSCALL_SBRK => sys_sbrk(args[0] as isize, args[1] as usize),
        SYSCALL_BRK => sys_brk(args[0]),

        SYSCALL_GETPID => sys_getpid(),
        SYSCALL_GETPPID => sys_getppid(),
        SYSCALL_GETUID => sys_getuid(),
        SYSCALL_GETEUID => sys_geteuid(),
        SYSCALL_GETGID => sys_getgid(),
        SYSCALL_GETEGID => sys_getegid(),
        SYSCALL_GETTID => sys_gettid(),

        SYSCALL_CLONE => {
            unsafe {
                //llvm_asm!("sfence.vma" :::: "volatile");
            }
            sys_fork(args[0] as usize, args[1] as  usize, args[2] as  usize, args[3] as  usize, args[4] as usize)
        },
        SYSCALL_EXEC => {
            unsafe {
                //llvm_asm!("sfence.vma" :::: "volatile");
            }
            sys_exec(args[0] as *const u8, args[1] as *const usize)
        },
        SYSCALL_WAIT4 => {
            unsafe {
                //llvm_asm!("sfence.vma" :::: "volatile");
            }
            sys_wait4(args[0] as isize, args[1] as *mut i32, args[2] as isize)
        },
        SYSCALL_PRLIMIT => 0,
        
        SYSCALL_RENAMEAT2 => sys_renameat2(
            args[0] as isize, args[1] as *const u8,
            args[2] as isize, args[3] as *const u8, args[4] as u32
        ),
        
        // SYSCALL_WAITPID => sys_waitpid(args[0] as isize, args[1] as *mut i32),
        SYSCALL_MMAP => {
            unsafe {
                //llvm_asm!("sfence.vma" :::: "volatile");
            }
            sys_mmap(args[0] as usize, args[1] as usize, args[2] as usize, 
            args[3] as usize, args[4] as isize, args[5] as usize)
        },
        SYSCALL_MUNMAP => { 
            unsafe {
                //llvm_asm!("sfence.vma" :::: "volatile");
            }
            sys_munmap(args[0] as usize, args[1] as usize) 
        },
        SYSCALL_MPROTECT => {sys_mprotect(args[0] as usize, args[1] as usize, args[2] as isize)},
        SYSCALL_LS => sys_ls(args[0] as *const u8),
        SYSCALL_SHUTDOWN => shutdown(),
        SYSCALL_CLEAR => sys_clear(args[0] as *const u8),
        _ => 0
        //_ => {println!("Unsupported syscall_id:{}, arg0={} arg1={}", syscall_id, args[0], args[1]); 0}
        //_ => panic!("Unsupported syscall_id: {}", syscall_id),

    }
}


