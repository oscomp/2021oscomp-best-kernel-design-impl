#![no_std]
#![no_main]

#[macro_use]
extern crate user_lib;


use user_lib::console::getchar;

use user_lib::ls;

#[no_mangle]
pub fn main(argc: usize, argv: &[&str]) -> i32 {
    ls(argv[0]);
    0
}