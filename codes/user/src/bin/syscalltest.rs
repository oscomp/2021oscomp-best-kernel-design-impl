#![no_std]
#![no_main]

#[macro_use]
extern crate user_lib;

use user_lib::*;

pub fn simple_syscall(){
    let mut start = TimeVal::new();
    let mut end = TimeVal::new();
    get_timeval(&mut start);
    println!("test: run sys_getppid 1000000 times, start {:?}",start);
    for _ in 0..1000000{
        getppid();
    }
    get_timeval(&mut end);
    println!("test: run sys_getppid 1000000 times, end {:?}",end);
    println!("test: run sys_getppid 1000000 times, spent {:?}",end-start);
}

pub fn simple_trap(){
    let mut start = TimeVal::new();
    let mut end = TimeVal::new();
    get_timeval(&mut start);
    println!("test: run trap 1000000 times, start {:?}",start);
    for _ in 0..1000000{
        trap();
    }
    get_timeval(&mut end);
    println!("test: run trap 1000000 times, end {:?}",end);
    println!("test: run trap 1000000 times, spent {:?}",end-start);
}

#[no_mangle]
pub fn main() -> i32 {
    simple_trap();
    0
}