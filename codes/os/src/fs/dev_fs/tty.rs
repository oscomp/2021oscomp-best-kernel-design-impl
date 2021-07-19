use spin::{Mutex, RwLock};
use alloc::collections::VecDeque;
use lazy_static::lazy_static;
use alloc::sync::Arc;
use core::mem::size_of;
use super::{ioctl::Termios};
use crate::fs::File;
use crate::mm::{
    copy_from_user,
    copy_to_user
};
use crate::mm::UserBuffer;
use super::ioctl::*;

lazy_static! {
    pub static ref TTY: Arc<TtyINode> = Arc::new(TtyINode::default());
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct Winsize {
     ws_row: u16,
     ws_col: u16,
     xpixel: u16,
     ypixel: u16,
}

impl Default for Winsize{
    fn default() -> Self {
        Self{
            ws_row:24,
            ws_col:80,
            xpixel:0,
            ypixel:0,
        }
    }
}


#[derive(Default)]
pub struct TtyINode {
    /// foreground process group
    foreground_pgid: RwLock<i32>,
    buf: Mutex<VecDeque<u8>>,
    //eventbus: Mutex<EventBus>,
    winsize: RwLock<Winsize>,
    termios: RwLock<Termios>,
}

// TODO: independ of rust sbi
impl File for TtyINode {
    fn readable(&self) -> bool {
        false
    }
    
    fn writable(&self) -> bool {
        false
    }
    
    fn read(&self, buf: UserBuffer) -> usize {
        0
    }
    
    fn write(&self, buf: UserBuffer) -> usize {
        0
    }

    fn ioctl(&self, cmd: u32, arg: usize)->isize{
        //println!("ioctl: cmd={}; arg={:X}", cmd, arg);
        let cmd = cmd as usize;
        match cmd {
            TIOCGPGRP => {
                //let argp = arg as *mut i32; // pid_t
                let argp =  *self.foreground_pgid.read();
                copy_to_user(arg, &argp as *const i32, 4);
                0
            }
            TIOCSPGRP => {
                //let fpgid = unsafe { *(arg as *const i32) };
                let mut argp:i32 = 0;
                copy_from_user(&mut argp as *mut i32, arg, 4);
                *self.foreground_pgid.write() = argp;
                0
            }
            TIOCGWINSZ => {
                let winsize = Winsize::default();
                let size = size_of::<Winsize>();
                //println!("size = {}", size);
                copy_to_user(arg, &winsize as *const Winsize, size);
                0
            }
            TCGETS => {
                let termois  = *self.termios.read();
                let size = size_of::<Termios>();
                copy_to_user(arg, &termois as *const Termios, size);
                0
            }
            TCSETS => {
                let mut termios = Termios::default();
                let size = size_of::<Termios>();
                copy_from_user(&mut termios as *mut Termios, arg, size);
                *self.termios.write() = termios;
                0
            }
            /* WARNING: 仅临时handle */
            RTC_RD_TIME=> {
                0
            }
            _ => -1,// not support
        }
    }

}