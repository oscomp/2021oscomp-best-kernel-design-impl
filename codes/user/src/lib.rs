#![no_std]
#![feature(llvm_asm)]
#![feature(linkage)]
#![feature(panic_info_message)]
#![feature(alloc_error_handler)]


#[macro_use]
pub mod console;
mod syscall;
mod lang_items;

extern crate alloc;
#[macro_use]
extern crate bitflags;

use syscall::*;
use buddy_system_allocator::LockedHeap;
use alloc::vec::Vec;
use core::fmt::{self, Debug, Formatter};
use core::ops::{Add, Sub};

const USER_HEAP_SIZE: usize = 32768;
const AT_FDCWD: i32 = -100;

pub const TICKS_PER_SEC: usize = 100;
pub const MSEC_PER_SEC: usize = 1000;
pub const USEC_PER_SEC: usize = 1000_000;
pub const NSEC_PER_SEC: usize = 1000_000_000;

static mut HEAP_SPACE: [u8; USER_HEAP_SIZE] = [0; USER_HEAP_SIZE];

#[global_allocator]
static HEAP: LockedHeap = LockedHeap::empty();

#[alloc_error_handler]
pub fn handle_alloc_error(layout: core::alloc::Layout) -> ! {
    panic!("Heap allocation error, layout = {:?}", layout);
}

#[no_mangle]
#[link_section = ".text.entry"]
pub extern "C" fn _start(argc: usize, argv: usize) -> ! {
    unsafe {
        HEAP.lock()
            .init(HEAP_SPACE.as_ptr() as usize, USER_HEAP_SIZE);
        // println!{"``````````````````the address of heap_space is {:?}", HEAP_SPACE.as_ptr()}
    }
    let mut v: Vec<&'static str> = Vec::new();
    for i in 0..argc {
        let str_start = unsafe {
            ((argv + i * core::mem::size_of::<usize>()) as *const usize).read_volatile()
        };
        let len = (0usize..).find(|i| unsafe {
            ((str_start + *i) as *const u8).read_volatile() == 0
        }).unwrap();
        v.push(
            core::str::from_utf8(unsafe {
                core::slice::from_raw_parts(str_start as *const u8, len)
            }).unwrap()
        );
    }
    exit(main(argc, v.as_slice()));
}

#[linkage = "weak"]
#[no_mangle]
fn main(_argc: usize, _argv: &[&str]) -> i32 {
    panic!("Cannot find main!");
}

bitflags! {
    pub struct OpenFlags: u32 {
        const RDONLY = 0;
        const WRONLY = 1 << 0;
        const RDWR = 1 << 1;
        const CREATE = 1 << 6;
        const TRUNC = 1 << 10;
    }
}

#[derive(Copy, Clone)]
pub struct TimeVal{
    pub sec: usize,
    pub usec: usize,
}

impl TimeVal{
    pub fn new() -> Self{
        Self{
            sec:0,
            usec:0
        }
    }

    pub fn add_usec(&mut self, usec:usize){
        self.usec += usec;
        self.sec += self.usec/1000_000;
        self.usec %= 1000_000;
    }

    pub fn is_zero(&self) -> bool{
        self.sec == 0 && self.usec == 0
    }

}

impl Add for TimeVal {
    type Output = Self;

    fn add(self, other: Self) -> Self {
        let mut sec = self.sec + other.sec;
        let mut usec = self.usec + other.usec;
        sec += usec/USEC_PER_SEC;
        usec %= USEC_PER_SEC;
        Self {
            sec: sec,
            usec: usec,
        }
    }
}


// if self is less than other, then return 0
impl Sub for TimeVal {
    type Output = Self;

    fn sub(self, other: Self) -> Self {
        if self.sec < other.sec{
            return Self{sec:0,usec:0}
        }
        else if self.sec == other.sec{
            if self.usec < other.usec{
                return Self{sec:0,usec:0}
            }
            else{
                return Self{sec:0,usec:self.usec-other.usec}
            }
        }
        else{
            let mut sec = self.sec - other.sec;
            let mut usec = self.usec - other.usec;
            if self.usec < other.usec{
                sec -= 1;
                usec = USEC_PER_SEC + self.usec - other.usec;
            }
            Self {
                sec: sec,
                usec: usec,
            }
        }
    }
}

impl Debug for TimeVal {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.write_fmt(format_args!("( sec:{}, usec:{})", self.sec, self.usec))
    }
}

pub fn dup(fd: usize) -> isize { sys_dup(fd) }
pub fn chdir(path: &str) -> isize { sys_chdir(path) }
pub fn unlink(path: &str) -> isize { sys_unlinkat(AT_FDCWD, path, 0) }
pub fn mkdir(path: &str) -> isize { sys_mkdir(path) }
pub fn open(path: &str, flags: OpenFlags) -> isize { sys_open(path, flags.bits) }
pub fn close(fd: usize) -> isize { sys_close(fd) }
pub fn pipe(pipe_fd: &mut [usize]) -> isize { sys_pipe(pipe_fd) }
pub fn read(fd: usize, buf: &mut [u8]) -> isize { sys_read(fd, buf) }
pub fn write(fd: usize, buf: &[u8]) -> isize { sys_write(fd, buf) }
pub fn exit(exit_code: i32) -> ! { sys_exit(exit_code); }
pub fn yield_() -> isize { sys_yield() }
pub fn get_time() -> isize {
    let mut time = TimeVal::new();
    sys_get_time(&mut time);
    (time.sec*1000 + time.usec/1000) as isize
    }
pub fn get_timeval(time:&mut TimeVal) -> isize { sys_get_time(time) }
pub fn getpid() -> isize { sys_getpid() }
pub fn getppid() -> isize { sys_getppid() }
pub fn fork() -> isize { sys_fork() }
pub fn exec(path: &str, args: &[*const u8]) -> isize { 
    // println!{"calling exec2..."}
    sys_exec(path, args) }
pub fn wait(exit_code: &mut i32) -> isize {
    loop {
        match sys_waitpid(-1, exit_code as *mut _) {
            -2 => { yield_(); }
            // -1 or a real pid
            exit_pid => return exit_pid,
        }
    }
}
pub fn wait4(pid: isize, wstatus: &mut i32, option: usize) -> isize {
    sys_wait4(pid, wstatus, option)
}
pub fn waitpid(pid: usize, exit_code: &mut i32) -> isize {
    loop {
        match sys_waitpid(pid as isize, exit_code as *mut _) {
            -2 => { yield_(); }
            // -1 or a real pid
            exit_pid => return exit_pid,
        }
    }
}

pub fn sleep(period_ms: usize) {
    sys_sleep(period_ms);
}

// Not standard POSIX sys_call
pub fn ls(path: &str) -> isize { sys_ls(path) }
pub fn shutdown() -> isize { sys_shutdown() }
pub fn trap() -> isize { sys_trap() }
