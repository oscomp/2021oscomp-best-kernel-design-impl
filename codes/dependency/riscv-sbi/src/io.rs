//! IO operations to the debug console.

use crate::legacy;
use core::fmt::{self, Write};
use spin::Mutex;

struct Stdout;

impl fmt::Write for Stdout {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        for byte in s.as_bytes() {
            legacy::console_putchar(*byte as usize);
        }
        Ok(())
    }
}

/// Print format struct to the debug console.
pub fn print_fmt(args: fmt::Arguments) {
    static STDOUT: Mutex<Stdout> = Mutex::new(Stdout);
    STDOUT.lock().write_fmt(args).unwrap();
}

/// Prints to the debug console.
#[macro_export]
macro_rules! print {
    ($($arg:tt)*) => ({
        $crate::io::print_fmt(format_args!($($arg)*));
    });
}

/// Prints to the debug console, with a newline.
#[macro_export]
macro_rules! println {
    () => ($crate::print!("\n"));
    ($($arg:tt)*) => ($crate::print!("{}\n", format_args!($($arg)*)));
}
