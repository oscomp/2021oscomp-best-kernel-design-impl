use core::fmt::{self, Debug, Formatter};
use core::ops::{Add, Sub};
use bitflags::*;
use crate::timer::USEC_PER_SEC;

// sys_clone
bitflags!{
    pub struct CloneFlags: usize{
        const SIGCHLD = 17;
        const CLONE_CHILD_CLEARTID = 0x00200000;
        const CLONE_CHILD_SETTID = 0x01000000;
    }
}
// sys_getrusage
pub const RUSAGE_SELF:isize = 0; /* The calling process.  */
pub const RUSAGE_CHILDREN:isize = -1; /* All of its terminated child processes.  */
pub const RUSAGE_THREAD:isize = 1;   /* The calling thread.  */
// sys_setitimer
pub const ITIMER_REAL:isize = 0; /* Timers run in real time.  */
pub const ITIMER_VIRTUAL:isize = 1; /* Timers run only when the process is executing.  */
pub const ITIMER_PROF:isize = 2; /* Timers run when the process is executing and when the system is executing on behalf of the process.  */



pub struct utsname{
    sysname: [u8; 65],
    nodename: [u8; 65],
    release: [u8; 65],
    version: [u8; 65],
    machine: [u8; 65],
    domainname: [u8; 65],
}

pub struct TimeVal{
    sec: usize,
    usec: usize,
}

pub struct ITimerVal{
    it_interval: TimeVal, /* Interval for periodic timer */
    it_value: TimeVal,    /* Time until next expiration */
}

// Resource usage of process
pub struct RUsage{
    ru_utime   :TimeVal,      /* user CPU time used */
    ru_stime   :TimeVal,     /* system CPU time used */
    ru_maxrss  :isize  ,      // NOT IMPLEMENTED /* maximum resident set size */
    ru_ixrss   :isize  ,      // NOT IMPLEMENTED /* integral shared memory size */
    ru_idrss   :isize   ,     // NOT IMPLEMENTED /* integral unshared data size */
    ru_isrss   :isize  ,      // NOT IMPLEMENTED /* integral unshared stack size */
    ru_minflt  :isize  ,      // NOT IMPLEMENTED /* page reclaims (soft page faults) */
    ru_majflt  :isize  ,      // NOT IMPLEMENTED /* page faults (hard page faults) */
    ru_nswap   :isize  ,      // NOT IMPLEMENTED /* swaps */
    ru_inblock :isize  ,      // NOT IMPLEMENTED /* block input operations */
    ru_oublock :isize  ,      // NOT IMPLEMENTED /* block output operations */
    ru_msgsnd  :isize  ,      // NOT IMPLEMENTED /* IPC messages sent */
    ru_msgrcv  :isize  ,      // NOT IMPLEMENTED /* IPC messages received */
    ru_nsignals:isize  ,      // NOT IMPLEMENTED /* signals received */
    ru_nvcsw   :isize  ,      // NOT IMPLEMENTED /* voluntary context switches */
    ru_nivcsw  :isize  ,      // NOT IMPLEMENTED /* involuntary context switches */
}



impl utsname{
    pub fn new() -> Self{
        Self{
            //sysname: utsname::str2u8("UltraOS"),
            //nodename: utsname::str2u8("UltraOS"),
            //release: utsname::str2u8("5.10.0-7-riscv64"),
            //version: utsname::str2u8("1.1"),
            //machine: utsname::str2u8("RISC-V64"),
            //domainname: utsname::str2u8("UltraTEAM/UltraOS"),
            sysname: utsname::str2u8("UltraOS"),
            nodename: utsname::str2u8("UltraOS"),
            release: utsname::str2u8("5.10.0-7-riscv64"),
            version: utsname::str2u8("1.1"),
            machine: utsname::str2u8("RISC-V64"),
            domainname: utsname::str2u8("UltraTEAM/UltraOS"),
        }
    }

    fn str2u8(str: &str) -> [u8;65]{
        let mut arr:[u8;65] = [0;65];
        let str_bytes = str.as_bytes();
        let len = str.len();
        for i in 0..len{
            arr[i] = str_bytes[i];
        }
        arr
    }
    
    pub fn as_bytes(&self) -> &[u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts(
                self as *const _ as usize as *const u8,
                size,
            )
        }
    }
}

impl TimeVal{
    pub fn new() -> Self{
        Self{
            sec:0,
            usec:0
        }
    }

    pub fn add_usec(&mut self, usec:usize){
        self.usec += usec;
        self.sec += self.usec/1000_000;
        self.usec %= 1000_000;
    }

    pub fn is_zero(&self) -> bool{
        self.sec == 0 && self.usec == 0
    }

}

impl ITimerVal{
    pub fn new() -> Self{
        Self{
            it_interval: TimeVal::new(),
            it_value: TimeVal::new(),
        }
    }

    pub fn is_zero(&self) -> bool{
        self.it_interval.is_zero() && self.it_value.is_zero()
    }
    
    pub fn as_bytes(&self) -> &[u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts(
                self as *const _ as usize as *const u8,
                size,
            )
        }
    }
}

impl RUsage{
    pub fn new() -> Self{
        Self{
            ru_utime   :TimeVal::new(),
            ru_stime   :TimeVal::new(),
            ru_maxrss  :0,
            ru_ixrss   :0,
            ru_idrss   :0,
            ru_isrss   :0,
            ru_minflt  :0,
            ru_majflt  :0,
            ru_nswap   :0,
            ru_inblock :0,
            ru_oublock :0,
            ru_msgsnd  :0,
            ru_msgrcv  :0,
            ru_nsignals:0,
            ru_nvcsw   :0,
            ru_nivcsw  :0,
        }
    }

    pub fn add_utime(&mut self, usec: usize){
        self.ru_utime.add_usec(usec);
    }

    pub fn add_stime(&mut self, usec: usize){
        self.ru_stime.add_usec(usec);
    }

    pub fn as_bytes(&self) -> &[u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts(
                self as *const _ as usize as *const u8,
                size,
            )
        }
    }

}

impl Add for TimeVal {
    type Output = Self;

    fn add(self, other: Self) -> Self {
        let mut sec = self.sec + other.sec;
        let mut usec = self.usec + other.usec;
        sec += usec/USEC_PER_SEC;
        usec %= USEC_PER_SEC;
        Self {
            sec: sec,
            usec: usec,
        }
    }
}


// if self is less than other, then return 0
impl Sub for TimeVal {
    type Output = Self;

    fn sub(self, other: Self) -> Self {
        if self.sec < other.sec{
            return Self{sec:0,usec:0}
        }
        else if self.sec == other.sec{
            if self.usec < other.usec{
                return Self{sec:0,usec:0}
            }
            else{
                return Self{sec:0,usec:self.usec-other.usec}
            }
        }
        else{
            let mut sec = self.sec - other.sec;
            let mut usec = self.usec - other.usec;
            if self.usec < other.usec{
                sec -= 1;
                usec = USEC_PER_SEC + self.usec - other.usec;
            }
            Self {
                sec: sec,
                usec: usec,
            }
        }
    }
}

impl Debug for TimeVal {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.write_fmt(format_args!("( sec:{}, usec:{})", self.sec, self.usec))
    }
}

impl Debug for RUsage {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.write_fmt(format_args!("( ru_utime:{:?}, ru_stime:{:?})", self.ru_utime, self.ru_stime))
    }
}