// 获取/设置相应终端termios 结构中的信息
pub const TCGETS: usize = 0x5401;
pub const TCSETS: usize = 0x5402;

// 获取/设置进程组和tty的关系
pub const TIOCGPGRP: usize = 0x540F;
pub const TIOCSPGRP: usize = 0x5410;

// 获取窗体大小
pub const TIOCGWINSZ: usize = 0x5413;

// File IOctl Close on Exec
// 对文件设置专用标志，通知内核当exec()系统调用发生时自动关闭打开的文件。
pub const FIOCLEX: usize = 0x5451; 
// 清除由FIOCLEX设置的标志
pub const FIONCLEX: usize = 0x5450; 

// rustc using pipe and ioctl pipe file with this request id
// for non-blocking/blocking IO control setting
pub const FIONBIO: usize = 0x5421;

pub const RTC_RD_TIME:usize = 0x80247009;


/*  The termios functions describe a general terminal interface that
    is provided to control asynchronous communications ports. 
*/
#[repr(C)]
#[derive(Clone, Copy)]
pub struct Termios {
    pub iflag: u32,  // input modes
    pub oflag: u32,  // ouput modes
    pub cflag: u32,  // control modes
    pub lflag: u32,  // local modes
    pub line: u8,
    pub cc: [u8; 32], // terminal special characters.
    pub ispeed: u32,
    pub ospeed: u32,
}

impl Default for Termios {
    fn default() -> Self {
        Termios {
            // IMAXBEL | IUTF8 | IXON | IXANY | ICRNL | BRKINT
            iflag: 0o66402,
            // OPOST | ONLCR
            oflag: 0o5,
            // HUPCL | CREAD | CSIZE | EXTB
            cflag: 0o2277,
            // IEXTEN | ECHOTCL | ECHOKE ECHO | ECHOE | ECHOK | ISIG | ICANON
            lflag: 0o105073,
            line: 0,
            cc: [
                3,   // VINTR Ctrl-C
                28,  // VQUIT
                127, // VERASE
                21,  // VKILL
                4,   // VEOF Ctrl-D
                0,   // VTIME
                1,   // VMIN
                0,   // VSWTC
                17,  // VSTART
                19,  // VSTOP
                26,  // VSUSP Ctrl-Z
                255, // VEOL
                18,  // VREPAINT
                15,  // VDISCARD
                23,  // VWERASE
                22,  // VLNEXT
                255, // VEOL2
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            ],
            ispeed: 0,
            ospeed: 0,
        }
    }
}

bitflags! {
    pub struct LocalModes : u32 {
        const ISIG = 0o000001;
        const ICANON = 0o000002;
        const ECHO = 0o000010;
        const ECHOE = 0o000020;
        const ECHOK = 0o000040;
        const ECHONL = 0o000100;
        const NOFLSH = 0o000200;
        const TOSTOP = 0o000400;
        const IEXTEN = 0o100000;
        const XCASE = 0o000004;
        const ECHOCTL = 0o001000;
        const ECHOPRT = 0o002000;
        const ECHOKE = 0o004000;
        const FLUSHO = 0o010000;
        const PENDIN = 0o040000;
        const EXTPROC = 0o200000;
    }
}

/* FIOCLEX */
pub fn f_close_on_exec(){

}

pub fn f_nclose_on_exec(){

}
