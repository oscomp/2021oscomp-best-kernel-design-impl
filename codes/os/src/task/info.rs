use core::fmt::{self, Debug, Formatter};
use core::ops::{Add, Sub};
use alloc::collections::{BTreeMap, BinaryHeap};
use alloc::vec::Vec;
use bitflags::*;
use crate::lang_items::Bytes;
use crate::timer::USEC_PER_SEC;

// sys_clone
bitflags!{
    pub struct CloneFlags: usize{
        const SIGCHLD = 17;
        const CLONE_CHILD_CLEARTID = 0x00200000;
        const CLONE_CHILD_SETTID = 0x01000000;
    }
}
// signal
bitflags!{
    pub struct Signals: usize{
        const	SIGHUP		= 1 << ( 1-1);  
        const	SIGINT		= 1 << ( 2-1);  
        const	SIGQUIT		= 1 << ( 3-1);  
        const	SIGILL		= 1 << ( 4-1);  
        const	SIGTRAP		= 1 << ( 5-1);	
        const	SIGABRT		= 1 << ( 6-1);	
        const	SIGIOT		= 1 << ( 6-1);  
        const	SIGBUS		= 1 << ( 7-1);  
        const	SIGFPE		= 1 << ( 8-1);  
        const	SIGKILL		= 1 << ( 9-1);  
        const	SIGUSR1		= 1 << (10-1);	
        const	SIGSEGV		= 1 << (11-1);	
        const	SIGUSR2		= 1 << (12-1);	
        const	SIGPIPE		= 1 << (13-1);	
        const	SIGALRM		= 1 << (14-1);	
        const	SIGTERM		= 1 << (15-1);	
        const	SIGSTKFLT	= 1 << (16-1);	
        const	SIGCHLD		= 1 << (17-1);	
        const	SIGCONT		= 1 << (18-1);	
        const	SIGSTOP		= 1 << (19-1);	
        const	SIGTSTP		= 1 << (20-1);	
        const	SIGTTIN		= 1 << (21-1);	
        const	SIGTTOU		= 1 << (22-1);	
        const	SIGURG		= 1 << (23-1);	
        const	SIGXCPU		= 1 << (24-1);	
        const	SIGXFSZ		= 1 << (25-1);	
        const	SIGVTALRM	= 1 << (26-1);	
        const	SIGPROF		= 1 << (27-1);	
        const	SIGWINCH	= 1 << (28-1);	
        const	SIGIO		= 1 << (29-1);	
    }
}
bitflags!{
    /* Bits in `sa_flags'.  */
    pub struct SaFlags: usize{
        const SA_NOCLDSTOP = 1		   ;     /* Don't send SIGCHLD when children stop.  */
        const SA_NOCLDWAIT = 2		   ;     /* Don't create zombie on child death.  */
        const SA_SIGINFO   = 4		   ;     /* Invoke signal-catching function with three arguments instead of one.  */
        const SA_ONSTACK   = 0x08000000;    /* Use signal stack by using `sa_restorer'. */
        const SA_RESTART   = 0x10000000;    /* Restart syscall on signal return.  */
        const SA_NODEFER   = 0x40000000;    /* Don't automatically block the signal when its handler is being executed.  */
        const SA_RESETHAND = 0x80000000;    /* Reset to SIG_DFL on entry to handler.  */
        const SA_INTERRUPT = 0x20000000;    /* Historical no-op.  */
    }
}
pub const SIGRTMIN:usize = 32;
// sighandler
pub const SIG_DFL:usize = 0;	/* Default action.  */
pub const SIG_IGN:usize = 1;	/* Ignore signal.  */
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

#[derive(Copy, Clone)]
pub struct TimeVal{
    pub sec: usize,
    pub usec: usize,
}


#[derive(Copy, Clone)]
pub struct ITimerVal{
    pub it_interval: TimeVal, /* Interval for periodic timer */
    pub it_value: TimeVal,    /* Time until next expiration */
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

#[derive(Clone)]
pub struct SigAction {
    pub sa_handler:usize,
    // pub sa_sigaction:usize,
    pub sa_mask:Vec<Signals>,
    pub sa_flags:SaFlags,
}


#[derive(Clone)]
pub struct SigInfo{
    pub is_signal_execute: bool, // is process now executing in signal handler
    pub signal_pending: BinaryHeap<Signals>,
    pub signal_handler: BTreeMap<Signals,SigAction>,
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
            sysname: utsname::str2u8("Linux"),
            nodename: utsname::str2u8("debian"),
            release: utsname::str2u8("5.10.0-7-riscv64"),
            version: utsname::str2u8("#1 SMP Debian 5.10.40-1 (2021-05-28)"),
            machine: utsname::str2u8("riscv64"),
            domainname: utsname::str2u8(""),
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

impl SigAction{
    pub fn new() -> Self{
        Self{
            sa_handler: 0,
            sa_flags: SaFlags::from_bits(0).unwrap(),
            sa_mask: Vec::new(),
        }
    }

    pub fn is_null(&self) -> bool{
        self.sa_handler==0 && self.sa_flags.is_empty() && self.sa_mask.is_empty()
    }
}

impl SigInfo{
    pub fn new() -> Self{
        Self{
            is_signal_execute: false,
            signal_pending: BinaryHeap::new(),
            signal_handler: BTreeMap::new(),
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

impl Debug for ITimerVal {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.write_fmt(format_args!("( it_interval:{:?}, it_value:{:?})", self.it_interval, self.it_value))
    }
}

impl Debug for RUsage {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.write_fmt(format_args!("( ru_utime:{:?}, ru_stime:{:?})", self.ru_utime, self.ru_stime))
    }
}

impl Debug for SigAction {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.write_fmt(format_args!("( sa_handler:0x{:X}, sa_mask:{:?}, sa_flags:{:?})", self.sa_handler,  self.sa_mask,  self.sa_flags ))
    }
}

impl Debug for SigInfo {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.write_fmt(format_args!("( is_signal_execute:{}, signal_pending:{:?}, signal_handler:{:?})", self.is_signal_execute,  self.signal_pending,  self.signal_handler ))
    }
}
