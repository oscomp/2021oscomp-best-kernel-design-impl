use super::{VirtAddr, UserBuffer, translated_byte_buffer};
use crate::fs::{File, FileClass};
use alloc::sync::{Arc};
use alloc::vec::Vec;

pub struct MmapArea {
    pub mmap_start: VirtAddr,
    pub mmap_top: VirtAddr,
    pub mmap_set: Vec<Arc<MmapSpace>>,
}

impl MmapArea {
    pub fn new(
        mmap_start: VirtAddr,
        mmap_top: VirtAddr
    ) -> Self {
        Self {
            mmap_start,
            mmap_top,
            mmap_set: Vec::new(),
        }
    }

    pub fn get_mmap_top(&mut self) -> VirtAddr { self.mmap_top }

    pub fn push(&mut self, start: usize, len: usize, prot: usize, flags: usize,
                fd: usize, offset: usize, fd_table: Vec<Option<FileClass>>, token: usize) -> usize {
        if start != 0 {
            panic!{"The start arg is not NULL!"};
        }
        let start_addr = self.get_mmap_top();

        let mut mmap_space = MmapSpace::new(start_addr, len, prot, flags, 0, fd);
        mmap_space.map_file(start_addr, len, offset, fd_table, token);
        self.mmap_set.push(Arc::new(mmap_space));

        // println!{"The start addr is {}", start_addr.0};
        start_addr.0
    }

    pub fn remove(&mut self, start: usize, len: usize) -> isize {
        let pair = self.mmap_set.iter().enumerate()
            .find(|(_, p)| { p.oaddr.0 == start });
        if let Some((idx, _)) = pair {
            if self.mmap_top == VirtAddr::from(start + len) {
                self.mmap_top = VirtAddr::from(start);
            }
            self.mmap_set.remove(idx);
            0
        } else {
            panic!{"No matched Mmap Space!"}
        }
    }
}

pub struct MmapSpace {
    // pub addr: VirtAddr,
    pub oaddr: VirtAddr,
    pub valid: usize,
    // pub length: usize,
    pub length: usize,
    pub prot: usize,
    pub flags: usize,
    pub fd: usize,
}

impl MmapSpace{
    pub fn new(
        oaddr: VirtAddr,
        length: usize,
        prot: usize,
        flags: usize,
        valid: usize,
        fd: usize
    ) -> Self {
        Self {oaddr, length, prot, flags, valid, fd}
    }

    pub fn map_file(&mut self, va_start: VirtAddr, len: usize, offset: usize, fd_table: Vec<Option<FileClass>>, token: usize) -> isize {
        if self.fd >= fd_table.len() { return -1; }

        if let Some(file) = &fd_table[self.fd] {
            match file {
                FileClass::File(f)=>{
                    f.set_offset(offset);
                    if !f.readable() { return -1; }
                    // println!{"The va_start is {}", self.oaddr.0};
                    f.read(UserBuffer::new(translated_byte_buffer(token, self.oaddr.0 as *const u8, len)));
                },
                _ => { return -1; },
            };
        } else { return -1 };
        return 1;
    }
}