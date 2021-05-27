#![no_std]
#![no_main]
#![feature(global_asm)]
#![feature(llvm_asm)]
#![feature(asm)]
#![feature(panic_info_message)]
#![feature(const_in_array_repeat_expressions)]
#![feature(alloc_error_handler)]

use lazy_static::lazy_static;
use sbi::sbi_send_ipi;
use spin::*;
use alloc::sync::Arc;

extern crate alloc;

#[macro_use]
extern crate bitflags;

#[macro_use]
mod console;
mod lang_items;
mod sbi;
mod syscall;
mod trap;
mod config;
mod task;
mod timer;
mod mm;
mod fs;
mod drivers;

global_asm!(include_str!("entry.asm"));
global_asm!(include_str!("start_app.S"));

fn clear_bss() {
    extern "C" {
        fn sbss();
        fn ebss();
    }
    (sbss as usize..ebss as usize).for_each(|a| {
        unsafe { (a as *mut u8).write_volatile(0) }
    });
}

pub fn id() -> usize {
    let cpu_id;
    unsafe {
        llvm_asm!("mv $0, tp" : "=r"(cpu_id));
    }
    cpu_id
}

struct Core2flag{
    is_in: bool,
}

impl Core2flag{
    pub fn is_in(&self)->bool{
        self.is_in
    }
    pub fn set_in(&mut self){
        self.is_in = true;
    }
}


lazy_static! {
    static ref CORE2_FLAG: Arc<Mutex<Core2flag>> = Arc::new(Mutex::new(
        Core2flag{
            is_in:false,
        }
    ));
}

#[no_mangle]
pub fn rust_main() -> ! {
    //let mut sp:usize;
    //let mut tp:usize;
    //unsafe {
    //    llvm_asm!("mv $0, sp" : "=r"(sp));
    //    llvm_asm!("mv $0, tp" : "=r"(tp));
    //}
    //println!("sp:{:x}",sp);
    //println!("tp:{:x}",tp);
    //println!("enter kernel");
        // println!("[core 2] Hello, world!");
    let core = id();
    //println!("get id");
    if core != 0 {
        println!("other core");
        loop{}
        //mm::init_othercore();
        //trap::init();
        //trap::enable_timer_interrupt();
        //timer::set_next_trigger();
        //task::run_tasks();
        //panic!("Unreachable in rust_main!");
    }
    //println!("clear bss");
    clear_bss();
    //println!("init mm");
    mm::init();
    //println!("init mm ... ok");
    mm::remap_test();
    trap::init();
    trap::enable_timer_interrupt();
    timer::set_next_trigger();
    //println!("init fs");
    fs::list_apps();
    task::add_initproc();
    let mask:usize = 1 << 1;
    sbi_send_ipi(&mask as *const usize as usize);
    // CORE2_FLAG.lock().set_in();
    task::run_tasks();
    panic!("Unreachable in rust_main!");
}