mod context;

use riscv::register::{
    mtvec::TrapMode,
    stvec,
    scause::{
        self,
        Trap,
        Exception,
        Interrupt,
    },
    sepc,
    stval,
    sie,
};
use crate::mm::{
    VirtAddr,
    VirtPageNum,
    print_free_pages,
};
use crate::syscall::{syscall, test};
use crate::task::{
    exit_current_and_run_next,
    suspend_current_and_run_next,
    current_user_token,
    current_trap_cx,
    get_core_id,
    current_task,
    update_user_clock, 
    update_kernel_clock, 
    get_user_runtime_usec, 
    get_kernel_runtime_usec,
    Signals,
    perform_signal_handler,
};
use crate::timer::set_next_trigger;
use crate::config::{TRAP_CONTEXT, TRAMPOLINE, USER_STACK_SIZE};
use crate::gdb_print;
use crate::monitor::*;

global_asm!(include_str!("trap.S"));
global_asm!(include_str!("trap_signal.S"));

pub fn init() {
    set_kernel_trap_entry();
}

fn set_kernel_trap_entry() {
    unsafe {
        stvec::write(trap_from_kernel as usize, TrapMode::Direct);
    }
}

fn set_user_trap_entry() {
    unsafe {
        stvec::write(TRAMPOLINE as usize, TrapMode::Direct);
    }
}

pub fn enable_timer_interrupt() {
    unsafe { sie::set_stimer(); }
}


pub struct GlobalSatp {
    satp:usize,
    syscall:usize,
}

impl GlobalSatp{
    pub fn set(&mut self, satp:usize){
        self.satp = satp;
    }
    pub fn get(&self)->usize{
        self.satp
    }
    pub fn set_syscall(&mut self, syscall_id:usize){
        self.syscall = syscall_id;
    }
    pub fn get_syscall(&self)->usize{
        self.syscall
    }
}

use lazy_static::lazy_static;
use alloc::sync::Arc;
use spin::Mutex;
lazy_static! {
    pub static ref G_SATP: Arc<Mutex<GlobalSatp>> = Arc::new(Mutex::new(GlobalSatp{satp:0, syscall:0}));
}

#[no_mangle]
pub fn trap_handler() -> ! {
    set_kernel_trap_entry();
    //G_SATP.lock().set(current_user_token());
    //crate::syscall::test();
    // update RUsage of process
    // let ru_utime = get_user_runtime_usec();
    // current_task().unwrap().acquire_inner_lock().rusage.add_utime(ru_utime);
    // update_kernel_clock();

    //let mut is_schedule = false;
    let scause = scause::read();
    let stval = stval::read();
    match scause.cause() {
        Trap::Exception(Exception::UserEnvCall) => {
            // println!{"pinUserEnvCall"}
            // jump to next instruction anyway
            let mut cx = current_trap_cx();
            cx.sepc += 4;

            //G_SATP.lock().set_syscall(cx.x[17]);
            let syscall_id = cx.x[17];
            if syscall_id > 62 && syscall_id != 113 {
                unsafe {
                    //llvm_asm!("sfence.vma zero, zero" :::: "volatile");
                }
            }
            //get system call return value
            let result = syscall(syscall_id, [cx.x[10], cx.x[11], cx.x[12], cx.x[13], cx.x[14], cx.x[15]]);
            // cx is changed during sys_exec, so we have to call it again
            //if syscall_id != 64 && syscall_id != 63{
            //    println!("[{}]syscall-({}) = 0x{:X}  ", current_task().unwrap().pid.0, syscall_id, result);
            //} 
            cx = current_trap_cx();
            cx.x[10] = result as usize;
            // println!{"cx written..."}
        }
        
        Trap::Exception(Exception::InstructionFault) |
        Trap::Exception(Exception::InstructionPageFault) => {
            let task = current_task().unwrap();
            // println!{"pinLoadFault"}
            //println!("prev syscall = {}", G_SATP.lock().get_syscall());
            println!(
                "[kernel] {:?} in application-{}, bad addr = {:#x}, bad instruction = {:#x}, core dumped.",
                scause.cause(),
                task.pid.0,
                stval,
                current_trap_cx().sepc,
            );
            drop(task);
            // page fault exit code
            let current_task = current_task().unwrap();
            if current_task.is_signal_execute() || !current_task.check_signal_handler(Signals::SIGSEGV){
                drop(current_task);
                exit_current_and_run_next(-2);
            }
        }
        Trap::Exception(Exception::LoadFault) |
        Trap::Exception(Exception::StoreFault) |
        Trap::Exception(Exception::StorePageFault) |
        Trap::Exception(Exception::LoadPageFault) => {
            // println!("page fault 1");
            let va: VirtAddr = (stval as usize).into();
            // The boundary decision
            if va > TRAMPOLINE.into() {
                panic!("VirtAddr out of range!");
            }
            let vpn: VirtPageNum = va.floor();
            let heap_base = current_task().unwrap().acquire_inner_lock().heap_start;
            let heap_pt = current_task().unwrap().acquire_inner_lock().heap_pt;
            let stack_top = current_task().unwrap().acquire_inner_lock().base_size;
            let stack_bottom = stack_top - USER_STACK_SIZE;
            // println!{"The base of the user stack: {:X} ~ {:X}", stack_bottom, stack_top};
            // println!{"============================{:?}", vpn}
            let mmap_start = current_task().unwrap().acquire_inner_lock().mmap_area.mmap_start;
            let mmap_end = current_task().unwrap().acquire_inner_lock().mmap_area.mmap_top;
            // println!("page fault 2");
            // println!{"start: {:?} end: {:?}", mmap_start, mmap_end};
            if va >= mmap_start && va < mmap_end {
                //println!{"where is the lazy_mmap_page!!!!!!!!"}
                // exit_current_and_run_next(-2);
                current_task().unwrap().lazy_mmap(va.0);
            } else if va.0 >= heap_base && va.0 <= heap_pt {
                current_task().unwrap().acquire_inner_lock().lazy_alloc_heap(vpn);
            } else if va.0 >= stack_bottom && va.0 <= stack_top {
                //println!{"lazy_stack_page: {:?}", va}
                current_task().unwrap().acquire_inner_lock().lazy_alloc_stack(vpn);
            } else {
                // get the PageTableEntry that faults
                let pte = current_task().unwrap().acquire_inner_lock().enquire_vpn(vpn);
                // println!{"PageTableEntry: {}", pte.bits};
                // if the virtPage is a CoW
                if pte.is_some() && pte.unwrap().is_cow() {
                    let former_ppn = pte.unwrap().ppn();
                    //println!{"1---{}: {:?}", current_task().unwrap().pid.0, current_task().unwrap().acquire_inner_lock().get_trap_cx()};
                    // println!("cow addr = {:X}", stval);
                    current_task().unwrap().acquire_inner_lock().cow_alloc(vpn, former_ppn);
                    // println!{"2---{:?}", current_task().unwrap().acquire_inner_lock().get_trap_cx()};
                    // println!{"cow_alloc returned..."}
                    // let pte = current_task().unwrap().acquire_inner_lock().translate_vpn(vpn);
                    // println!{"PageTableEntry: {}", pte.bits};
                } else {
                    // page fault exit code
                    let current_task = current_task().unwrap();
                    if current_task.is_signal_execute() || !current_task.check_signal_handler(Signals::SIGSEGV){
                        println!(
                            "[kernel] {:?} in application, bad addr = {:#x}, bad instruction = {:#x}, core dumped.",
                            scause.cause(),
                            stval,
                            current_trap_cx().sepc,
                        );
                        drop(current_task);
                        exit_current_and_run_next(-2);
                    }
                }
            }
            // println!{"Trap solved..."}
        }
        Trap::Exception(Exception::IllegalInstruction) => {
            // println!{"pinIllegalInstruction"}
            println!("[kernel] IllegalInstruction in application, continue.");
            //let mut cx = current_trap_cx();
            //cx.sepc += 4;
            println!(
                "         {:?} in application, bad addr = {:#x}, bad instruction = {:#x}, core dumped.",
                scause.cause(),
                stval,
                current_trap_cx().sepc,
            );
            // illegal instruction exit code
            exit_current_and_run_next(-3);
        }
        Trap::Interrupt(Interrupt::SupervisorTimer) => {
            gdb_print!(TIMER_ENABLE,"[timer]");
            set_next_trigger();
            suspend_current_and_run_next();
            //is_schedule = true;
        }
        _ => {
            panic!("Unsupported trap {:?}, stval = {:#x}!", scause.cause(), stval);
        }
    }
    // println!("before trap_return");
    trap_return();
}

#[no_mangle]
pub fn trap_return() -> ! {
   

    // update RUsage of process
    // update_user_clock();
    // let ru_stime = get_kernel_runtime_usec();
    // current_task().unwrap().acquire_inner_lock().rusage.add_stime(ru_stime);
    perform_signal_handler();
    // println!("after perform_signal_handler");
    set_user_trap_entry();

    // println!("core:{} trap return ",get_core_id());
    let trap_cx_ptr = TRAP_CONTEXT;
    //println!("{:X}", TRAP_CONTEXT);
    let user_satp = current_user_token();
    //println!("[trap_return] satp = {:X}", user_satp);
    // println!("satp = {:X}", current_user_token());
    //let task = current_task().unwrap();
    //let inner = task.acquire_inner_lock();
    //inner.print_cx();
    //drop(inner);
    //drop(task);
    //println!{"{:?}", current_task().unwrap().acquire_inner_lock().get_trap_cx()};
    let trap_cx = current_task().unwrap().acquire_inner_lock().get_trap_cx();
    if trap_cx.get_sp() == 0{
        println!("[trap_ret] sp = 0");
    }
    //println!("[trap_ret] sepc = {:X}", trap_cx.sepc);

    extern "C" {
        fn __alltraps();
        fn __restore();
        fn __signal_trampoline();
    }
    // println!("__signal_trampoline is at 0x{:X}", __signal_trampoline as usize);
    let restore_va = __restore as usize - __alltraps as usize + TRAMPOLINE;
    // println!{"restore_va: {}", restore_va}
    //let sched:usize = {
    //    if user_satp != G_SATP.lock().get() {
    //        //println!("{:X}  {:X}", user_satp, G_SATP.lock().get());
    //        1
    //    } else {
    //        0
    //    }
    //};
    unsafe {
        llvm_asm!("fence.i" :::: "volatile");
        // WARNING: here, we make a2 = __signal_trampoline, because otherwise the "__signal_trampoline" func will be optimized to DEATH
        llvm_asm!("jr $0" :: "r"(restore_va), "{a0}"(trap_cx_ptr), "{a1}"(user_satp), "{a2}"(__signal_trampoline as usize) :: "volatile");
    }
    panic!("Unreachable in back_to_user!");
}

#[no_mangle]
pub fn trap_from_kernel() -> ! {
    panic!("a trap {:?} from kernel! Stvec:{:x}, Stval:{:X}", scause::read().cause(), stvec::read().bits(), stval::read());
}

pub use context::{TrapContext};
