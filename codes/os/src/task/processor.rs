// #![feature(llvm_asm)]
// #[macro_use]
use super::{TaskControlBlock, RUsage};
use alloc::sync::Arc;
use core::{borrow::Borrow, cell::RefCell};
use lazy_static::*;
use super::{fetch_task, TaskStatus, Signals, SIG_DFL};
use super::__switch;
use crate::timer::get_time_us;
use crate::trap::TrapContext;
use crate::task::manager::add_task;
use crate::gdb_print;
use crate::monitor::*;

pub fn get_core_id() -> usize{
    let tp:usize;
    unsafe {
        llvm_asm!("mv $0, tp" : "=r"(tp));
    }
    // tp
    0
}

pub struct Processor {
    inner: RefCell<ProcessorInner>,
}

unsafe impl Sync for Processor {}

struct ProcessorInner {
    current: Option<Arc<TaskControlBlock>>,
    idle_task_cx_ptr: usize,
    user_clock: usize,  /* Timer usec when last enter into the user program */
    kernel_clock: usize, /* Timer usec when user program traps into the kernel*/
}

impl Processor {
    pub fn new() -> Self {
        Self {
            inner: RefCell::new(ProcessorInner {
                current: None,
                idle_task_cx_ptr: 0,
                user_clock: 0,  
                kernel_clock: 0,
            }),
        }
    }

    // when trap return to user program, use this func to update user clock
    pub fn update_user_clock(& self){
        self.inner.borrow_mut().user_clock = get_time_us();
    }
    
    // when trap into kernel, use this func to update kernel clock
    pub fn update_kernel_clock(& self){
        self.inner.borrow_mut().kernel_clock = get_time_us();
    }

    pub fn get_user_clock(& self) -> usize{
        return self.inner.borrow().user_clock;
    }

    pub fn get_kernel_clock(& self) -> usize{
        return self.inner.borrow().kernel_clock;
    }

    fn get_idle_task_cx_ptr2(&self) -> *const usize {
        let inner = self.inner.borrow();
        &inner.idle_task_cx_ptr as *const usize
    }
    pub fn run(&self) {
        loop{
            // True: Not first time to fetch a task 
            if let Some(current_task) = take_current_task(){
                gdb_print!(PROCESSOR_ENABLE,"[hart {} run:pid{}]", get_core_id(), current_task.pid.0);
                let mut current_task_inner = current_task.acquire_inner_lock();
                //println!("get lock");
                let task_cx_ptr2 = current_task_inner.get_task_cx_ptr2();
                let idle_task_cx_ptr2 = self.get_idle_task_cx_ptr2();
                // True: switch
                // False: return to current task, don't switch
                if let Some(task) = fetch_task() {
                    let mut task_inner = task.acquire_inner_lock();
                    task_inner.memory_set.activate();// change satp
                    let next_task_cx_ptr2 = task_inner.get_task_cx_ptr2();
                    task_inner.task_status = TaskStatus::Running;
                    drop(task_inner);
                    // release
                    self.inner.borrow_mut().current = Some(task);
                    ////////// current task  /////////
                    // update RUsage of process
                    // let ru_stime = get_kernel_runtime_usec();
                    // update_kernel_clock();
                    // current_task_inner.rusage.add_stime(ru_stime);

                    // Change status to Ready
                    current_task_inner.task_status = TaskStatus::Ready;
                    drop(current_task_inner);
                    add_task(current_task);
                    ////////// current task  /////////
                    unsafe {
                        __switch(
                            idle_task_cx_ptr2,
                            next_task_cx_ptr2,
                        );
                    }
                }
                else{
                    drop(current_task_inner);
                    self.inner.borrow_mut().current = Some(current_task);
                    unsafe {
                        __switch(
                            idle_task_cx_ptr2,
                            task_cx_ptr2,
                        );
                    }
                }
            // False: First time to fetch a task
            } else {
                // Keep fetching
                gdb_print!(PROCESSOR_ENABLE,"[run:no current task]");
                if let Some(task) = fetch_task() {
                    // acquire
                    let idle_task_cx_ptr2 = self.get_idle_task_cx_ptr2();
                    let mut task_inner = task.acquire_inner_lock();
                    let next_task_cx_ptr2 = task_inner.get_task_cx_ptr2();
                    task_inner.task_status = TaskStatus::Running;
                    task_inner.memory_set.activate();// change satp
                    // release
                    drop(task_inner);
                    self.inner.borrow_mut().current = Some(task);
                    unsafe {
                        __switch(
                            idle_task_cx_ptr2,
                            next_task_cx_ptr2,
                        );
                    }
                }
            }
        }
    }
    pub fn take_current(&self) -> Option<Arc<TaskControlBlock>> {
        self.inner.borrow_mut().current.take()
    }
    pub fn current(&self) -> Option<Arc<TaskControlBlock>> {
        self.inner.borrow().current.as_ref().map(|task| Arc::clone(task))
    }
}

lazy_static! {
    pub static ref PROCESSOR_LIST: [Processor; 2] = [Processor::new(),Processor::new()];
}

pub fn run_tasks() {
    let core_id: usize = get_core_id();
    PROCESSOR_LIST[core_id].run();
}

pub fn take_current_task() -> Option<Arc<TaskControlBlock>> {
    let core_id: usize = get_core_id();
    PROCESSOR_LIST[core_id].take_current()
}

pub fn current_task() -> Option<Arc<TaskControlBlock>> {
    let core_id: usize = get_core_id();
    PROCESSOR_LIST[core_id].current()
}

pub fn current_user_token() -> usize {
    // let core_id: usize = get_core_id();
    let task = current_task().unwrap();
    let token = task.acquire_inner_lock().get_user_token();
    token
}

pub fn current_trap_cx() -> &'static mut TrapContext {
    current_task().unwrap().acquire_inner_lock().get_trap_cx()
}

pub fn print_core_info(){
    println!( "[core{}] pid = {}", 0, PROCESSOR_LIST[0].current().unwrap().getpid() );
    println!( "[core{}] pid = {}", 1, PROCESSOR_LIST[1].current().unwrap().getpid() );
}

// when trap return to user program, use this func to update user clock
pub fn update_user_clock(){
    let core_id: usize = get_core_id();
    PROCESSOR_LIST[core_id].update_user_clock();
}

// when trap into kernel, use this func to update kernel clock
pub fn update_kernel_clock(){
    let core_id: usize = get_core_id();
    PROCESSOR_LIST[core_id].update_kernel_clock();
}

// when trap into kernel, use this func to get time spent in user (it is duration not accurate time)
pub fn get_user_runtime_usec() -> usize{
    let core_id: usize = get_core_id();
    return get_time_us() - PROCESSOR_LIST[core_id].get_user_clock();
}

// when trap return to user program, use this func to get time spent in kernel (it is duration not accurate time)
pub fn get_kernel_runtime_usec() -> usize{
    let core_id: usize = get_core_id();
    return get_time_us() - PROCESSOR_LIST[core_id].get_kernel_clock();
}


pub fn schedule(switched_task_cx_ptr2: *const usize) {
    let core_id: usize = get_core_id();
    let idle_task_cx_ptr2 = PROCESSOR_LIST[core_id].get_idle_task_cx_ptr2();
    unsafe {
        __switch(
            switched_task_cx_ptr2,
            idle_task_cx_ptr2,
        );
    }
}
