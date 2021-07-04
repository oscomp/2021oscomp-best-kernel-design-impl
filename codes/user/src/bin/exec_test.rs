#![no_std]
#![no_main]

#[macro_use]
extern crate user_lib;
use user_lib::{fork, yield_, waitpid, exit, wait};

const MAGIC: i32 = -0x10384;

#[no_mangle]
pub fn main(argc:isize, argv: *const usize) -> i32 {
    unsafe{
        println!("argc: {}",argc);
        for i in 0..argc{
            let mut j=0;
            let mut c:char = 1 as char;
            print!("argv[{}]:",i);
            while c != 0 as char {
                let array = (*(argv.add(i as usize)) + j) as  *const u8;
                c = *array as char;
                print!("{}",c);
                j +=1;
            }
            println!("");
        }
    }
    0
}

