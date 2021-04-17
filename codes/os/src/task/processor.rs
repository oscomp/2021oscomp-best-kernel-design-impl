#![feature(llvm_asm)]

use super::TaskControlBlock;
use alloc::sync::Arc;
use core::{borrow::Borrow, cell::RefCell};
use lazy_static::*;
use super::{fetch_task, TaskStatus};
use super::__switch;
use crate::trap::TrapContext;

pub fn get_core_id() -> usize{
    let tp:usize;
    unsafe {
        llvm_asm!("mv $0, tp" : "=r"(tp));
    }
    tp
}

pub struct Processor {
    inner: RefCell<ProcessorInner>,
}

unsafe impl Sync for Processor {}

struct ProcessorInner {
    current: Option<Arc<TaskControlBlock>>,
    idle_task_cx_ptr: usize,
}

impl Processor {
    pub fn new() -> Self {
        Self {
            inner: RefCell::new(ProcessorInner {
                current: None,
                idle_task_cx_ptr: 0,
            }),
        }
    }
    fn get_idle_task_cx_ptr2(&self) -> *const usize {
        let inner = self.inner.borrow();
        &inner.idle_task_cx_ptr as *const usize
    }
    pub fn run(&self) {
        let core = get_core_id();
        // println!("core:{} run tasks",core);
        // if core == 1{
        //     loop{}
        // }
        loop {
            if let Some(task) = fetch_task() {
                // println!("core:{} exec tasks",get_core_id());
                let idle_task_cx_ptr2 = self.get_idle_task_cx_ptr2();
                // acquire
                let mut task_inner = task.acquire_inner_lock();
                let next_task_cx_ptr2 = task_inner.get_task_cx_ptr2();
                task_inner.task_status = TaskStatus::Running;
                drop(task_inner);
                // release
                self.inner.borrow_mut().current = Some(task);
                // println!("core:{} switch tasks",get_core_id());
                unsafe {
                    __switch(
                        idle_task_cx_ptr2,
                        next_task_cx_ptr2,
                    );
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
    let core_id: usize = get_core_id();
    let task = current_task().unwrap();
    let token = task.acquire_inner_lock().get_user_token();
    token
}

pub fn current_trap_cx() -> &'static mut TrapContext {
    current_task().unwrap().acquire_inner_lock().get_trap_cx()
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
