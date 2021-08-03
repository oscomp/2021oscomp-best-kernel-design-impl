use core::ops::Index;

use crate::{console::print, mm::{
    MemorySet,
    PhysPageNum,
    KERNEL_SPACE, 
    KERNEL_MMAP_AREA, 
    //KERNEL_TOKEN,
    //PageTable,
    VirtAddr,
    VirtPageNum,
    PageTableEntry,
    translated_refmut,
    MmapArea,
    MapPermission,
    // PTEFlags,
}};
use crate::trap::{TrapContext, trap_handler};
use crate::config::*;
use crate::gdb_println;
use crate::monitor::*;
use super::TaskContext;
use super::{PidHandle, pid_alloc, KernelStack, RUsage};
use alloc::sync::{Weak, Arc};
use alloc::vec;
use alloc::vec::Vec;
use alloc::string::String;
use core::fmt::{self, Debug, Formatter};
use spin::{Mutex, MutexGuard};
use crate::fs::{ FileDescripter, Stdin, Stdout, FileClass};

pub struct AuxHeader{
    pub aux_type: usize,
    pub value: usize,
}

impl Debug for AuxHeader {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.write_fmt(format_args!("AuxHeader: type:{} value:0x{:X}", self.aux_type, self.value))
    }
}


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

pub type FdTable =  Vec<Option<FileDescripter>>;
pub struct TaskControlBlockInner {
    // task
    pub trap_cx_ppn: PhysPageNum,
    pub task_cx_ptr: usize,
    pub task_status: TaskStatus,
    pub parent: Option<Weak<TaskControlBlock>>,
    pub children: Vec<Arc<TaskControlBlock>>,
    pub exit_code: i32,
    // memory
    pub memory_set: MemorySet,
    pub base_size: usize,
    pub heap_start: usize,
    pub heap_pt: usize,
    pub mmap_area: MmapArea,
    // file
    pub fd_table: FdTable,
    pub current_path: String,
    // info
    pub address: ProcAddress,
    pub rusage:RUsage,
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
        // println!{"trap_cx_ppn: {:X}", self.trap_cx_ppn.0}
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
            // println!("task_cx = {:?}", *(self.task_cx_ptr as *const TaskContext) );
            // println!("trap_cx = {:?}", *(self.trap_cx_ppn.0 as *const TrapContext) );
        }
    }

    pub fn get_work_path(&self)->String{
        self.current_path.clone()
    }
    pub fn translate_vpn(&self, vpn: VirtPageNum) -> PageTableEntry {
        self.memory_set.translate(vpn).unwrap()
    }
    pub fn enquire_vpn(&self, vpn: VirtPageNum) -> Option<PageTableEntry> {
        self.memory_set.translate(vpn)
    }
    pub fn cow_alloc(&mut self, vpn: VirtPageNum, former_ppn: PhysPageNum) -> usize {
        let ret = self.memory_set.cow_alloc(vpn, former_ppn);
        // println!{"finished cow_alloc!"}
        ret
    }
}



impl TaskControlBlock {
    pub fn acquire_inner_lock(&self) -> MutexGuard<TaskControlBlockInner> {
        // println!{"acquiring lock..."}
        self.inner.lock()
    }
    pub fn new(elf_data: &[u8]) -> Self {
        // memory_set with elf program headers/trampoline/trap context/user stack
        let (memory_set, user_sp, user_heap, entry_point, auxv) = MemorySet::from_elf(elf_data);
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
                rusage: RUsage::new(),
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
                    Some( FileDescripter::new(
                        false,
                        FileClass::Abstr(Arc::new(Stdin)) 
                    )),
                    // 1 -> stdout
                    Some( FileDescripter::new(
                        false,
                        FileClass::Abstr(Arc::new(Stdout)) 
                    )),
                    // 2 -> stderr
                    Some( FileDescripter::new(
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
    //      *envp [] (with NULL as the end) 8 bytes each
    //      auxv[] (with NULL as the end) 16 bytes each: now has PAGESZ(6)
    //      padding (16 bytes-align)
    //      rand bytes: Now set 0x00 ~ 0x0f (not support random) 16bytes
    //      String: platform "RISC-V64"
    //      Argument string(argv[])
    //      Environment String (envp[]): now has SHELL, PWD, LOGNAME, HOME, USER, PATH
    // STACK BOTTOM
    // Due to "push" operations, we will start from the bottom
    pub fn exec(&self, elf_data: &[u8], args: Vec<String>) {
        // memory_set with elf program headers/trampoline/trap context/user stack
        
        let (memory_set, mut user_sp, user_heap, entry_point, mut auxv) = MemorySet::from_elf(elf_data);
        
        // println!("user_sp {:X}", user_sp);
        let trap_cx_ppn = memory_set
            .translate(VirtAddr::from(TRAP_CONTEXT).into())
            .unwrap()
            .ppn();
        
        ////////////// envp[] ///////////////////
        let mut env: Vec<String> = Vec::new();
        env.push(String::from("SHELL=/user_shell"));
        env.push(String::from("PWD=/"));
        env.push(String::from("USER=root"));
        env.push(String::from("MOTD_SHOWN=pam"));
        env.push(String::from("LANG=C.UTF-8"));
        env.push(String::from("INVOCATION_ID=e9500a871cf044d9886a157f53826684"));
        env.push(String::from("TERM=vt220"));
        env.push(String::from("SHLVL=2"));
        env.push(String::from("JOURNAL_STREAM=8:9265"));
        env.push(String::from("OLDPWD=/root"));
        env.push(String::from("_=busybox"));
        env.push(String::from("LOGNAME=root"));
        env.push(String::from("HOME=/"));
        env.push(String::from("PATH=/"));
        let mut envp: Vec<usize> = (0..=env.len()).collect();
        envp[env.len()] = 0;
        for i in 0..env.len() {
            user_sp -= env[i].len() + 1;
            envp[i] = user_sp;
            let mut p = user_sp;
            // write chars to [user_sp, user_sp + len]
            for c in env[i].as_bytes() {
                *translated_refmut(memory_set.token(), p as *mut u8) = *c;
                p += 1;
            }
            *translated_refmut(memory_set.token(), p as *mut u8) = 0;
        }
        // make the user_sp aligned to 8B for k210 platform
        user_sp -= user_sp % core::mem::size_of::<usize>();


        ////////////// argv[] ///////////////////
        let mut argv: Vec<usize> = (0..=args.len()).collect();
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
        auxv.push(AuxHeader{aux_type: AT_RANDOM, value: user_sp});
        for i in 0..0xf {
            *translated_refmut(memory_set.token(), p as *mut u8) = i as u8;
            p += 1;
        }
        
        ////////////// padding //////////////////////
        user_sp -= user_sp % 16;
        
        ////////////// auxv[] //////////////////////
        auxv.push(AuxHeader{aux_type: AT_EXECFN, value: argv[0]});// file name
        auxv.push(AuxHeader{aux_type: AT_NULL, value:0});// end
        user_sp -= auxv.len() * core::mem::size_of::<AuxHeader>();
        let auxv_base = user_sp;
        // println!("[auxv]: base 0x{:X}", auxv_base);
        for i in 0..auxv.len() {
            // println!("[auxv]: {:?}", auxv[i]);
            let addr = user_sp + core::mem::size_of::<AuxHeader>() * i;
            *translated_refmut(memory_set.token(), addr as *mut usize) = auxv[i].aux_type ;
            *translated_refmut(memory_set.token(), (addr + core::mem::size_of::<usize>()) as *mut usize) = auxv[i].value ;
        }


        ////////////// *envp [] //////////////////////
        user_sp -= (env.len() + 1) * core::mem::size_of::<usize>();
        let envp_base = user_sp;
        *translated_refmut(memory_set.token(), (user_sp + core::mem::size_of::<usize>() * (env.len())) as *mut usize) = 0;
        for i in 0..env.len() {
            *translated_refmut(memory_set.token(), (user_sp + core::mem::size_of::<usize>() * i) as *mut usize) = envp[i] ;
        }
        
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
        // println!{"--------------------pin15"}
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
        // gdb_println!(EXEC_ENABLE,"[exec] finish");
        // **** release current PCB lock
    }
    
    pub fn fork(self: &Arc<TaskControlBlock>, is_create_thread: bool) -> Arc<TaskControlBlock> {
        // ---- hold parent PCB lock
        let mut parent_inner = self.acquire_inner_lock();
        // println!{"trap context of pid{}: {:X}", self.pid.0, parent_inner.trap_cx_ppn.0}
        parent_inner.print_cx();
        let user_heap_top = parent_inner.heap_start + USER_HEAP_SIZE;
        let user_heap_base = parent_inner.heap_start;
        // copy user space(include trap context)
        let memory_set = MemorySet::from_copy_on_write(
            &mut parent_inner.memory_set,
            user_heap_base,
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
                rusage: RUsage::new(),
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
        task_control_block.acquire_inner_lock().print_cx();
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

    pub fn mmap(&self, start: usize, len: usize, prot: usize, flags: usize, fd: isize, off: usize) -> usize {
        // gdb_println!(SYSCALL_ENABLE,"[mmap](0x{:X},{},{},0x{:X},{},{})",start, len, prot, flags, fd, off);
        
        if start % PAGE_SIZE != 0{
            panic!("mmap: start_va not aligned");
        } 
        let mut inner = self.acquire_inner_lock();
        let fd_table = inner.fd_table.clone();
        let token = inner.get_user_token();
        let mut va_top = inner.mmap_area.get_mmap_top();
        let mut end_va = VirtAddr::from(va_top.0 + len);
        // "prot<<1" is equal to  meaning of "MapPermission"
        let map_flags = (((prot & 0b111)<<1) + (1<<4))  as u8; // "1<<4" means user
    
        let mut startvpn = start/PAGE_SIZE;
        
        if start != 0 { // "Start" va Already mapped
            while startvpn < (start+len)/PAGE_SIZE {
                // println!("[mmap]:map_flags 0x{:X}",map_flags);
                // inner.memory_set.print_pagetable();
                if inner.memory_set.set_pte_flags(startvpn.into(), map_flags as usize) == -1{
                    panic!("mmap: start_va not mmaped");
                }
                // inner.memory_set.print_pagetable();
                startvpn +=1;
            }
            return start;
        }
        else{ // "Start" va not mapped
            // println!("[insert_mmap_area]: va_top 0x{:X} end_va 0x{:X}", va_top.0, end_va.0);
            // println!("[insert_mmap_area]: flags 0x{:X}",flags);
            // println!("[insert_mmap_area]: map_flags 0x{:X}",map_flags);
            // println!("[insert_mmap_area]: map_flags {:?}",MapPermission::from_bits(map_flags).unwrap());
            // inner.memory_set.print_pagetable();
            inner.memory_set.insert_mmap_area(va_top, end_va, MapPermission::from_bits(map_flags).unwrap());
            // inner.memory_set.print_pagetable();
            inner.mmap_area.push(va_top.0, len, prot, flags, fd, off, fd_table, token)
        }
    }

    pub fn munmap(&self, start: usize, len: usize) -> isize {
        let mut inner = self.acquire_inner_lock();
        inner.memory_set.remove_area_with_start_vpn(VirtAddr::from(start).into());
        inner.mmap_area.remove(start, len)
    }


    // create mmap in kernel space, used for elf file only
    pub fn kmmap(&self, start: usize, len: usize, prot: usize, flags: usize, fd: isize, off: usize) -> usize {
        gdb_println!(SYSCALL_ENABLE,"kmap(0x{:X},{},{},{},{},{})",start, len, prot, flags, fd, off);
        let mut ks_lock = KERNEL_SPACE.lock();
        let mut kma_lock = KERNEL_MMAP_AREA.lock();
        let mut inner = self.acquire_inner_lock();
        let fd_table = inner.fd_table.clone();
        let token = ks_lock.token();
        let va_top = kma_lock.get_mmap_top();
        let end_va = VirtAddr::from(va_top.0 + len);
        // println!("vatop = 0x{:X}, end_va = 0x{:X}", va_top.0, end_va.0);
        ks_lock.insert_mmap_area(va_top, end_va,  MapPermission::W | MapPermission::R );
        // let page_table = PageTable::from_token(KERNEL_TOKEN.token());
        // println!("pte = 0x{:X}", page_table.translate(VirtAddr::from(MMAP_BASE).floor()).unwrap().bits);
        //println!("ppn = 0x{:X}", page_table.translate(VirtAddr::from(MMAP_BASE).floor()).unwrap().ppn().0);
        //point();
        //ks_lock.activate();
        //unsafe{
        //    *(MMAP_BASE as *mut usize) = 5;
        //}
    
        kma_lock.push(va_top.into(), len, prot, flags, fd, off, fd_table, token)
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