use crate::lang_items::Bytes;

  /* Per-process CPU limit, in seconds.  */
 pub const RLIMIT_CPU:i32 = 0;

  /* Largest file that can be created, in bytes.  */
 pub const RLIMIT_FSIZE:i32 = 1;

  /* Maximum size of data segment, in bytes.  */
 pub const RLIMIT_DATA:i32 = 2;

  /* Maximum size of stack segment, in bytes.  */
 pub const RLIMIT_STACK:i32 = 3;

  /* Largest core file that can be created, in bytes.  */
 pub const RLIMIT_CORE:i32 = 4;

  /* Largest resident set size, in bytes.
     This affects swapping; processes that are exceeding their
     resident set size will be more likely to have physical memory
     taken from them.  */
 pub const RLIMIT_RSS:i32 = 5;

  /* Number of open files.  */
 pub const RLIMIT_NOFILE:i32 = 7;

  /* Address space limit.  */
 pub const RLIMIT_AS:i32 = 9;

  /* Number of processes.  */
 pub const RLIMIT_NPROC:i32 = 6;

  /* Locked-in-memory address space.  */
 pub const RLIMIT_MEMLOCK:i32 = 8;

  /* Maximum number of file locks.  */
 pub const RLIMIT_LOCKS:i32 = 10;

  /* Maximum number of pending signals.  */
 pub const RLIMIT_SIGPENDING:i32 = 11;

  /* Maximum bytes in POSIX message queues.  */
 pub const RLIMIT_MSGQUEUE:i32 = 12;

  /* Maximum nice priority allowed to raise to.
     Nice levels 19 .. -20 correspond to 0 .. 39
     values of this resource limit.  */
 pub const RLIMIT_NICE:i32 = 13;

  /* Maximum realtime priority allowed for non-priviledged
     processes.  */
 pub const RLIMIT_RTPRIO:i32 = 14;

  /* Maximum CPU time in microseconds that a process scheduled under a real-time
     scheduling policy may consume without making a blocking system
     call before being forcibly descheduled.  */
 pub const RLIMIT_RTTIME:i32 = 15;

 pub const RLIMIT_NLIMITS:i32 = 16; //?


/*
    The soft limit is the value that the kernel enforces for the
    corresponding resource.  The hard limit acts as a ceiling for the
    soft limit: an unprivileged process may set only its soft limit
    to a value in the range from 0 up to the hard limit, and
    (irreversibly) lower its hard limit.
*/

#[derive(Clone,Copy)]
pub struct RLimit {
    rlim_cur:i64, // Soft limit
    rlim_max:i64, // Hard limit
}

impl RLimit {
    pub fn new()->Self {
        Self{
            rlim_cur:-1,
            rlim_max:-1,
        }
    }

    pub fn set_cur(&mut self, rlim_cur:i64){
        self.rlim_cur = rlim_cur;
    }

    pub fn set_max(&mut self, rlim_max:i64){
        self.rlim_max = rlim_max;
    }

    pub fn get_cur(&mut self)->i64{
        self.rlim_cur
    }

    pub fn get_max(&mut self)->i64{
        self.rlim_max
    }    
}

impl Bytes<RLimit> for RLimit {}


