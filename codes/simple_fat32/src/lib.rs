#![no_std]
#![feature(llvm_asm)]
extern crate alloc;


mod block_dev;
mod layout;
mod fat32_manager;
mod vfs;
mod block_cache;
#[macro_use]
mod console;
mod sbi;

pub const BLOCK_SZ:usize = 512;
pub use block_dev::BlockDevice;
pub use vfs::VFile;
pub use layout::ShortDirEntry;
//pub use layout::NAME_LENGTH_LIMIT;
pub use fat32_manager::FAT32Manager;
pub use layout::*;
use block_cache::{get_block_cache,get_info_cache,write_to_dev,set_start_sec, CacheMode};
/*
pub trait w_field {
    fn set(&self, value:Self);
}

impl w_field for u32 {
    fn set(&mut self, value:Self){
        *self = Self
    }
}*/

pub fn clone_into_array<A, T>(slice: &[T]) -> A
where
    A: Default + AsMut<[T]>,
    T: Clone,
{
    let mut a = Default::default();
    <A as AsMut<[T]>>::as_mut(&mut a).clone_from_slice(slice);
    a
}