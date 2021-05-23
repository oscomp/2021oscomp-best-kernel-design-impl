use super::File;
use crate::mm::{UserBuffer};
use crate::sbi::console_getchar;
use crate::task::suspend_current_and_run_next;
use lazy_static::*;
use spin::Mutex;
use crate::task::get_core_id;

pub struct Stdin;

pub struct Stdout;

lazy_static!{
    pub static ref STDOUTLOCK:Mutex<usize> = Mutex::new(0);
    pub static ref STDINLOCK:Mutex<usize> = Mutex::new(0);
}

impl File for Stdin {
    fn readable(&self) -> bool { true }
    fn writable(&self) -> bool { false }
    fn read(&self, mut user_buf: UserBuffer) -> usize {
        assert_eq!(user_buf.len(), 1);
        let lock = STDINLOCK.lock();
        // busy loop
        let mut c: usize;
        loop {
            c = console_getchar();
            if c == 0 {
                suspend_current_and_run_next();
                continue;
            } else {
                break;
            }
        }
        let ch = c as u8;
        unsafe { user_buf.buffers[0].as_mut_ptr().write_volatile(ch); }
        1
    }
    fn write(&self, _user_buf: UserBuffer) -> usize {
        panic!("Cannot write to stdin!");
    }
}

impl File for Stdout {
    fn readable(&self) -> bool { false }
    fn writable(&self) -> bool { true }
    fn read(&self, _user_buf: UserBuffer) -> usize{
        panic!("Cannot read from stdout!");
    }
    fn write(&self, user_buf: UserBuffer) -> usize {
        let lock = STDOUTLOCK.lock();
        for buffer in user_buf.buffers.iter() {
            print!("{}", core::str::from_utf8(*buffer).unwrap());
        }
        user_buf.len()
    }
}