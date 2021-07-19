use super::File;
use crate::mm::{UserBuffer};
use crate::sbi::console_getchar;
use crate::task::suspend_current_and_run_next;
use k210_hal::{clock::Clocks, fpioa, pac, prelude::*};
use lazy_static::*;
use spin::Mutex;

//use crate::task::get_core_id;

// 这个模块的两个宏应该公开
// 如果制造实例的时候，给定了stdout，那么就会打印到这个stdout里面
use embedded_hal::serial::{Read, Write};
use nb::block;


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
        //assert_eq!(user_buf.len(), 1);
        let lock = STDINLOCK.lock();
        // busy loop
        let mut c: usize;
        let mut count = 0;
        if user_buf.len() > 1{
            return 0;
        }
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
        unsafe { 
            user_buf.buffers[0].as_mut_ptr().write_volatile(ch); 
            //user_buf.write_at(count, ch);
        }
        return 1
        /* 
        loop {
            if count == user_buf.len(){
                break;
            }
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
            if ch as char == '\n' {
                break;
            }
            unsafe { 
                //user_buf.buffers[0].as_mut_ptr().write_volatile(ch); 
                user_buf.write_at(count, ch);
            }
            count += 1;
        }
        count*/
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


/// Legacy standard input/output
pub trait LegacyStdio: Send {
    /// Get a character from legacy stdin
    fn getchar(&mut self) -> u8;
    /// Put a character into legacy stdout
    fn putchar(&mut self, ch: u8);
}

/// Use serial in `embedded-hal` as legacy standard input/output
struct EmbeddedHalSerial<T> {
    inner: T,
}

impl<T> EmbeddedHalSerial<T> {
    /// Create a wrapper with a value
    fn new(inner: T) -> Self {
        Self { inner }
    }
}

impl<T: Send> LegacyStdio for EmbeddedHalSerial<T>
where
    T: Read<u8> + Write<u8>,
{
    fn getchar(&mut self) -> u8 {
        // 直接调用embedded-hal里面的函数
        // 关于unwrap：因为这个是legacy函数，这里没有详细的处理流程，就panic掉
        block!(self.inner.try_read()).ok().unwrap()
    }

    fn putchar(&mut self, ch: u8) {
        // 直接调用函数写一个字节
        block!(self.inner.try_write(ch)).ok();
        // 写一次flush一次，因为是legacy，就不考虑效率了
        block!(self.inner.try_flush()).ok();
    }
}

struct Fused<T, R>(T, R);

// 和上面的原理差不多，就是分开了
impl<T, R> LegacyStdio for Fused<T, R>
where
    T: Write<u8> + Send + 'static,
    R: Read<u8> + Send + 'static,
{
    fn getchar(&mut self) -> u8 {
        block!(self.1.try_read()).ok().unwrap()
    }

    fn putchar(&mut self, ch: u8) {
        block!(self.0.try_write(ch)).ok();
        block!(self.0.try_flush()).ok();
    }
}

use alloc::boxed::Box;

lazy_static::lazy_static! {
    static ref LEGACY_STDIO: Mutex<Option<Box<dyn LegacyStdio>>> =
        Mutex::new(None);
}


#[cfg(feature = "board_qemu")]
pub fn init(){
    let serial = crate::drivers::Ns16550a::new(0x10000000, 0 /*, 11_059_200, 115200*/);
    init_legacy_stdio_embedded_hal(serial);
}

#[cfg(feature = "board_k210")]
pub fn init(){
    //let serial = crate::drivers::Ns16550a::new(0x10000000, 0 /*, 11_059_200, 115200*/);
    println!("0");
    let p = pac::Peripherals::take().unwrap();
    println!("1");
    let mut sysctl = p.SYSCTL.constrain();
    println!("2");
    let fpioa = p.FPIOA.split(&mut sysctl.apb0);
    println!("3");
    let clocks = Clocks::new();
    println!("4");
    let _uarths_tx = fpioa.io5.into_function(fpioa::UARTHS_TX);
    let _uarths_rx = fpioa.io4.into_function(fpioa::UARTHS_RX);
    // Configure UART
    println!("5");
    let serial = p.UARTHS.configure(115_200.bps(), &clocks);
    let (tx, rx) = serial.split();
    println!("6");
    init_legacy_stdio_embedded_hal_fuse(tx, rx);
}

#[doc(hidden)] // use through a macro
pub fn init_legacy_stdio_embedded_hal<T: Read<u8> + Write<u8> + Send + 'static>(serial: T) {
    let serial = EmbeddedHalSerial::new(serial);
    *LEGACY_STDIO.lock() = Some(Box::new(serial));
}

#[doc(hidden)] // use through a macro
pub fn init_legacy_stdio_embedded_hal_fuse<T, R>(tx: T, rx: R)
where
    T: Write<u8> + Send + 'static,
    R: Read<u8> + Send + 'static,
{
    let serial = Fused(tx, rx);
    *LEGACY_STDIO.lock() = Some(Box::new(serial));
}

pub(crate) fn legacy_stdio_putchar(ch: u8) {
    if let Some(stdio) = LEGACY_STDIO.lock().as_mut() {
        stdio.putchar(ch)
    }
}

pub(crate) fn legacy_stdio_getchar() -> u8 {
    if let Some(stdio) = LEGACY_STDIO.lock().as_mut() {
        stdio.getchar()
    } else {
        0 // default: always return 0
    }
}

use core::fmt;

//struct Stdout;

impl fmt::Write for Stdout {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        if let Some(stdio) = LEGACY_STDIO.lock().as_mut() {
            for byte in s.as_bytes() {
                stdio.putchar(*byte)
            }
        }
        Ok(())
    }
}

#[doc(hidden)]
pub fn _print(args: fmt::Arguments) {
    use fmt::Write;
    Stdout.write_fmt(args).unwrap();
}

// Prints to the legacy debug console.
//
// This is only supported when there exists legacy extension; 
// otherwise platform caller should use an early kernel input/output device
// declared in platform specific hardware.
//#[macro_export(local_inner_macros)]
//macro_rules! print {
//    ($($arg:tt)*) => ({
//        $crate::legacy_stdio::_print(core::format_args!($($arg)*));
//    });
//}

// Prints to the legacy debug console, with a newline.
//
// This is only supported when there exists legacy extension; 
// otherwise platform caller should use an early kernel input/output device
// declared in platform specific hardware.
//#[macro_export(local_inner_macros)]
//macro_rules! println {
//    ($fmt: literal $(, $($arg: tt)+)?) => {
//        $crate::legacy_stdio::_print(core::format_args!(core::concat!($fmt, "\n") $(, $($arg)+)?));
//    }
//}
