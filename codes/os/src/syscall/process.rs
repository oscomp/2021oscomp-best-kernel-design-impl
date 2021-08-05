use crate::task::*;
use crate::timer::*;
use crate::mm::{
    UserBuffer,
    translated_str,
    translated_refmut,
    translated_ref,
    translated_byte_buffer, 
    print_free_pages,
    PageTable,
};
use crate::fs::{DiskInodeType, FileClass, FileDescripter, OpenFlags, open};
use crate::config::{PAGE_SIZE, CLOCK_FREQ};
use crate::gdb_print;
use crate::gdb_println;
use crate::monitor::*;
use alloc::sync::Arc;
use alloc::vec::Vec;
//use alloc::vec;
use alloc::string::String;
//use simple_fat32::println;
use core::mem::size_of;
use core::slice;
//use easy_fs::DiskInodeType;


pub fn sys_exit(exit_code: i32) -> ! {
    exit_current_and_run_next(exit_code);
    panic!("Unreachable in sys_exit!");
}

pub fn sys_yield() -> isize {
    suspend_current_and_run_next();
    0
}
pub fn sys_times(time: *mut i64) -> isize {
    // struct tms              
    // {                     
    //     long tms_utime;  
    //     long tms_stime;  
    //     long tms_cutime; 
    //     long tms_cstime; 
    // };
    let token = current_user_token();
    let sec = get_time_us() as i64;
    *translated_refmut(token, time) = sec;
    *translated_refmut(token, unsafe { time.add(1) }) = sec;
    *translated_refmut(token, unsafe { time.add(2) }) = sec;
    *translated_refmut(token, unsafe { time.add(3) }) = sec;
    0
}

pub fn sys_get_time_of_day(time: *mut u64) -> isize {
    // pub struct TimeVal{
    //     sec: u64,
    //     usec: u64,
    // }
    let token = current_user_token();
    let ticks = get_time();
    let sec = (ticks/CLOCK_FREQ) as u64;
    let usec = ((ticks%CLOCK_FREQ) * (USEC_PER_SEC / CLOCK_FREQ)) as u64;
    *translated_refmut(token, time) = sec ;
    *translated_refmut(token, unsafe { time.add(1) }) = usec;
    0

}


pub fn sys_getrusage(who: isize, usage: *mut u8) -> isize {
    if who != RUSAGE_SELF {
        panic!("sys_getrusage: \"who\" not supported!");
        return -1;
    }
    let task = current_task().unwrap();
    let token = current_user_token();
    let mut userbuf = UserBuffer::new(translated_byte_buffer(token, usage, core::mem::size_of::<RUsage>()));
    let rusage = &task.acquire_inner_lock().rusage;
    userbuf.write(rusage.as_bytes());
    gdb_println!(SYSCALL_ENABLE, "sys_getrusage(who: {}, usage: {:?}) = {}", who, rusage, 0);
    // crate::sbi::shutdown();
    0
}

pub fn sys_uname(buf: *mut u8) -> isize {
    // let uname = utsname {
    //     sysname: b"UltraOS\0",
    //     nodename:  b"UltraOS\0",
    //     release:  b"Alpha\0",
    //     version:  b"1.1\0",
    //     machine:  b"RISC-V64\0",
    //     domainname: b"UltraTEAM/UltraOS\0"
    // };
    let token = current_user_token();
    let mut buf_vec = translated_byte_buffer(token, buf, size_of::<utsname>());
    let uname = utsname::new();
    // 使用UserBuffer结构，以便于跨页读写
    let mut userbuf = UserBuffer::new(buf_vec);
    userbuf.write(uname.as_bytes());
    0
}

pub fn sys_sleep(time_req: *mut u64, time_remain: *mut u64) -> isize{
    // pub struct TimeVal{
    //     sec: u64,
    //     usec: u64,
    // }
    let token = current_user_token();
    let sec = *translated_ref(token, time_req) as usize;
    let usec = *translated_ref(token, unsafe{time_req.add(1)}) as usize;
    // println!("sys_sleep: sec:{} usec:{}", sec, usec);
    let (end_sec,end_usec) = sum_time(sec, usec, get_time_s(), get_time_us());
    loop{
        let cur_sec = get_time_s();
        let cur_usec = get_time_us();
        if compare_time(end_sec, end_usec, cur_sec, cur_usec) {
            print!("\n");
            suspend_current_and_run_next();
        }
        else{
            if time_remain as usize != 0{
                *translated_refmut(token, time_remain) = 0 as u64;
            }
            *translated_refmut(token, unsafe { time_remain.add(1) }) = 0 as u64;
            return 0;
        }
    }
    0
}


pub fn sys_clock_get_time(clk_id: usize, tp: *mut u64) -> isize{
    // struct timespec {
    //     time_t   tv_sec;        /* seconds */
    //     long     tv_nsec;       /* nanoseconds */
    // };
    if tp as usize == 0 {// point is null
        return 0;
    }
    
    let token = current_user_token();
    let ticks = get_time();
    let sec = (ticks/CLOCK_FREQ) as u64;
    let nsec = ((ticks%CLOCK_FREQ) * (NSEC_PER_SEC / CLOCK_FREQ))  as u64;
    *translated_refmut(token, tp) = sec ;
    *translated_refmut(token, unsafe { tp.add(1) }) = nsec;
    gdb_println!(SYSCALL_ENABLE, "sys_get_time(clk_id: {}, tp: (sec: {}, nsec: {}) = {}", clk_id, sec, nsec, 0);
    0
}

// @Arg: value: ITimerVal pointer
pub fn sys_getitimer(which: isize, curr_value: *mut u8) -> isize{
    // pub struct ITimerVal{
    //     it_interval: TimeVal, /* Interval for periodic timer */
    //     it_value: TimeVal,    /* Time until next expiration */
    // }
    // pub struct TimeVal{
    //     sec: usize,
    //     usec: usize,
    // }
    let token = current_user_token();
    if curr_value as usize != 0{
        let itimer = current_task().unwrap().acquire_inner_lock().itimer;
        let mut buf_vec = translated_byte_buffer(token, curr_value, size_of::<ITimerVal>());
        // 使用UserBuffer结构，以便于跨页读写
        let mut userbuf = UserBuffer::new(buf_vec);
        if itimer.is_zero(){
            userbuf.write(itimer.as_bytes());
        }
        else{
            let mut remaining_itimer = itimer;

        }
        0
    }
    else{
        -1
    }
}

// @Arg: value: ITimerVal pointer
pub fn sys_setitimer(which: isize, new_value: *mut usize, old_value: *mut usize) -> isize{
    // if old_value as usize != 0{
    //     old_value
    // }
    0
}



pub fn sys_set_tid_address(tidptr: usize) -> isize {
    current_task().unwrap().acquire_inner_lock().address.clear_child_tid = tidptr;
    // print!("sys_set_tid_address: return {}", sys_gettid());
    sys_gettid()
}

// For user, pid is tgid in kernel
pub fn sys_getpid() -> isize {
    current_task().unwrap().tgid as isize
}

// For user, pid is tgid in kernel
pub fn sys_getppid() -> isize {
    // let mut search_task_pid: usize = current_task().unwrap().pid.0;
    let mut search_task: Arc<TaskControlBlock> = current_task().unwrap();
    search_task = search_task.get_parent().unwrap();
    search_task.tgid as isize
    // while search_task_pid != 0 {
    //     search_task = search_task.get_parent().unwrap();
    //     search_task_pid = search_task.pid.0;
    // }
    
    // getppid(child_pid, &search_task) as isize
}

pub fn sys_getuid() -> isize {
    0 // root user
}

pub fn sys_geteuid() -> isize {
    0 // root user
}

pub fn sys_getgid() -> isize {
    0 // root group
}

pub fn sys_getegid() -> isize {
    0 // root group
}

// For user, tid is pid in kernel
pub fn sys_gettid() -> isize {
    current_task().unwrap().pid.0 as isize
}

pub fn sys_sbrk(grow_size: isize, is_shrink: usize) -> isize {
    let current_va = current_task().unwrap().grow_proc(grow_size) as isize;
    // gdb_println!(SYSCALL_ENABLE,"(sys_sbrk ret 0x{:X})",current_va);
    current_va
}

pub fn sys_brk(brk_addr: usize) -> isize{
    let mut addr_new = 0;
    if brk_addr == 0 {
        addr_new = sys_sbrk(0, 0) as usize;
    }
    else{
        let former_addr = current_task().unwrap().grow_proc(0);
        let grow_size: isize = (brk_addr - former_addr) as isize;
        addr_new = current_task().unwrap().grow_proc(grow_size);
    }
    gdb_println!(SYSCALL_ENABLE,"sys_brk(0x{:X}) = 0x{:X}", brk_addr, addr_new);
    addr_new as isize
}

//long clone(unsigned long flags, void *child_stack,
//    int *ptid, int *ctid,
//    unsigned long newtls);
pub fn sys_fork(flags: usize, stack_ptr: usize, ptid: usize, ctid: usize, newtls: usize) -> isize {
    print_free_pages();
    let current_task = current_task().unwrap();
    let new_task = current_task.fork(false);
    let tid = new_task.getpid();
    let flags = CloneFlags::from_bits(flags).unwrap();
    if flags.contains(CloneFlags::CLONE_CHILD_SETTID) && ctid != 0{
        new_task.acquire_inner_lock().address.set_child_tid = ctid; 
        *translated_refmut(new_task.acquire_inner_lock().get_user_token(), ctid as *mut i32) = tid  as i32;
    }
    if flags.contains(CloneFlags::CLONE_CHILD_CLEARTID) && ctid != 0{
        new_task.acquire_inner_lock().address.clear_child_tid = ctid;
    }
    if !flags.contains(CloneFlags::SIGCHLD){
        panic!("sys_fork: FLAG not supported!");
        return -1;
    }

    
    if stack_ptr != 0{
        let trap_cx = new_task.acquire_inner_lock().get_trap_cx();
        trap_cx.set_sp(stack_ptr);
    }
    let new_pid = new_task.pid.0;
    // modify trap context of new_task, because it returns immediately after switching
    let trap_cx = new_task.acquire_inner_lock().get_trap_cx();
    // we do not have to move to next instruction since we have done it before
    // for child process, fork returns 0
    trap_cx.x[10] = 0;
    // add new task to scheduler
    add_task(new_task);
    print_free_pages();
    gdb_print!(SYSCALL_ENABLE,"sys_fork(flags: {:?}, stack_ptr: 0x{:X}, ptid: {}, ctid: {}, newtls: {}) = {}", flags, stack_ptr, ptid, ctid, newtls, new_pid);
    new_pid as isize
}

pub fn sys_exec(path: *const u8, mut args: *const usize) -> isize {
    //println!("[sys_exec]");
    let token = current_user_token();
    // println!{"----------pin1"}
    let path = translated_str(token, path);
    // println!{"----------pin2"}
    let mut args_vec: Vec<String> = Vec::new();
    // println!{"----------pin3"}
    loop {
        let arg_str_ptr = *translated_ref(token, args);
        if arg_str_ptr == 0 {
            break;
        }
        args_vec.push(translated_str(token, arg_str_ptr as *const u8));
        // println!("arg{}: {}",0, args_vec[0]);
        unsafe { args = args.add(1); }
    }
    let task = current_task().unwrap();
    let mut inner = task.acquire_inner_lock();
    let args_vec_copy = args_vec.clone();
    
    if let Some(app_inode) = open(inner.current_path.as_str(),path.as_str(), OpenFlags::RDONLY, DiskInodeType::File) {
        let len = app_inode.get_size();
        gdb_println!(EXEC_ENABLE,"[exec] File size: {} bytes", len);
        let fd = inner.alloc_fd();
        inner.fd_table[fd] = Some( 
            FileDescripter::new(
                false, 
                FileClass::File(app_inode)
            )
        );
        drop(inner);
        // print!("[exec 0]");
        let elf_buf = task.kmmap(0, len, 0, 0, fd as isize, 0);
        // print!("[exec 1]");
        let argc = args_vec.len();
        unsafe{
            // print!("[exec 2]");
            let elf_ref = slice::from_raw_parts(elf_buf as *const u8, len);
            task.exec(elf_ref, args_vec);
            let inner = task.acquire_inner_lock();
        }
        // print!("[exec 3]");
        // print_free_pages();
        task.kmunmap(elf_buf, len);
        // drop fd
        let mut inner = task.acquire_inner_lock();
        inner.fd_table[fd].take();
        //println!("[sys_exec] finish");
        gdb_println!(SYSCALL_ENABLE, "sys_exec(path: {}, args: {:?}) = {}", path, args_vec_copy, argc);
        0 
    } else {
        -1
    }
}

pub fn sys_wait4(pid: isize, wstatus: *mut i32, option: isize) -> isize {
    if option != 0 {
        panic!{"Extended option not support yet..."};
    }

    loop {
        let task = current_task().unwrap();
        let mut inner = task.acquire_inner_lock();
        if inner.children
            .iter()
            .find(|p| {pid == -1 || pid as usize == p.getpid()})
            .is_none() {
            return -1;
            // ---- release current PCB lock
            }
        let waited = inner.children
            .iter()
            .enumerate()
            .find(|(_, p)| {
                // ++++ temporarily hold child PCB lock
                p.acquire_inner_lock().is_zombie() && (pid == -1 || pid as usize == p.getpid())
                // ++++ release child PCB lock
            });
        if let Some((idx,_)) = waited {
            let waited_child = inner.children.remove(idx);
            // confirm that child will be deallocated after being removed from children list
            assert_eq!(Arc::strong_count(&waited_child), 1);
            let found_pid = waited_child.getpid();
            // ++++ temporarily hold child lock
            let exit_code = waited_child.acquire_inner_lock().exit_code;
            let ret_status = exit_code << 8;
            if (wstatus as usize) != 0{
                *translated_refmut(inner.memory_set.token(), wstatus) = ret_status;
            }
            // println!("=============The pid being waited is {}===================", pid);
            // println!("=============The exit code of waiting_pid is {}===========", exit_code);
            return found_pid as isize;
        } else {
            drop(inner);
            suspend_current_and_run_next();
            continue;
        }
    }
}

/// If there is not a child process whose pid is same as given, return -1.
/// Else if there is a child process but it is still running, return -2.
pub fn sys_waitpid(pid: isize, exit_code_ptr: *mut i32) -> isize {
    let task = current_task().unwrap();
    // find a child process

    // ---- hold current PCB lock
    let mut inner = task.acquire_inner_lock();
    if inner.children
        .iter()
        .find(|p| {pid == -1 || pid as usize == p.getpid()})
        .is_none() {
        return -1;
        // ---- release current PCB lock
    }
    let pair = inner.children
        .iter()
        .enumerate()
        .find(|(_, p)| {
            // ++++ temporarily hold child PCB lock
            p.acquire_inner_lock().is_zombie() && (pid == -1 || pid as usize == p.getpid())
            // ++++ release child PCB lock
        });
    if let Some((idx, _)) = pair {
        let child = inner.children.remove(idx);
        // confirm that child will be deallocated after being removed from children list
        assert_eq!(Arc::strong_count(&child), 1);
        let found_pid = child.getpid();
        // ++++ temporarily hold child lock
        let exit_code = child.acquire_inner_lock().exit_code;
        // ++++ release child PCB lock
        *translated_refmut(inner.memory_set.token(), exit_code_ptr) = exit_code;
        found_pid as isize
    } else {
        -2
    }
    // ---- release current PCB lock automatically
}

// not support full flags: MAP_FIXED
// WARNING: if mmap len is 0, we will alloc one page for it, which actually should be forbidden.

pub fn sys_mmap(start: usize, len: usize, prot: usize, flags: usize, fd: isize, off: usize) -> isize {
    let task = current_task().unwrap();
    let mut adjust_len = len;
    if adjust_len == 0{
        adjust_len = PAGE_SIZE;
        //println!("[sys_mmap]:adjust_len = {}",adjust_len);
    }
    
    println!("[{}][insert_mmap_area]: len 0x{:X} start 0x{:X}", task.pid.0, start, len);
    let result_addr = task.mmap(start, adjust_len, prot, flags, fd, off);
    gdb_println!(SYSCALL_ENABLE,"sys_mmap(0x{:X},{},{},0x{:X},{},{}) = 0x{:X}",start, len, prot, flags, fd, off, result_addr);
    return result_addr as isize;
}

pub fn sys_munmap(start: usize, len: usize) -> isize {
    let task = current_task().unwrap();
    task.munmap(start, len)
}

pub fn sys_mprotect(addr: usize, len: usize, prot: isize) -> isize{
    if (addr % PAGE_SIZE != 0) || (len % PAGE_SIZE != 0){ // Not align
        println!("sys_mprotect: not align");
        return -1
    }
    let task = current_task().unwrap();
    // let token = task.acquire_inner_lock().get_user_token();
    let memory_set = &mut task.acquire_inner_lock().memory_set;
    let start_vpn = addr / PAGE_SIZE;
    for i in 0..(len/PAGE_SIZE){
        // here (prot << 1) is identical to BitFlags of X/W/R in pte flags
        if memory_set.set_pte_flags(start_vpn.into(), (prot as usize)<<1) == -1{
            // if fail
            panic!("sys_mprotect: No such pte");
        }
    }
    0
}