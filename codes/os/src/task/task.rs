use crate::{console::print, mm::{
    MemorySet,
    PhysPageNum,
    KERNEL_SPACE, 
    VirtAddr,
    translated_refmut,
    MmapArea,
    MapPermission,
}};
use crate::trap::{TrapContext, trap_handler};
use crate::config::{TRAP_CONTEXT, USER_HEAP_SIZE, MMAP_BASE};
use super::TaskContext;
use super::{PidHandle, pid_alloc, KernelStack};
use alloc::sync::{Weak, Arc};
use alloc::vec;
use alloc::vec::Vec;
use alloc::string::String;
use spin::{Mutex, MutexGuard};
use crate::fs::{File, Stdin, Stdout, FileClass};

pub struct TaskControlBlock {
    // immutable
    pub pid: PidHandle,
    pub kernel_stack: KernelStack,
    // mutable
    inner: Mutex<TaskControlBlockInner>,
}

pub struct TaskControlBlockInner {
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
    pub fd_table: Vec<Option<FileClass>>,
    pub current_path: String,
    pub mmap_area: MmapArea,
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
        let kernel_stack = KernelStack::new(&pid_handle);
        let kernel_stack_top = kernel_stack.get_top();
        // push a task context which goes to trap_return to the top of kernel stack
        let task_cx_ptr = kernel_stack.push_on_top(TaskContext::goto_trap_return());
        let task_control_block = Self {
            pid: pid_handle,
            kernel_stack,
            inner: Mutex::new(TaskControlBlockInner {
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
                    Some( FileClass::Abstr(Arc::new(Stdin)) ),
                    // 1 -> stdout
                    Some( FileClass::Abstr(Arc::new(Stdout)) ),
                    // 2 -> stderr
                    Some( FileClass::Abstr(Arc::new(Stdout)) ),
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
    pub fn exec(&self, elf_data: &[u8], args: Vec<String>) {
        // memory_set with elf program headers/trampoline/trap context/user stack
        let (memory_set, mut user_sp, user_heap, entry_point) = MemorySet::from_elf(elf_data);
        let trap_cx_ppn = memory_set
            .translate(VirtAddr::from(TRAP_CONTEXT).into())
            .unwrap()
            .ppn();
        // push arguments on user stack
        // sp减小[参数个数*usize]，用于存放参数地址 
        user_sp -= (args.len() + 1) * core::mem::size_of::<usize>();
        let argv_base = user_sp;
        
        let mut argv: Vec<_> = (0..=args.len())
            .map(|arg| {
                translated_refmut(
                    memory_set.token(),
                    (argv_base + arg * core::mem::size_of::<usize>()) as *mut usize
                )
            })
            .collect();

        // argv的类型实际为 Vec<&'static mut T>
        // 所以这里直接往对应的地址写数据
        *argv[args.len()] = 0;
        for i in 0..args.len() {
            // sp继续向下增长，留空间给每个参数
            // +1是因为需要'\0'
            user_sp -= args[i].len() + 1;
            *argv[i] = user_sp;
            let mut p = user_sp;
            // 将字符写入栈 [user_sp, user_sp + len]
            for c in args[i].as_bytes() {
                *translated_refmut(memory_set.token(), p as *mut u8) = *c;
                p += 1;
            }
            *translated_refmut(memory_set.token(), p as *mut u8) = 0;
        }
        // make the user_sp aligned to 8B for k210 platform
        user_sp -= user_sp % core::mem::size_of::<usize>();

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
        // initialize trap_cx
        let mut trap_cx = TrapContext::app_init_context(
            entry_point,
            user_sp,
            KERNEL_SPACE.lock().token(),
            self.kernel_stack.get_top(),
            trap_handler as usize,
        );
        trap_cx.x[10] = args.len();
        trap_cx.x[11] = argv_base;
        *inner.get_trap_cx() = trap_cx;
        // **** release current PCB lock
    }
    pub fn fork(self: &Arc<TaskControlBlock>) -> Arc<TaskControlBlock> {
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
        let kernel_stack = KernelStack::new(&pid_handle);
        let kernel_stack_top = kernel_stack.get_top();
        // push a goto_trap_return task_cx on the top of kernel stack
        let task_cx_ptr = kernel_stack.push_on_top(TaskContext::goto_trap_return());
        // copy fd table
        let mut new_fd_table: Vec<Option<FileClass>> = Vec::new();
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
            kernel_stack,
            inner: Mutex::new(TaskControlBlockInner {
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