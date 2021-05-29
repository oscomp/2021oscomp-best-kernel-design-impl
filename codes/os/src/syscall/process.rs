use crate::task::{
    suspend_current_and_run_next,
    exit_current_and_run_next,
    current_task,
    current_user_token,
    add_task,
    TaskControlBlock,
    utsname,
    SIGCHILD
};
use crate::timer::*;
use crate::mm::{
    UserBuffer,
    translated_str,
    translated_refmut,
    translated_ref,
    translated_byte_buffer,
};
use crate::fs::{
    open,
    //find_par_inode_id,
    OpenFlags,
    DiskInodeType
};
use alloc::sync::Arc;
use alloc::vec::Vec;
//use alloc::vec;
use alloc::string::String;
use core::mem::size_of;
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

pub fn sys_get_time(time: *mut u64) -> isize {
    // pub struct TimeVal{
    //     sec: u64,
    //     usec: u64,
    // }
    let token = current_user_token();
    let sec = get_time_us() as u64;
    *translated_refmut(token, time) = get_time_s() as u64;
    *translated_refmut(token, unsafe { time.add(1) }) = get_time_us() as u64;
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
            suspend_current_and_run_next();
        }
        else{
            *translated_refmut(token, time_remain) = 0 as u64;
            *translated_refmut(token, unsafe { time_remain.add(1) }) = 0 as u64;
            return 0;
        }
    }
    0
}

pub fn sys_getpid() -> isize {
    current_task().unwrap().pid.0 as isize
}

pub fn sys_getppid() -> isize {
    // let mut search_task_pid: usize = current_task().unwrap().pid.0;
    let mut search_task: Arc<TaskControlBlock> = current_task().unwrap();
    search_task = search_task.get_parent().unwrap();
    search_task.pid.0 as isize
    // while search_task_pid != 0 {
    //     search_task = search_task.get_parent().unwrap();
    //     search_task_pid = search_task.pid.0;
    // }
    
    // getppid(child_pid, &search_task) as isize
}


pub fn sys_sbrk(grow_size: isize, is_shrink: usize) -> isize {
    current_task().unwrap().grow_proc(grow_size) as isize
}

pub fn sys_brk(brk_addr: usize) -> isize{
    if brk_addr == 0 {
        return sys_sbrk(0, 0) as isize
    }
    let former_addr = current_task().unwrap().grow_proc(0);
    let grow_size: isize = (brk_addr - former_addr) as isize;
    current_task().unwrap().grow_proc(grow_size);
    0
}

pub fn sys_fork(flags: usize, stack_ptr: usize) -> isize {
    if flags != SIGCHILD{
        return -1;
    }
    
    let current_task = current_task().unwrap();
    let new_task = current_task.fork();
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
    new_pid as isize
}

pub fn sys_exec(path: *const u8, mut args: *const usize) -> isize {
    let token = current_user_token();
    let path = translated_str(token, path);
    let mut args_vec: Vec<String> = Vec::new();
    loop {
        let arg_str_ptr = *translated_ref(token, args);
        if arg_str_ptr == 0 {
            break;
        }
        args_vec.push(translated_str(token, arg_str_ptr as *const u8));
        unsafe { args = args.add(1); }
    }
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    //println!("try get app {}{}", inner.current_path.as_str(), path);
    if let Some(app_inode) = open(inner.current_path.as_str(),path.as_str(), OpenFlags::RDONLY, DiskInodeType::File) {
        drop(inner);
        //let par_inode_id:u32 = find_par_inode_id(path.as_str());
        let all_data = app_inode.read_all();
        //let task = current_task().unwrap();
        let argc = args_vec.len();
        task.exec(all_data.as_slice(), args_vec);
        // return argc because cx.x[10] will be covered with it later
        argc as isize
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
        // println!{"acquiring lock......"};
        let waited = inner.children
            .iter()
            .enumerate()
            .find(|(_, p)| {
                // ++++ temporarily hold child PCB lock
                p.acquire_inner_lock().is_zombie() && (pid == -1 || pid as usize == p.getpid())
                // ++++ release child PCB lock
            });
        // println!{"drop lock......"};
        if let Some((idx,_)) = waited {
            let waited_child = inner.children.remove(idx);
            // confirm that child will be deallocated after being removed from children list
            assert_eq!(Arc::strong_count(&waited_child), 1);
            let found_pid = waited_child.getpid();
            // ++++ temporarily hold child lock
            // println!{"acquiring lock......"};
            let exit_code = waited_child.acquire_inner_lock().exit_code;
            // println!{"drop lock......"};
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

pub fn sys_mmap(start: usize, len: usize, prot: usize, flags: usize, fd: usize, off: usize) -> isize {
    let task = current_task().unwrap();
    task.mmap(start, len, prot, flags, fd, off) as isize
}

pub fn sys_munmap(start: usize, len: usize) -> isize {
    let task = current_task().unwrap();
    task.munmap(start, len)
}