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
pub const DT_UNKNOWN:u8 = 0;
pub const DT_DIR:u8 = 4;
pub const DT_REG:u8 = 4; //常规文件

pub const NAME_LIMIT:usize = 16; // TODO:太大了会有跨页问题。。

#[repr(C)]
#[derive(Debug)]
pub struct Dirent {
    pub d_ino: usize,
    pub d_off: isize,   //到下一个dirent的偏移？？？/ 在目录中的偏移
    pub d_reclen: u16,  //当前dirent长度？？？/ 文件名的长度
    pub d_type: u8,     
    pub d_name: [u8;NAME_LIMIT],
}

impl Dirent {
    pub fn empty()->Self{
        Self{
            d_ino:0,
            d_off:0,
            d_reclen:0,
            d_type:0,
            d_name:[0;NAME_LIMIT],
        }
    }

    pub fn new(name:&str, inode:usize, offset:isize, reclen: u16, d_type: u8)->Self{
        let mut dirent = Self{
            d_ino:inode,
            d_off:offset,
            d_reclen:reclen,
            d_type,
            d_name:[0;NAME_LIMIT],
        };
        dirent.fill_name(name);
        dirent
    }

    pub fn fill_info(&mut self, name:&str, inode:usize, offset:isize, reclen: u16, d_type: u8) {
        *self = Self {
            d_ino:inode,
            d_off:offset,
            d_reclen:reclen,
            d_type,
            d_name:self.d_name,
        };
        self.fill_name(name);
    }

    fn fill_name(&mut self, name:&str) {
        let len = name.len().min(NAME_LIMIT);
        let name_bytes = name.as_bytes();
        for i in 0..len {
            self.d_name[i] = name_bytes[i];
        }
    }
}

#[repr(C)]
struct Kstat {
	
}