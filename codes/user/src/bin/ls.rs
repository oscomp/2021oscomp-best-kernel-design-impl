#![no_std]
#![no_main]

#[macro_use]
extern crate user_lib;


use user_lib::console::getchar;

use user_lib::ls;

#[no_mangle]
pub fn main(argc: usize, argv: &[&str]) -> i32 {
    if argv.is_empty(){
        ls("l\0");
    }
    else{
        if argc == 1 {
            ls("\0");
        } else{
            ls(argv[1]);
        }
    }
    0
}