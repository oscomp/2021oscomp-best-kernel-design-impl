#![no_std]
#![no_main]

#[macro_use]
extern crate user_lib;
use user_lib::{fork, wait4, exit};

#[no_mangle]
pub fn main() {
    let mut i: i32 = 100000;
    let mut wstatus: i32 = 0;
    let cpid: isize = fork();
    if cpid == 0 {
        while i > 0 {i -= 1;}
        println!{"this is the child process!"};
        exit(3);
    } else {
        let pid_t: isize = wait4(cpid, & mut wstatus, 0);
        assert_eq!(pid_t, cpid);
        wstatus >>= 8;
        println!{"wstatus: {}", wstatus};
    }
}