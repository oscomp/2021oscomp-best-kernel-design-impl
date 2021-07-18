use super::{VirtAddr, UserBuffer, translated_byte_buffer};
use crate::fs::{File, FileClass};
use crate::task::FdTable;
use alloc::sync::{Arc};
use alloc::vec::Vec;

bitflags! {
    pub struct MmapProts: usize {
        const PROT_NONE = 0;
        const PROT_READ = 1;
        const PROT_WRITE = 2;
        const PROT_EXEC = 4;
        const PROT_GROWSDOWN = 0x01000000;
        const PROT_GROWSUP = 0x02000000;
    }
}

bitflags! {
    pub struct MmapFlags: usize {
        const MAP_FILE = 0;
        const MAP_SHARED= 0x01;
        const MAP_PRIVATE = 0x02;
        const MAP_FIXED = 0x10;
        const MAP_ANONYMOUS = 0x20;
    }
}

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
                fd: isize, offset: usize, fd_table: FdTable, token: usize) -> usize {
        
        let start_addr = start.into();

        let mut mmap_space = MmapSpace::new(start_addr, len, prot, flags, 0, fd);
        mmap_space.map_file(start_addr, len, offset, fd_table, token);
        // println!{"The start addr is {:X}", start_addr.0};

        self.mmap_set.push(Arc::new(mmap_space));

        // update mmap_top
        if self.mmap_top == start_addr{
            self.mmap_top = (start_addr.0 + len).into();
        }

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
    pub fd: isize,
}

impl MmapSpace{
    pub fn new(
        oaddr: VirtAddr,
        length: usize,
        prot: usize,
        flags: usize,
        valid: usize,
        fd: isize
    ) -> Self {
        Self {oaddr, length, prot, flags, valid, fd}
    }

    pub fn map_file(&mut self, va_start: VirtAddr, len: usize, offset: usize, fd_table: FdTable, token: usize) -> isize {
        let flags = MmapFlags::from_bits(self.flags).unwrap();
        // print!("map_file: va_strat:0x{:X} flags:{:?}",va_start.0, flags);
        if flags.contains(MmapFlags::MAP_ANONYMOUS)
            && self.fd == -1 
            && offset == 0{
            // print!("[map_anonymous_file]");
            return 1;
        }
        
        if self.fd as usize >= fd_table.len() { return -1; }

        if let Some(file) = &fd_table[self.fd as usize] {
            match &file.fclass {
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