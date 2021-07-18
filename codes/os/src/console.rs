use core::fmt::{self, Write};
use crate::sbi::console_putchar;

struct Stdout;


impl Write for Stdout {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        for c in s.chars() {
            console_putchar(c as usize);
        }
        Ok(())
    }
}

pub fn print(args: fmt::Arguments) {
    Stdout.write_fmt(args).unwrap();
}



//#[cfg(feature = "board_qemu")]
#[macro_export]
macro_rules! print {
    ($fmt: literal $(, $($arg: tt)+)?) => {
        $crate::console::print(format_args!($fmt $(, $($arg)+)?));
        //$crate::fs::_print(format_args!($fmt $(, $($arg)+)?));
    }
}

//#[cfg(feature = "board_qemu")]
#[macro_export]
macro_rules! println {
    ($fmt: literal $(, $($arg: tt)+)?) => {
        $crate::console::print(format_args!(concat!($fmt, "\n") $(, $($arg)+)?));
        //$crate::fs::_print(format_args!(core::concat!($fmt, "\n") $(, $($arg)+)?));
    }
}

/* 
#[cfg(feature = "board_k210")]
#[macro_export]
macro_rules! print {
    ($fmt: literal $(, $($arg: tt)+)?) => {
        $crate::console::print(format_args!($fmt $(, $($arg)+)?));
        //$crate::fs::_print(format_args!($fmt $(, $($arg)+)?));
    }
}

#[cfg(feature = "board_k210")]
#[macro_export]
macro_rules! println {
    ($fmt: literal $(, $($arg: tt)+)?) => {
        $crate::console::print(format_args!(concat!($fmt, "\n") $(, $($arg)+)?));
        //$crate::fs::_print(format_args!(core::concat!($fmt, "\n") $(, $($arg)+)?));
    }
}*/


