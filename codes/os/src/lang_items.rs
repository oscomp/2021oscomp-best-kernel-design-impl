use core::panic::PanicInfo;
use crate::sbi::shutdown;

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    if let Some(location) = info.location() {
        println!("[kernel] Panicked at {}:{} {}", location.file(), location.line(), info.message().unwrap());
    } else {
        println!("[kernel] Panicked: {}", info.message().unwrap());
    }
    shutdown()
}

#[macro_export]
macro_rules! color_text {
    ($text:expr, $color:expr) => {{
        format_args!("\x1b[{}m{}\x1b[0m", $color, $text)
    }};
}