//use crate::{drivers::BLOCK_DEVICE, println};
//use crate::color_text;
//use alloc::sync::Arc;
//use lazy_static::*;
//use bitflags::*;
//use alloc::vec::Vec;
//use spin::Mutex;
//use super::File;
//use crate::mm::UserBuffer;
//use simple_fat32::{ATTRIBUTE_ARCHIVE, ATTRIBUTE_DIRECTORY, FAT32Manager, VFile};

#[repr(C)]
pub struct Dirent {
    pub d_ino: usize,
    pub d_off: isize,
    pub d_reclen: u16, 
    pub d_type: u8,
    pub d_name: *mut u8,
}

#[repr(C)]
struct Kstat {
	
}