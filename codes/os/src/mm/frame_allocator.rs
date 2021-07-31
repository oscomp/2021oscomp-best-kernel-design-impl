use super::{PhysAddr, PhysPageNum};
use alloc::vec::Vec;
use spin::Mutex;
use crate::config::MEMORY_END;
use lazy_static::*;
use core::fmt::{self, Debug, Formatter};
use alloc::collections::BTreeMap;

#[derive(Clone)]
pub struct FrameTracker {
    pub ppn: PhysPageNum,
}

impl FrameTracker {
    pub fn new(ppn: PhysPageNum) -> Self {
        // page cleaning
        let bytes_array = ppn.get_bytes_array();
        for i in bytes_array {
            *i = 0;
        }
        Self { ppn }
    }
    pub fn from_ppn(ppn: PhysPageNum) -> Self {
        Self { ppn }
    }
}

impl Debug for FrameTracker {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.write_fmt(format_args!("FrameTracker:PPN={:#x}", self.ppn.0))
    }
}

impl Drop for FrameTracker {
    fn drop(&mut self) {
        frame_dealloc(self.ppn);
    }
}

trait FrameAllocator {
    fn new() -> Self;
    fn alloc(&mut self) -> Option<PhysPageNum>;
    fn dealloc(&mut self, ppn: PhysPageNum);
    fn add_ref(&mut self, ppn: PhysPageNum);
    fn enquire_ref(&self, ppn: PhysPageNum) -> usize;
}

pub struct StackFrameAllocator {
    current: usize,
    end: usize,
    recycled: Vec<usize>,
    refcounter: BTreeMap<usize, u8>,
}

impl StackFrameAllocator {
    pub fn init(&mut self, l: PhysPageNum, r: PhysPageNum) {
        self.current = l.0;
        self.end = r.0;
        println!("last {} Physical Frames.", self.end - self.current);
    }

    pub fn add_free(&mut self, ppn: usize){
        self.recycled.push(ppn);
    }

    pub fn print_free(&mut self){
        let size = self.recycled.len() + self.end - self.current;
        println!("Free memory: {} pages", size);
    }
}
impl FrameAllocator for StackFrameAllocator {
    fn new() -> Self {
        Self {
            current: 0,
            end: 0,
            recycled: Vec::new(),
            refcounter: BTreeMap::new(),
        }
    }
    fn alloc(&mut self) -> Option<PhysPageNum> {
        if let Some(ppn) = self.recycled.pop() {
            // println!{"alloced recycled ppn: {:X}", ppn}
            self.refcounter.insert(ppn, 1);
            Some(ppn.into())
        } else {
            if self.current == self.end {
                None
            } else {
                // println!{"alloced ppn: {:X}", self.current}
                self.current += 1;
                self.refcounter.insert(self.current - 1, 1);
                Some((self.current - 1).into())
            }
        }
    }
    fn dealloc(&mut self, ppn: PhysPageNum) {
        let ppn = ppn.0; 
        // if self.refcounter.contains_key(&ppn) {
        // let no_ref = false;
        let ref_times = self.refcounter.get_mut(&ppn).unwrap();
        *ref_times -= 1;
        // println!{"the refcount of {:X} decrease to {}", ppn, ref_times}
        if *ref_times == 0 {
            self.refcounter.remove(&ppn);
            // println!{"dealloced ppn: {:X}", ppn}
            // validity check
            if ppn >= self.current || self.recycled
                .iter()
                .find(|&v| {*v == ppn})
                .is_some() {
                // panic!("Frame ppn={:#x} has not been allocated!", ppn);
            }
            // recycle
            self.recycled.push(ppn);
        }
    }
    fn add_ref(&mut self, ppn: PhysPageNum) {
        let ppn = ppn.0; 
        let ref_times = self.refcounter.get_mut(&ppn).unwrap();
        *ref_times += 1;
    }
    fn enquire_ref(&self, ppn: PhysPageNum) -> usize{
        let ppn = ppn.0; 
        let ref_times = self.refcounter.get(&ppn).unwrap();
        (*ref_times).clone() as usize
    }
}

type FrameAllocatorImpl = StackFrameAllocator;

lazy_static! {
    pub static ref FRAME_ALLOCATOR: Mutex<FrameAllocatorImpl> =
        Mutex::new(FrameAllocatorImpl::new());
}

pub fn init_frame_allocator() {
    extern "C" {
        fn ekernel();
    }
    FRAME_ALLOCATOR
        .lock()
        .init(PhysAddr::from(ekernel as usize).ceil(), PhysAddr::from(MEMORY_END).floor());
}

pub fn frame_alloc() -> Option<FrameTracker> {
    FRAME_ALLOCATOR
        .lock()
        .alloc()
        .map(|ppn| FrameTracker::new(ppn))
}

pub fn frame_add_ref(ppn: PhysPageNum) {
    FRAME_ALLOCATOR
        .lock()
        .add_ref(ppn)
}

pub fn frame_alloc_raw() -> Option<PhysPageNum> {
    FRAME_ALLOCATOR
        .lock()
        .alloc()
}

pub fn frame_dealloc(ppn: PhysPageNum) {
    FRAME_ALLOCATOR
        .lock()
        .dealloc(ppn);
}

pub fn enquire_refcount(ppn: PhysPageNum) -> usize {
    FRAME_ALLOCATOR
        .lock()
        .enquire_ref(ppn)
}
pub fn add_free(ppn: usize){
    FRAME_ALLOCATOR.lock().recycled.push(ppn);
}

pub fn print_free_pages(){
    FRAME_ALLOCATOR.lock().print_free();
}

#[allow(unused)]
pub fn frame_allocator_test() {
    let mut v: Vec<FrameTracker> = Vec::new();
    for i in 0..5 {
        let frame = frame_alloc().unwrap();
        println!("{:?}", frame);
        v.push(frame);
    }
    v.clear();
    for i in 0..5 {
        let frame = frame_alloc().unwrap();
        println!("{:?}", frame);
        v.push(frame);
    }
    drop(v);
    println!("frame_allocator_test passed!");
}
