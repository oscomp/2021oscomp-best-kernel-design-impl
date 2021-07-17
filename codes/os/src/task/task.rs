use core::ops::Index;

use crate::{console::print, mm::{
    MemorySet,
    PhysPageNum,
    KERNEL_SPACE, 
    KERNEL_MMAP_AREA, 
    //KERNEL_TOKEN,
    //PageTable,
    VirtAddr,
    translated_refmut,
    MmapArea,
    MapPermission,

}};
use crate::trap::{TrapContext, trap_handler};
use crate::config::{TRAP_CONTEXT, USER_HEAP_SIZE, MMAP_BASE};
use crate::gdb_println;
use crate::monitor::*;
use super::TaskContext;
use super::{PidHandle, pid_alloc, KernelStack};
use alloc::sync::{Weak, Arc};
use alloc::vec;
use alloc::vec::Vec;
use alloc::string::String;
use spin::{Mutex, MutexGuard};
use crate::fs::{ FileDiscripter, Stdin, Stdout, FileClass};

pub struct ProcAddress {
    pub set_child_tid: usize,
    pub clear_child_tid: usize,
}

pub struct TaskControlBlock {
    // immutable
    pub pid: PidHandle,
    pub tgid: usize,
    pub kernel_stack: KernelStack,
    // mutable
    inner: Mutex<TaskControlBlockInner>,
}

pub type FdTable =  Vec<Option<FileDiscripter>>;
pub struct TaskControlBlockInner {
    pub address: ProcAddress,
    pub trap_cx_ppn: PhysPageNum,
    pub base_size: usize,
    pub heap_start: usize,
    pub heap_pt: usize,
    pub task_cx_ptr: usize,
    pub task_status: TaskStatus,
    pub memory_set: MemorySet,
    pub parent: Option<Weak<TaskControlBlock>>,
    pub children: Vec<Arc<TaskControlBlock>>,
    pub exit_code: i32,
    pub fd_table: FdTable,
    pub current_path: String,
    pub mmap_area: MmapArea,
}

impl ProcAddress {
    pub fn new() -> Self{
        Self{set_child_tid: 0, clear_child_tid: 0}
    }
}

impl TaskControlBlockInner {
    pub fn get_task_cx_ptr2(&self) -> *const usize {
        &self.task_cx_ptr as *const usize
    }
    pub fn get_trap_cx(&self) -> &'static mut TrapContext {
        self.trap_cx_ppn.get_mut()
    }
    pub fn get_user_token(&self) -> usize {
        self.memory_set.token()
    }
    fn get_status(&self) -> TaskStatus {
        self.task_status
    }
    pub fn is_zombie(&self) -> bool {
        self.get_status() == TaskStatus::Zombie
    }
    pub fn alloc_fd(&mut self) -> usize {
        if let Some(fd) = (0..self.fd_table.len())
            .find(|fd| self.fd_table[*fd].is_none()) {
            fd
        } else {
            self.fd_table.push(None);
            self.fd_table.len() - 1
        }
    }
    pub fn print_cx(&self) {
        unsafe{ 
            println!("task_cx = {:?}", *(self.task_cx_ptr as *const TaskContext) );
            println!("trap_cx = {:?}", *(self.trap_cx_ppn.0 as *const TrapContext) );
        }
    }

    pub fn get_work_path(&self)->String{
        self.current_path.clone()
    }
}



impl TaskControlBlock {
    pub fn acquire_inner_lock(&self) -> MutexGuard<TaskControlBlockInner> {
        self.inner.lock()
    }
    pub fn new(elf_data: &[u8]) -> Self {
        // memory_set with elf program headers/trampoline/trap context/user stack
        let (memory_set, user_sp, user_heap, entry_point) = MemorySet::from_elf(elf_data);
        let trap_cx_ppn = memory_set
            .translate(VirtAddr::from(TRAP_CONTEXT).into())
            .unwrap()
            .ppn();
        // alloc a pid and a kernel stack in kernel space
        let pid_handle = pid_alloc();
        let tgid = pid_handle.0;
        let kernel_stack = KernelStack::new(&pid_handle);
        let kernel_stack_top = kernel_stack.get_top();
        // push a task context which goes to trap_return to the top of kernel stack
        let task_cx_ptr = kernel_stack.push_on_top(TaskContext::goto_trap_return());
        let task_control_block = Self {
            pid: pid_handle,
            tgid,
            kernel_stack,
            inner: Mutex::new(TaskControlBlockInner {
                address: ProcAddress::new(),
                trap_cx_ppn,
                base_size: user_sp,
                heap_start: user_heap,
                heap_pt: user_heap,
                task_cx_ptr: task_cx_ptr as usize,
                task_status: TaskStatus::Ready,
                memory_set,
                mmap_area: MmapArea::new(VirtAddr::from(MMAP_BASE), VirtAddr::from(MMAP_BASE)),
                parent: None,
                children: Vec::new(),
                exit_code: 0,
                fd_table: vec![
                    // 0 -> stdin
                    Some( FileDiscripter::new(
                        false,
                        FileClass::Abstr(Arc::new(Stdin)) 
                    )),
                    // 1 -> stdout
                    Some( FileDiscripter::new(
                        false,
                        FileClass::Abstr(Arc::new(Stdout)) 
                    )),
                    // 2 -> stderr
                    Some( FileDiscripter::new(
                        false,
                        FileClass::Abstr(Arc::new(Stdout)) 
                    )),
                ],
                current_path: String::from("/"), // 只有initproc在此建立，其他进程均为fork出
            }),
        };
        // prepare TrapContext in user space
        let trap_cx = task_control_block.acquire_inner_lock().get_trap_cx();
        *trap_cx = TrapContext::app_init_context(
            entry_point,
            user_sp,
            KERNEL_SPACE.lock().token(),
            kernel_stack_top,
            trap_handler as usize,
        );
        task_control_block
    }
    pub fn get_parent(&self) -> Option<Arc<TaskControlBlock>> {
        let inner = self.inner.lock();
        inner.parent.as_ref().unwrap().upgrade()
    }
    
    // exec will push following arguments to user stack:
    // STACK TOP
    //      argc
    //      *argv [] (with NULL as the end) 8 bytes each
    //      *envp [] (with NULL as the end) 8 bytes each: now only NULL
    //      auxv[] (with NULL as the end) 16 bytes each: now only NULL
    //      padding (16 bytes-align)
    //      rand bytes: Now set 0x00 ~ 0x0f (not support random) 16bytes
    //      String: platform "RISC-V64"
    //      Argument string(argv[])
    //      Environment String (envp[]): now NOTHING
    // STACK BOTTOM
    // Due to "push" operations, we will start from the bottom
    pub fn exec(&self, elf_data: &[u8], args: Vec<String>) {
        // memory_set with elf program headers/trampoline/trap context/user stack
        // TODO: check fd.cloexec and close fd when need!!!!!!!!
        let (memory_set, mut user_sp, user_heap, entry_point) = MemorySet::from_elf(elf_data);
        
        // println!("user_sp {:X}", user_sp);
        let trap_cx_ppn = memory_set
            .translate(VirtAddr::from(TRAP_CONTEXT).into())
            .unwrap()
            .ppn();
        
        let mut argv: Vec<usize> = (0..=args.len()).collect();
        ////////////// argv[] ///////////////////
        argv[args.len()] = 0;
        for i in 0..args.len() {
            user_sp -= args[i].len() + 1;
            // println!("user_sp {:X}", user_sp);
            argv[i] = user_sp;
            let mut p = user_sp;
            // write chars to [user_sp, user_sp + len]
            for c in args[i].as_bytes() {
                *translated_refmut(memory_set.token(), p as *mut u8) = *c;
                // print!("({})",*c as char);
                p += 1;
            }
            *translated_refmut(memory_set.token(), p as *mut u8) = 0;
        }
        // make the user_sp aligned to 8B for k210 platform
        user_sp -= user_sp % core::mem::size_of::<usize>();
        
        ////////////// platform String ///////////////////
        let platform = "RISC-V64";
        user_sp -= platform.len() + 1;
        user_sp -= user_sp % core::mem::size_of::<usize>();
        let mut p = user_sp;
        for c in platform.as_bytes() {
            *translated_refmut(memory_set.token(), p as *mut u8) = *c;
            p += 1;
        }
        *translated_refmut(memory_set.token(), p as *mut u8) = 0;

        ////////////// rand bytes ///////////////////
        user_sp -= 16;
        p = user_sp;
        for i in 0..0xf {
            *translated_refmut(memory_set.token(), p as *mut u8) = i as u8;
            p += 1;
        }
        
        ////////////// padding //////////////////////
        user_sp -= user_sp % 16;
        
        ////////////// auxv[] //////////////////////
        user_sp -= user_sp % 16;
        let auxv_base = user_sp;
        *translated_refmut(memory_set.token(), auxv_base as *mut usize) = 0;
        *translated_refmut(memory_set.token(), (auxv_base + core::mem::size_of::<usize>()) as *mut usize) = 0;

        ////////////// *envp [] //////////////////////
        user_sp -= core::mem::size_of::<usize>();
        let envp_base = user_sp;
        *translated_refmut(memory_set.token(), envp_base as *mut usize) = 0;
        
        ////////////// *argv [] //////////////////////
        user_sp -= (args.len() + 1) * core::mem::size_of::<usize>();
        let argv_base = user_sp;
        *translated_refmut(memory_set.token(), (user_sp + core::mem::size_of::<usize>() * (args.len())) as *mut usize) = 0;
        for i in 0..args.len() {
            *translated_refmut(memory_set.token(), (user_sp + core::mem::size_of::<usize>() * i) as *mut usize) = argv[i] ;
        }

        ////////////// argc //////////////////////
        user_sp -= core::mem::size_of::<usize>();
        *translated_refmut(memory_set.token(), user_sp as *mut usize) = args.len();


        // **** hold current PCB lock
        let mut inner = self.acquire_inner_lock();
        // substitute memory_set
        // QUES
        //inner.current_inode = par_inode_id;
        inner.memory_set = memory_set;
        inner.heap_start = user_heap;
        inner.heap_pt = user_heap;
        // println!("The heap start is {}", user_heap);
        // update trap_cx ppn
        inner.trap_cx_ppn = trap_cx_ppn;

        inner.fd_table.iter_mut()
            .find(
                |fd|{
                    fd.is_some() && fd.as_ref().unwrap().get_cloexec()
                }
            ).take();

        // initialize trap_cx
        // println!("[exec] entry point = 0x{:X}", entry_point);
        
        let mut trap_cx = TrapContext::app_init_context(
            entry_point,
            user_sp,
            KERNEL_SPACE.lock().token(),
            self.kernel_stack.get_top(),
            trap_handler as usize,
        );
        trap_cx.x[10] = args.len();
        trap_cx.x[11] = argv_base;
        trap_cx.x[12] = envp_base;
        trap_cx.x[13] = auxv_base;
        *inner.get_trap_cx() = trap_cx;
        gdb_println!(EXEC_ENABLE,"[exec] finish");
        // **** release current PCB lock
    }
    pub fn fork(self: &Arc<TaskControlBlock>, is_create_thread: bool) -> Arc<TaskControlBlock> {
        // ---- hold parent PCB lock
        let mut parent_inner = self.acquire_inner_lock();
        // copy user space(include trap context)
        let memory_set = MemorySet::from_existed_user(
            &parent_inner.memory_set
        );
        let trap_cx_ppn = memory_set
            .translate(VirtAddr::from(TRAP_CONTEXT).into())
            .unwrap()
            .ppn();
        // alloc a pid and a kernel stack in kernel space
        let pid_handle = pid_alloc();
        let mut tgid = 0;
        if is_create_thread{
            tgid = self.pid.0;
        }
        else{
            tgid = pid_handle.0;
        }
        let kernel_stack = KernelStack::new(&pid_handle);
        let kernel_stack_top = kernel_stack.get_top();
        // push a goto_trap_return task_cx on the top of kernel stack
        let task_cx_ptr = kernel_stack.push_on_top(TaskContext::goto_trap_return());
        // copy fd table
        let mut new_fd_table: FdTable = Vec::new();
        for fd in parent_inner.fd_table.iter() {
            if let Some(file) = fd {
                new_fd_table.push(Some( file.clone() ));
            } else {
                new_fd_table.push(None);
            }
        }
        //println!("fork: parent_inner.current_inode = {}",parent_inner.current_inode);
        let task_control_block = Arc::new(TaskControlBlock {
            pid: pid_handle,
            tgid,
            kernel_stack,
            inner: Mutex::new(TaskControlBlockInner {
                address: ProcAddress::new(),
                trap_cx_ppn,
                base_size: parent_inner.base_size,
                heap_start: parent_inner.heap_start,
                heap_pt: parent_inner.heap_pt,
                task_cx_ptr: task_cx_ptr as usize,
                task_status: TaskStatus::Ready,
                memory_set,
                mmap_area: MmapArea::new(VirtAddr::from(MMAP_BASE), VirtAddr::from(MMAP_BASE)),
                parent: Some(Arc::downgrade(self)),
                children: Vec::new(),
                exit_code: 0,
                fd_table: new_fd_table,
                current_path: parent_inner.current_path.clone(),
            }),
        });
        // add child
        parent_inner.children.push(task_control_block.clone());
        // modify kernel_sp in trap_cx
        // **** acquire child PCB lock
        let trap_cx = task_control_block.acquire_inner_lock().get_trap_cx();
        // **** release child PCB lock
        trap_cx.kernel_sp = kernel_stack_top;
        // return
        task_control_block
        // ---- release parent PCB lock
    }
    pub fn getpid(&self) -> usize {
        self.pid.0
    }

    pub fn gettgid(&self) -> usize {
        self.tgid
    }

    pub fn mmap(&self, start: usize, len: usize, prot: usize, flags: usize, fd: usize, off: usize) -> usize {
        let mut inner = self.acquire_inner_lock();
        let fd_table = inner.fd_table.clone();
        let token = inner.get_user_token();
        let va_top = inner.mmap_area.get_mmap_top();
        let end_va = VirtAddr::from(va_top.0 + len);

        inner.memory_set.insert_mmap_area(va_top, end_va, MapPermission::U | MapPermission::W | MapPermission::R);
        inner.mmap_area.push(start, len, prot, flags, fd, off, fd_table, token)
    }

    pub fn munmap(&self, start: usize, len: usize) -> isize {
        let mut inner = self.acquire_inner_lock();
        inner.memory_set.remove_area_with_start_vpn(VirtAddr::from(start).into());
        inner.mmap_area.remove(start, len)
    }


    // create mmap in kernel space, used for elf file only
    pub fn kmmap(&self, start: usize, len: usize, prot: usize, flags: usize, fd: usize, off: usize) -> usize {
        let mut ks_lock = KERNEL_SPACE.lock();
        let mut kma_lock = KERNEL_MMAP_AREA.lock();
        let mut inner = self.acquire_inner_lock();
        let fd_table = inner.fd_table.clone();
        let token = ks_lock.token();
        let va_top = kma_lock.get_mmap_top();
        let end_va = VirtAddr::from(va_top.0 + len);
        //println!("vatop = 0x{:X}, end_va = 0x{:X}", va_top.0, end_va.0);
        ks_lock.insert_mmap_area(va_top, end_va,  MapPermission::W | MapPermission::R );
        //let page_table = PageTable::from_token(KERNEL_TOKEN.token());
        //println!("pte = 0x{:X}", page_table.translate(VirtAddr::from(MMAP_BASE).floor()).unwrap().bits);
        //println!("ppn = 0x{:X}", page_table.translate(VirtAddr::from(MMAP_BASE).floor()).unwrap().ppn().0);
        //point();
        //ks_lock.activate();
        //unsafe{
        //    *(MMAP_BASE as *mut usize) = 5;
        //}
    
        kma_lock.push(start, len, prot, flags, fd, off, fd_table, token)
    }

    pub fn kmunmap(&self, start: usize, len: usize) -> isize {
        let mut ks_lock = KERNEL_SPACE.lock();
        let mut kma_lock = KERNEL_MMAP_AREA.lock();
        ks_lock.remove_area_with_start_vpn(VirtAddr::from(start).into());
        kma_lock.remove(start, len)
    }

    pub fn grow_proc(&self, grow_size: isize) -> usize {
        if grow_size > 0 {
            let growed_addr: usize = self.inner.lock().heap_pt + grow_size as usize;
            let limit = self.inner.lock().heap_start + USER_HEAP_SIZE;
            if growed_addr > limit {
                panic!("process doesn't have enough memsize to grow! {} {} {} {}", limit, self.inner.lock().heap_pt, growed_addr, self.pid.0);
            }
            self.inner.lock().heap_pt = growed_addr;
        }
        else {
            let shrinked_addr: usize = self.inner.lock().heap_pt + grow_size as usize;
            if shrinked_addr < self.inner.lock().heap_start {
                panic!("Memory shrinked to the lowest boundary!")
            }
            self.inner.lock().heap_pt = shrinked_addr;
        }
        return self.inner.lock().heap_pt;
    }
}

#[derive(Copy, Clone, PartialEq)]
pub enum TaskStatus {
    Ready,
    Running,
    Zombie,
}


#[inline(never)]
#[no_mangle]
pub fn point(){
    println!("hi");
}