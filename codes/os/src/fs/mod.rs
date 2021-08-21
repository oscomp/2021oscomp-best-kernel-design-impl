mod pipe;
pub mod stdio;
mod inode;
mod mount;
pub mod finfo;
mod iovec;
mod dev_fs;

use crate::mm::UserBuffer;
use alloc::sync::Arc; 

#[derive(Clone)]
pub struct FileDescripter{
    cloexec: bool,
    pub fclass: FileClass,
}

impl FileDescripter {
    pub fn new(cloexec:bool, fclass:FileClass)->Self{
        Self{
            cloexec,
            fclass
        }
    }

    pub fn set_cloexec(&mut self, flag: bool){
        self.cloexec = flag;
    }

    pub fn get_cloexec(& self) -> bool{
        self.cloexec
    }
}

#[derive(Clone)]
pub enum FileClass {
    File (Arc<OSInode>),
    Abstr (Arc<dyn File + Send + Sync>),
}

pub trait File : Send + Sync {
    fn readable(&self) -> bool;
    fn writable(&self) -> bool;
    fn read(&self, buf: UserBuffer) -> usize;
    fn write(&self, buf: UserBuffer) -> usize;
    fn ioctl(&self, cmd: u32, arg: usize)-> isize {0}
    fn r_ready(&self)->bool{true}
    fn w_ready(&self)->bool{true}

}

pub use mount::MNT_TABLE;
pub use finfo::{Dirent, Kstat, NewStat, FdSet,  DT_DIR, DT_REG, DT_UNKNOWN, *};
pub use iovec::{IoVec, IoVecs};
pub use pipe::{Pipe, make_pipe};
pub use dev_fs::*;
pub use stdio::{Stdin, Stdout, _print};
pub use inode::{OSInode, open, clear_cache, init_rootfs, OpenFlags, list_apps, /*find_par_inode_id, */ch_dir, list_files,  DiskInodeType};