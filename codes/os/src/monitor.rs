// See detail at /doc/Monitor.md
#[allow(unused)]
use crate::config::{MEMORY_END, PAGE_SIZE};


// GDB debug pin

// select open channel 
pub const QEMU:usize = 0; // 1: open in qemu mode, 0: close in real world
pub const MEMORY_GDB_START:usize  = MEMORY_END - PAGE_SIZE;
pub const PROCESSOR_ENABLE:usize  = MEMORY_GDB_START + 0;
pub const EXIT_ENABLE:usize       = MEMORY_GDB_START + 1;
pub const EXEC_ENABLE:usize       = MEMORY_GDB_START + 2;
pub const SYSCALL_ENABLE:usize       = MEMORY_GDB_START + 3;
pub const TIMER_ENABLE:usize       = MEMORY_GDB_START + 4;
pub const FORK_ENABLE:usize       = MEMORY_GDB_START + 5;
pub const MAP_ENABLE:usize        = MEMORY_GDB_START + 6;
pub const SIGNAL_ENABLE:usize        = MEMORY_GDB_START + 7;
pub const BLANK_ENABLE:usize        = MEMORY_GDB_START + 8;


// always open channel
// pub const PROCESSOR_ENABLE:usize  = 1;
// pub const EXIT_ENABLE:usize       = 1;
//pub const EXEC_ENABLE:usize       = 1;
//pub const SYSCALL_ENABLE:usize    = 1; //always open
// pub const TIMER_ENABLE:usize   = 1;
// pub const SIGNAL_ENABLE:usize        = 1;

#[macro_export]
macro_rules! gdb_print {
    ($place:literal, $fmt: literal $(, $($arg: tt)+)?) => {
        unsafe{
            let enable:*mut u8 =  $place;
            if ($place == 1 )||(*enable > 0 && QEMU == 1){
                print!($fmt $(, $($arg)+)?);
            }
        }
    };

    ($place:expr, $fmt: literal $(, $($arg: tt)+)?) => {
        unsafe{
            let enable:*mut u8 =  $place as *mut u8;
            if ($place == 1 )||(*enable > 0 && QEMU == 1){
                print!($fmt $(, $($arg)+)?);
            }
        }
    };
}

#[macro_export]
macro_rules! gdb_println {
    ($place:literal, $fmt: literal $(, $($arg: tt)+)?) => {
        unsafe{
            let enable:*mut u8 =  $place;
            if ($place == 1 )||(*enable > 0 && QEMU == 1){
                println!($fmt $(, $($arg)+)?);
            }
        }
    };

    ($place:expr, $fmt: literal $(, $($arg: tt)+)?) => {
        unsafe{
            let enable:*mut u8 =  $place as *mut u8;
            if ($place == 1 )||(*enable > 0 && QEMU == 1){
                println!($fmt $(, $($arg)+)?);
            }
        }
    };
}