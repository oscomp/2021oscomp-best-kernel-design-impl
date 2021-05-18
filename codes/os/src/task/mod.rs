mod context;
mod switch;
mod task;
mod manager;
mod processor;
mod pid;

use crate::fs::{open, OpenFlags, DiskInodeType};
//use easy_fs::DiskInodeType;
use switch::__switch;
use task::{TaskControlBlock, TaskStatus};
use alloc::sync::Arc;
use alloc::vec;
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

pub fn add_initproc() {
    add_task(INITPROC.clone());
}
