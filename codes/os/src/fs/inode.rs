use easy_fs::{
    EasyFileSystem,
    Inode,
    DiskInodeType,
    //NAME_LENGTH_LIMIT,
};
use crate::drivers::BLOCK_DEVICE;
use crate::color_text;
use alloc::sync::Arc;
use lazy_static::*;
use bitflags::*;
use alloc::vec::Vec;
use alloc::vec;
use spin::Mutex;
use super::File;
use crate::mm::UserBuffer;

#[derive(PartialEq,Copy,Clone)]
pub enum SeekWhence{
    SeekSet, // 将offset设为新的读写位置 
    SeekCur, // 将当前读写位置往后增加offset个偏移量
    SeekEnd, // 将读写位置设为末尾，然后增加offset偏移量(此时offset可以<0)
}

// 此inode实际被当作文件
pub struct OSInode {
    readable: bool,
    writable: bool,
    inner: Mutex<OSInodeInner>,
}

pub struct OSDirEntry {
    inode_id: u32, 
    offset: usize,//dirent在目录中的偏移量
}

pub struct OSInodeInner {
    offset: usize, // 当前读写的位置
    inode: Arc<Inode>, // inode引用
}

impl OSInode {
    pub fn new(
        readable: bool,
        writable: bool,
        inode: Arc<Inode>,
    ) -> Self {
        Self {
            readable,
            writable,
            inner: Mutex::new(OSInodeInner {
                offset: 0,
                inode,
            }),
        }
    }
    
    pub fn read_all(&self) -> Vec<u8> {
        let mut inner = self.inner.lock();
        let mut buffer = [0u8; 512];
        let mut v: Vec<u8> = Vec::new();
        loop {
            let len = inner.inode.read_at(inner.offset, &mut buffer);
            if len == 0 {
                break;
            }
            inner.offset += len;
            v.extend_from_slice(&buffer[..len]);
        }
        v
    }

    // pub fn lseek(&self, offset: isize, whence: SeekWhence)->isize{
    //     let inner = self.inner.lock();
    //     if whence == SeekWhence::SEEK_END {
    //         if inner.offset as isize - offset < 0 {
    //             return -1;
    //         }
    //     } else {
    //         if offset < 0{
    //             return -1;
    //         }
    //     }
    //     match whence{
    //         SeekWhence::SEEK_CUR=>{
    //             inner.offset += offset as usize;
    //         }   
    //         SeekWhence::SEEK_END=>{
    //             let size = inner.inode.get_size();
    //             inner.offset = (size as isize + offset - 1) as usize;
    //         }
    //         SeekWhence::SEEK_SET=>{
    //             inner.offset = offset as usize;
    //         }
    //     }
    //     inner.offset as isize
    // }
}

lazy_static! {
    // 通过ROOT_INODE可以实现对efs的操作
    pub static ref ROOT_INODE: Arc<Inode> = {
        // 此处载入文件系统
        let efs = EasyFileSystem::open(BLOCK_DEVICE.clone());
        Arc::new(EasyFileSystem::get_inode(&efs, 0))
    };
}

lazy_static! {
    // 目录栈 
    pub static ref DIR_STACK: Vec<Arc<Inode>> = vec![ROOT_INODE.clone()];
}

pub fn list_apps() {
    println!("/**** APPS ****");
    for app in ROOT_INODE.ls() {
        println!("{}", app.0);
    }
    println!("**************/")
}

// TODO: 对所有的Inode加锁！
// 在这一层实现互斥访问
pub fn list_files(inode_id: u32){
    let curr_inode = EasyFileSystem::get_inode(&ROOT_INODE.get_fs(), inode_id);
    let file_vec = curr_inode.ls();
    for i in 0 .. file_vec.len() {
        if i != 0 && i % 6 == 0{
            println!("");
        }
        if file_vec[i].1 == DiskInodeType::File{
            print!("{}  ", file_vec[i].0);
        } else {
            // TODO: 统一配色！
            print!("{}  ", color_text!(file_vec[i].0, 96));
        }
        
    }
    println!("");
}

bitflags! {
    pub struct OpenFlags: u32 {
        const RDONLY = 0;
        const WRONLY = 1 << 0;
        const RDWR = 1 << 1;
        const CREATE = 1 << 9;
        const TRUNC = 1 << 10;
    }
}

impl OpenFlags {
    /// Do not check validity for simplicity
    /// Return (readable, writable)
    pub fn read_write(&self) -> (bool, bool) {
        if self.is_empty() {
            (true, false)
        } else if self.contains(Self::WRONLY) {
            (false, true)
        } else {
            (true, true)
        }
    }
}

pub fn find_par_inode_id(path: &str) -> u32{
    let mut pathv:Vec<&str> = path.split('/').collect();
    pathv.pop();
    let (inode,_) = ROOT_INODE.find_path(pathv).unwrap();
    //println!("find par ok");
    inode.get_id()
}


pub fn open(inode_id: u32, path: &str, flags: OpenFlags, type_: DiskInodeType) -> Option<Arc<OSInode>> {
    // DEBUG: 相对路径
    let cur_inode = {
        if inode_id == 0 {
            ROOT_INODE.clone()
        }else{
            Arc::new(EasyFileSystem::get_inode(
                &ROOT_INODE.get_fs(), 
                inode_id
            ))
        }
    };
    let mut pathv:Vec<&str> = path.split('/').collect();
    // shell应当保证此处输入的path不为空
    let (readable, writable) = flags.read_write();
    if flags.contains(OpenFlags::CREATE) {
        if let Some((inode,_)) = cur_inode.find_path(pathv.clone()) {
            // clear size
            inode.clear();
            Some(Arc::new(OSInode::new(
                readable,
                writable,
                inode,
            )))
        } else {
            // create file
            let name = pathv.pop().unwrap();
            if let Some((temp_inode,_)) = cur_inode.find_path(pathv.clone()){
                temp_inode.create( name, type_)
                .map(|inode| {
                    Arc::new(OSInode::new(
                        readable,
                        writable,
                        inode,
                    ))
                })
            }else{
                None
            }
        }
    } else {
        cur_inode.find_path(pathv)
            .map(|(inode, _)| {
                if flags.contains(OpenFlags::TRUNC) {
                    inode.clear();
                }
                Arc::new(OSInode::new(
                    readable,
                    writable,
                    inode
                ))
            })
    }
}


pub fn ch_dir(inode_id: u32, path: &str) -> i32{
    // 切换工作路径
    // 切换成功，返回inode_id，否则返回-1
    let pathv:Vec<&str> = path.split('/').collect();
    let cur_inode = EasyFileSystem::get_inode(
        &ROOT_INODE.get_fs(), 
        inode_id
    );
    if let Some((tar_inode,_)) = cur_inode.find_path(pathv){
        // ! 当inode_id > 2^16 时，有溢出的可能（目前不会发生。。
        tar_inode.get_id() as i32
    }else{
        -1
    }
}
 
// TODO: 不急
/* 
pub fn read_dir(inode_id: u32) -> Option<Arc<OSDirEntry>> {
    // 从目录中读取下一个目录项
}*/

// 复制文件/目录
pub fn fcopy(src_inode_id: u32, src_path: &str, dst_inode_id: u32, dst_path: &str )->bool{
    let spathv:Vec<&str> = src_path.split('/').collect();
    let dpathv:Vec<&str> = dst_path.split('/').collect();
    let src_ino = EasyFileSystem::get_inode(&ROOT_INODE.get_fs(), src_inode_id);
    src_ino.fcopy(spathv, dst_inode_id,dpathv)
}

// 移动文件/目录
pub fn fmove(src_inode_id: u32, src_path: &str, dst_inode_id: u32, dst_path: &str )->bool{
    let spathv:Vec<&str> = src_path.split('/').collect();
    let dpathv:Vec<&str> = dst_path.split('/').collect();
    let src_ino = EasyFileSystem::get_inode(&ROOT_INODE.get_fs(), src_inode_id);
    src_ino.fmove(spathv, dst_inode_id,dpathv)
}

pub fn remove(inode_id: u32, path: &str, type_: DiskInodeType)->bool{
    // type_确认要删除的文件类型，如果是目录，递归删除
    let curr_inode = EasyFileSystem::get_inode(
        &ROOT_INODE.get_fs().clone(), 
        inode_id
    );
    let pathv:Vec<&str> = path.split('/').collect();
    curr_inode.remove(pathv,type_)
}

impl File for OSInode {
    fn readable(&self) -> bool { self.readable }
    fn writable(&self) -> bool { self.writable }
    fn read(&self, mut buf: UserBuffer) -> usize {
        let mut inner = self.inner.lock();
        let mut total_read_size = 0usize;
        for slice in buf.buffers.iter_mut() {
            // buffer存放的元素是[u8]而不是u8
            let read_size = inner.inode.read_at(inner.offset, *slice);
            if read_size == 0 {
                break;
            }
            inner.offset += read_size;
            total_read_size += read_size;
        }
        total_read_size
    }
    fn write(&self, buf: UserBuffer) -> usize {
        let mut inner = self.inner.lock();
        let mut total_write_size = 0usize;
        for slice in buf.buffers.iter() {
            let write_size = inner.inode.write_at(inner.offset, *slice);
            assert_eq!(write_size, slice.len());
            inner.offset += write_size;
            total_write_size += write_size;
        }
        total_write_size
    }
}