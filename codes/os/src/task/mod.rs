mod context;
mod switch;
mod task;
mod manager;
mod processor;
mod pid;
mod info;

use crate::fs::{open, OpenFlags, DiskInodeType, File};
use crate::mm::UserBuffer;
//use easy_fs::DiskInodeType;
use switch::__switch;
pub use task::{TaskControlBlock, TaskStatus};
pub use info::*;
use alloc::sync::Arc;
use alloc::vec::Vec;
use manager::fetch_task;
use lazy_static::*;
pub use context::TaskContext;

pub use processor::{
    run_tasks,
    current_task,
    current_user_token,
    current_trap_cx,
    take_current_task,
    schedule,
    get_core_id,
    print_core_info
};
pub use manager::add_task;
pub use pid::{PidHandle, pid_alloc, KernelStack};

pub fn suspend_current_and_run_next() {
    // There must be an application running.
    let task = current_task().unwrap();

    // ---- hold current PCB lock
    let mut task_inner = task.acquire_inner_lock();
    let task_cx_ptr2 = task_inner.get_task_cx_ptr2();
    drop(task_inner);
    
    // jump to scheduling cycle
    // push back to ready queue while scheduling
    schedule(task_cx_ptr2);
}

pub fn exit_current_and_run_next(exit_code: i32) {
    // Forbid more than one process exit
    let mut initproc_inner = INITPROC.acquire_inner_lock();
    // take from Processor
    let task = take_current_task().unwrap();
    // **** hold current PCB lock
    let mut inner = task.acquire_inner_lock();
    // Change status to Zombie
    inner.task_status = TaskStatus::Zombie;
    inner.exit_code = exit_code;
    for child in inner.children.iter() {
        child.acquire_inner_lock().parent = Some(Arc::downgrade(&INITPROC));
        initproc_inner.children.push(child.clone());
    }

    inner.children.clear();
    // deallocate user space
    inner.memory_set.recycle_data_pages();
    // drop task manually to maintain rc correctly
    drop(inner);
    drop(task);
    drop(initproc_inner);
    // we do not have to save task context
    let _unused: usize = 0;
    schedule(&_unused as *const _);
}

lazy_static! {
    pub static ref INITPROC: Arc<TaskControlBlock> = Arc::new({
        let inode = open("/","initproc", OpenFlags::RDONLY, DiskInodeType::File).unwrap();
        let v = inode.read_all();
        TaskControlBlock::new(v.as_slice())
    });
}

pub fn add_initproc_into_fs() {
    extern "C" { fn _num_app(); }
    extern "C" { fn _app_names(); }
    let mut num_app_ptr = _num_app as usize as *mut usize;
    let start = _app_names as usize as *const u8;
    let mut app_start = unsafe {
        core::slice::from_raw_parts_mut(num_app_ptr.add(1), 3)
    };

    open(
        "/",
        "mnt",
        OpenFlags::CREATE,
        DiskInodeType::Directory
    );

    // find if there already exits 
    println!("Find if there already exits ");
    if let Some(inode) = open(
        "/",
        "initproc",
        OpenFlags::RDONLY,
        DiskInodeType::File
    ){
        println!("Already have init proc in FS");
        return;
    }


    // println!("Write apps(initproc & user_shell) to disk from mem ");

    //Write apps(initproc & user_shell) to disk from mem
    if let Some(inode) = open(
        "/",
        "initproc",
        OpenFlags::CREATE,
        DiskInodeType::File
    ){
        println!("Create initproc ");
        let mut data: Vec<&'static mut [u8]> = Vec::new();
        data.push( unsafe{
        core::slice::from_raw_parts_mut(
            app_start[0] as *mut u8,
            app_start[1] - app_start[0]
        )}) ;
        println!("Start write initproc ");
        inode.write(UserBuffer::new(data));
        println!("Init_proc OK");
    }
    else{
        panic!("initproc create fail!");
    }

    if let Some(inode) = open(
        "/",
        "user_shell",
        OpenFlags::CREATE,
        DiskInodeType::File
    ){
        println!("Create user_shell ");
        let mut data:Vec<&'static mut [u8]> = Vec::new();
        data.push(unsafe{
        core::slice::from_raw_parts_mut(
            app_start[1] as *mut u8,
            app_start[2] - app_start[1]
        )});
        println!("Start write user_shell ");
        inode.write(UserBuffer::new(data));
        println!("User_shell OK");
    }
    else{
        panic!("user_shell create fail!");
    }
    println!("Write apps(initproc & user_shell) to disk from mem");
}

pub fn add_initproc() {
    add_initproc_into_fs();
    add_task(INITPROC.clone());
}
