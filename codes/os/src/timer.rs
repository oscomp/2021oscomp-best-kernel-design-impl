use riscv::register::time;
use crate::sbi::set_timer;
use crate::config::CLOCK_FREQ;
use crate::task::TimeVal;

pub const TICKS_PER_SEC: usize = 100;
pub const MSEC_PER_SEC: usize = 1000;
pub const USEC_PER_SEC: usize = 1000_000;
pub const NSEC_PER_SEC: usize = 1000_000_000;

pub fn get_time() -> usize {
    let mut time:usize = 0;
    unsafe{
        asm!(
            "rdtime a0",
            inout("a0") time
        );
    }
    time
}

pub fn get_timeval() -> TimeVal {
    let ticks = get_time();
    let sec = ticks/CLOCK_FREQ;
    let usec = (ticks%CLOCK_FREQ) * USEC_PER_SEC / CLOCK_FREQ;
    TimeVal{
        sec:sec,
        usec:usec
    }
}

pub fn get_time_ns() -> usize {
    (get_time() / (CLOCK_FREQ / USEC_PER_SEC)) * MSEC_PER_SEC
}

pub fn get_time_us() -> usize {
    get_time() / (CLOCK_FREQ / USEC_PER_SEC) 
}

pub fn get_time_ms() -> usize {
    get_time() / (CLOCK_FREQ / MSEC_PER_SEC)
}

pub fn get_time_s() -> usize {
    get_time() / CLOCK_FREQ
}

pub fn sum_time(t_sec:usize, t_usec:usize, f_sec:usize, f_usec:usize) -> (usize,usize){
    let mut sum_usec = t_usec + f_usec;
    let mut sum_sec = t_sec + f_sec;
    if sum_usec >= USEC_PER_SEC{
        sum_usec -= USEC_PER_SEC;
        sum_sec += 1;
    } 
    (sum_sec, sum_usec)
}

pub fn compare_time(t_sec:usize, t_usec:usize, f_sec:usize, f_usec:usize) -> bool{
    // Compare sec
    if t_sec > f_sec{
        return true;
    }
    else if t_sec < f_sec{
        return false;
    } 
    //Compare usec
    if t_usec > f_usec{
        return true;
    }
    else if t_sec < f_usec{
        return false;
    } 
    return true;
}


pub fn set_next_trigger() {
    set_timer(get_time() + CLOCK_FREQ / TICKS_PER_SEC);
}