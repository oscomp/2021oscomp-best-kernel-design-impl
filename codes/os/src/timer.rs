use riscv::register::time;
use crate::sbi::set_timer;
use crate::config::CLOCK_FREQ;

const TICKS_PER_SEC: usize = 100;
const MSEC_PER_SEC: usize = 1000;
const USEC_PER_SEC: usize = 1000000;

pub fn get_time() -> usize {
    time::read()
}

pub fn get_time_us() -> usize {
    time::read() / (CLOCK_FREQ / USEC_PER_SEC) 
}

pub fn get_time_ms() -> usize {
    time::read() / (CLOCK_FREQ / MSEC_PER_SEC)
}

pub fn get_time_s() -> usize {
    time::read() / CLOCK_FREQ
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