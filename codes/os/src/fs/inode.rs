use crate::monitor::SYSCALL_ENABLE;
use crate::{drivers::BLOCK_DEVICE, println};
use crate::color_text;
use _core::usize;
use alloc::sync::Arc;
use lazy_static::*;
use bitflags::*;
use alloc::vec::Vec;
use alloc::string::String;
use spin::Mutex;
use super::{DT_DIR, DT_REG, DT_UNKNOWN, Dirent, File, Kstat, NewStat, finfo};
use crate::mm::UserBuffer;
use simple_fat32::{ATTRIBUTE_ARCHIVE, ATTRIBUTE_DIRECTORY, FAT32Manager, VFile};
//use crate::config::*;
//use crate::gdb_println;

pub const SEEK_SET:i32 = 0;  /* set to offset bytes.  */
pub const SEEK_CUR:i32 = 1;  /* set to its current location plus offset bytes.  */
pub const SEEK_END:i32 = 2;  /* set to the size of the file plus offset bytes.  */
/*  Adjust the file offset to the next location in the file
    greater than or equal to offset containing data.  If
    offset points to data, then the file offset is set to
    offset */
pub const SEEK_DATA:i32 = 3;
/*  Adjust the file offset to the next hole in the file
    greater than or equal to offset.  If offset points into
    the middle of a hole, then the file offset is set to
    offset.  If there is no hole past offset, then the file
    offset is adjusted to the end of the file (i.e., there is
    an implicit hole at the end of any file). */
pub const SEEK_HOLE:i32 = 4;  


#[derive(PartialEq,Copy,Clone,Debug)]
pub enum DiskInodeType {
    File,
    Directory,
}

// 此inode实际被当作文件
pub struct OSInode {
    readable: bool,
    writable: bool,
    //fd_cloexec: bool,
    inner: Mutex<OSInodeInner>,
}

pub struct OSInodeInner {
    offset: usize, // 当前读写的位置
    inode: Arc<VFile>, // inode引用
}

impl OSInode {
    pub fn new(
        readable: bool,
        writable: bool,
        inode: Arc<VFile>,
    ) -> Self {
        Self {
            readable,
            writable,
            //fd_cloexec:false,
            inner: Mutex::new(OSInodeInner {
                offset: 0,
                inode,
            }),
        }
    }

    pub fn is_dir(&self)->bool{
        let inner = self.inner.lock();
        inner.inode.is_dir()
    }
    
    /* this func will not influence the file offset 
    * @parm: if offset == -1, file offset will be used
    */
    pub fn read_vec(&self, offset:isize, len:usize)->Vec<u8>{
        let mut inner = self.inner.lock();
        let mut len = len;
        let ori_off = inner.offset;
        if offset >= 0 {
            inner.offset = offset as usize;
        }
        let mut buffer = [0u8; 512];
        let mut v: Vec<u8> = Vec::new();
        loop {
            let rlen = inner.inode.read_at(inner.offset, &mut buffer);
            if rlen == 0 {
                break;
            }
            inner.offset += rlen;
            v.extend_from_slice(&buffer[..rlen.min(len)]);
            if len > rlen {
                len -= rlen;
            } else {
                break;
            }
        }
        if offset >= 0 {
            inner.offset = ori_off; 
        }
        v

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

    pub fn write_all(&self, str_vec:&Vec<u8>)->usize{
        let mut inner = self.inner.lock();
        let mut remain = str_vec.len();
        let mut base = 0;
        loop {
            let len = remain.min(512);
            inner.inode.write_at(inner.offset, &str_vec.as_slice()[base .. base + len]);
            inner.offset += len;
            base += len;
            remain -= len;
            if remain == 0{
                break;
            }
        }
        return base
    }

    pub fn find(&self, path:&str, flags:OpenFlags)->Option<Arc<OSInode>>{
        let inner = self.inner.lock();
        let mut pathv:Vec<&str> = path.split('/').collect();
        let vfile = inner.inode.find_vfile_bypath(pathv);
        if vfile.is_none(){
            return None
        } else {
            let (readable, writable) = flags.read_write();
            return Some(Arc::new(OSInode::new(
                readable,
                writable,
                vfile.unwrap()
            )))
        }
    }

    pub fn getdirent(&self, dirent: &mut Dirent /*, offset:isize*/)->isize {
        let mut inner = self.inner.lock();
        let offset = inner.offset as u32;
        if let Some((name, off, first_clu,attri)) 
        = inner.inode.dirent_info(offset as usize){
            let mut d_type:u8 = 0;
            if attri & ATTRIBUTE_DIRECTORY != 0 {
                d_type = DT_DIR;
            } else if attri & ATTRIBUTE_ARCHIVE != 0 {
                d_type = DT_REG;
            } else {
                d_type = DT_UNKNOWN;
            }
            //println!("name = {}", name.as_str());
            dirent.fill_info(
                name.as_str(), 
                first_clu as usize, 
                (off - offset) as isize, 
                name.len() as u16, 
                d_type
            );
            inner.offset = off as usize; 
            let len = (name.len() + 8*4) as isize;
            len
        } else {
            -1
        }
    }

    pub fn get_fstat(&self, kstat:&mut Kstat){
        let inner = self.inner.lock();
        let vfile = inner.inode.clone();
        let (size, atime, mtime, ctime, ino) = vfile.stat();
        let st_mod:u32 = {
            if vfile.is_dir() {
                //println!("is dir");
                finfo::S_IFDIR | finfo::S_IRWXU | finfo::S_IRWXG | finfo::S_IRWXO
            } else {
                finfo::S_IFREG | finfo::S_IRWXU | finfo::S_IRWXG | finfo::S_IRWXO
            }
        };
        kstat.fill_info(
            0, 
            ino, 
            st_mod, 
            1, 
            size, 
            atime, 
            mtime, 
            ctime
        );
    }

    pub fn get_newstat(&self, stat:&mut NewStat){
        let inner = self.inner.lock();
        let vfile = inner.inode.clone();
        let (size, atime, mtime, ctime, ino) = vfile.stat();
        let st_mod:u32 = {
            if vfile.is_dir() {
                finfo::S_IFDIR | finfo::S_IRWXU | finfo::S_IRWXG | finfo::S_IRWXO
            } else {
                finfo::S_IFREG | finfo::S_IRWXU | finfo::S_IRWXG | finfo::S_IRWXO
            }
        };
        stat.fill_info(
            0, 
            ino, 
            st_mod, 
            1, 
            size, 
            atime, 
            mtime, 
            ctime
        );
    }

    pub fn get_size(&self)->usize{
        let inner = self.inner.lock();
        let (size, _, mt_me, _, _) = inner.inode.stat();
        return size as usize
    }

    pub fn create(&self, path:&str, type_: DiskInodeType)->Option<Arc<OSInode>>{
        let inner = self.inner.lock();
        let cur_inode = inner.inode.clone();
        if !cur_inode.is_dir(){
            println!("[create]:{} is not a directory!", path);
            return None;
        }
        let mut pathv:Vec<&str> = path.split('/').collect();
        let (readable, writable) = (true, true);
        if let Some(inode) = cur_inode.find_vfile_bypath(pathv.clone()) {
            // already exists, clear
            inode.remove();
        }  
        {
            // create file
            let name = pathv.pop().unwrap();
            if let Some(temp_inode) = cur_inode.find_vfile_bypath(pathv.clone()){
                let attribute = {
                    match type_ {
                        DiskInodeType::Directory=>{ ATTRIBUTE_DIRECTORY }
                        DiskInodeType::File=>{ ATTRIBUTE_ARCHIVE }
                    }
                };
                temp_inode.create( name, attribute)
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
    }

    pub fn clear(&self){
        let inner = self.inner.lock();
        inner.inode.clear();
    }

    pub fn delete(&self)->usize{
        let inner = self.inner.lock();
        inner.inode.remove()
    }

    pub fn set_head_cluster(&self, cluster:u32) {
        let inner = self.inner.lock();
        let vfile = &inner.inode;
        vfile.set_first_cluster(cluster);
    }    

    pub fn get_head_cluster(&self)->u32 {
        let inner = self.inner.lock();
        let vfile = &inner.inode;
        vfile.first_cluster()
    }
    
    pub fn set_delete_bit(&self) {
        let inner = self.inner.lock();
        inner.inode.set_delete_bit();
    }

    pub fn set_offset(& self, off:usize){
        let mut inner = self.inner.lock();
        inner.offset = off;
    }

    pub fn lseek(&self, offset: isize, whence: i32)->isize{
        let mut inner = self.inner.lock();
        if whence == SEEK_END {
            if inner.offset as isize - offset < 0 {
                return -1;
            }
        } else {
            if offset < 0{
                return -1;
            }
        }
        
        match whence{
            SEEK_CUR=>{
                inner.offset += offset as usize;
            },   
            SEEK_END=>{
                let size = inner.inode.get_size();
                inner.offset = (size as isize + offset - 1) as usize;
            },
            SEEK_SET=>{
                inner.offset = offset as usize;
            },
            _ => return -1,
        }
        inner.offset as isize
    }
}

lazy_static! {
    // 通过ROOT_INODE可以实现对efs的操作
    pub static ref ROOT_INODE: Arc<VFile> = {
        // 此处载入文件系统
        let fat32_manager = FAT32Manager::open(BLOCK_DEVICE.clone());
        let manager_reader = fat32_manager.read();
        Arc::new( manager_reader.get_root_vfile(& fat32_manager) )
    };
}
/* 
lazy_static! {
    // 目录栈 
    pub static ref DIR_STACK: Vec<Arc<Inode>> = vec![ROOT_INODE.clone()];
}
*/

pub fn init_rootfs(){
    println!("[fs] build rootfs ... start");
    println!("[fs] build rootfs: creating /proc");
    let file = open("/","proc", OpenFlags::CREATE, DiskInodeType::Directory).unwrap();
    println!("[fs] build rootfs: init /proc");
    let file = open("/proc","mounts", OpenFlags::CREATE, DiskInodeType::File).unwrap();
    let meminfo = open("/proc","meminfo", OpenFlags::CREATE, DiskInodeType::File).unwrap();
    let file = open("/","ls", OpenFlags::CREATE, DiskInodeType::File).unwrap();
    println!("[fs] build rootfs ... finish");
}


pub fn list_apps() {
    println!("/**** APPS ****");
    for app in ROOT_INODE.ls_lite().unwrap() {
        if app.1 & ATTRIBUTE_DIRECTORY == 0 {
            println!("{}", app.0);
        }
    }
    println!("**************/")
}

// TODO: 对所有的Inode加锁！
// 在这一层实现互斥访问
pub fn list_files(work_path: &str, path: &str){
    let work_inode = {
        if work_path == "/" || (path.len()>0 && path.chars().nth(0).unwrap() == '/') {
            //println!("curr is root");
            ROOT_INODE.clone()
        } else {
            let wpath:Vec<&str> = work_path.split('/').collect();
            ROOT_INODE.find_vfile_bypath( wpath ).unwrap()
        }
    };
    let mut pathv:Vec<&str> = path.split('/').collect();
    let cur_inode = work_inode.find_vfile_bypath(pathv).unwrap();

    let mut file_vec = cur_inode.ls_lite().unwrap();
    file_vec.sort();
    for i in 0 .. file_vec.len() {
        if file_vec[i].1 & ATTRIBUTE_DIRECTORY != 0 {
            println!("{}  ", color_text!(file_vec[i].0, 96));
        } else {
            // TODO: 统一配色！
            println!("{}  ", file_vec[i].0);
        }
        
    }
}

bitflags! {
    pub struct OpenFlags: u32 {
        const RDONLY = 0;
        const WRONLY = 1 << 0;
        const RDWR = 1 << 1;
        const CREATE = 1 << 6;
        const TRUNC = 1 << 10;
        const DIRECTROY = 0200000;
        const LARGEFILE  = 0100000;
        const CLOEXEC = 02000000;
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


pub fn open(work_path: &str, path: &str, flags: OpenFlags, type_: DiskInodeType) -> Option<Arc<OSInode>> {
    // DEBUG: 相对路径
    let cur_inode = {
        if work_path == "/" {
            ROOT_INODE.clone()
        } else {
            let wpath:Vec<&str> = work_path.split('/').collect();
            ROOT_INODE.find_vfile_bypath( wpath ).unwrap()
        }
    };
    let mut pathv:Vec<&str> = path.split('/').collect();
    //println!("[open] pathv = {:?}", pathv);
    // print!("\n");
    // shell应当保证此处输入的path不为空
    let (readable, writable) = flags.read_write();
    if flags.contains(OpenFlags::CREATE) {
        if let Some(inode) = cur_inode.find_vfile_bypath(pathv.clone()) {
            // clear size
            inode.remove();
        } 
        {
            // create file
            let name = pathv.pop().unwrap();
            if let Some(temp_inode) = cur_inode.find_vfile_bypath(pathv.clone()){
                let attribute = {
                    match type_ {
                        DiskInodeType::Directory=>{ ATTRIBUTE_DIRECTORY }
                        DiskInodeType::File=>{ ATTRIBUTE_ARCHIVE }
                    }
                };
                temp_inode.create( name, attribute)
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
        cur_inode.find_vfile_bypath(pathv)
            .map(|inode| {
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


pub fn ch_dir(work_path: &str, path: &str) -> isize{
    // 切换工作路径
    // 切换成功，返回inode_id，否则返回-1
    let cur_inode = {
        if work_path == "/" || ( path.len() > 0 && path.chars().nth(0).unwrap() == '/' ) {
            ROOT_INODE.clone()
        } else {
            let wpath:Vec<&str> = work_path.split('/').collect();
            //println!("in cd, work_pathv = {:?}", wpath);
            ROOT_INODE.find_vfile_bypath( wpath ).unwrap()
        }
    };
    let pathv:Vec<&str> = path.split('/').collect();
    if let Some(tar_dir) = cur_inode.find_vfile_bypath(pathv){
        // ! 当inode_id > 2^16 时，有溢出的可能（目前不会发生。。
        0
    }else{
        -1
    }
}

pub fn clear_cache(){
    ROOT_INODE.clear_cache();
}

// TODO: 不急 
// 复制文件/目录
//pub fn fcopy(src_inode_id: u32, src_path: &str, dst_inode_id: u32, dst_path: &str )->bool{
//    let spathv:Vec<&str> = src_path.split('/').collect();
//    let dpathv:Vec<&str> = dst_path.split('/').collect();
//    let src_ino = EasyFileSystem::get_inode(&ROOT_INODE.get_fs(), src_inode_id);
//    src_ino.fcopy(spathv, dst_inode_id,dpathv)
//}

// 移动文件/目录
//pub fn fmove(src_inode_id: u32, src_path: &str, dst_inode_id: u32, dst_path: &str )->bool{
//    let spathv:Vec<&str> = src_path.split('/').collect();
//    let dpathv:Vec<&str> = dst_path.split('/').collect();
//    let src_ino = EasyFileSystem::get_inode(&ROOT_INODE.get_fs(), src_inode_id);
//    src_ino.fmove(spathv, dst_inode_id,dpathv)
//}

// pub fn remove(inode_id: u32, path: &str, type_: DiskInodeType)->bool{
//     // type_确认要删除的文件类型，如果是目录，递归删除
//     let curr_inode = EasyFileSystem::get_inode(
//         &ROOT_INODE.get_fs().clone(), 
//         inode_id
//     );
//     let pathv:Vec<&str> = path.split('/').collect();
//     curr_inode.remove(pathv,type_)
// }

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
        //println!("ino_write");
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