use crate::{drivers::BLOCK_DEVICE, println};
use crate::color_text;
use _core::usize;
use alloc::sync::Arc;
use lazy_static::*;
use bitflags::*;
use alloc::vec::Vec;
use spin::Mutex;
use super::{File, Dirent, Kstat, DT_DIR, DT_REG, DT_UNKNOWN};
use crate::mm::UserBuffer;
use simple_fat32::{ATTRIBUTE_ARCHIVE, ATTRIBUTE_DIRECTORY, FAT32Manager, VFile};

#[derive(PartialEq,Copy,Clone,Debug)]
pub enum DiskInodeType {
    File,
    Directory,
}

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

    pub fn getdirent(&self, dirent: &mut Dirent, offset:isize)->isize {
        let inner = self.inner.lock();
        if let Some((name, offset, first_clu,attri)) 
        = inner.inode.dirent_info(offset as usize){
            let mut d_type:u8 = 0;
            if attri & ATTRIBUTE_DIRECTORY != 0 {
                d_type = DT_DIR;
            } else if attri & ATTRIBUTE_ARCHIVE != 0 {
                d_type = DT_REG;
            } else {
                d_type = DT_UNKNOWN;
            }
            dirent.fill_info(
                name.as_str(), 
                first_clu as usize, 
                offset as isize, 
                name.len() as u16, 
                d_type
            );
            (name.len() + 8*4) as isize
        } else {
            -1
        }
    }

    pub fn get_fstat(&self, kstat:&mut Kstat){
        let inner = self.inner.lock();
        let vfile = inner.inode.clone();
        let (size, atime, mtime, ctime) = vfile.stat();
        //println!("info = {:?}", (size, atime, mtime, ctime));
        kstat.fill_info(
            0, 
            233, 
            0, 
            1, 
            size, 
            atime, 
            mtime, 
            ctime
        );

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
            inode.clear();
            Some(Arc::new(OSInode::new(
                readable,
                writable,
                inode,
            )))
        } else {
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
    
    pub fn set_offset(& self, off:usize){
        let mut inner = self.inner.lock();
        inner.offset = off;
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
    //let curr_inode = EasyFileSystem::get_inode(&ROOT_INODE.get_fs(), inode_id);
    //println!("enter list files");
    let work_inode = {
        if work_path == "/" || (path.len()>0 && path.chars().nth(0).unwrap() == '/') {
            //println!("curr is root");
            ROOT_INODE.clone()
        } else {
            let wpath:Vec<&str> = work_path.split('/').collect();
            ROOT_INODE.find_vfile_bypath( wpath ).unwrap()
        }
    };
    //println!("path  = {}, len = {}", path, path.len());
    let mut pathv:Vec<&str> = path.split('/').collect();
    //println!("pathv.len = {}", path.len());
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
    println!("");
}

bitflags! {
    pub struct OpenFlags: u32 {
        const RDONLY = 0;
        const WRONLY = 1 << 0;
        const RDWR = 1 << 1;
        const CREATE = 1 << 6;
        const TRUNC = 1 << 10;
        const DIRECTROY = 0x0200000;
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

//pub fn find_par_inode_id(path: &str) -> u32{
//    let mut pathv:Vec<&str> = path.split('/').collect();
//    pathv.pop();
//    let inode = ROOT_INODE.find_vfile_bypath(pathv).unwrap();
//    //println!("find par ok");
//    inode.get_id()
//}

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
    //println!("pathv = {:?}", pathv);
    print!("\n");
    // shell应当保证此处输入的path不为空
    let (readable, writable) = flags.read_write();
    if flags.contains(OpenFlags::CREATE) {
        if let Some(inode) = cur_inode.find_vfile_bypath(pathv.clone()) {
            // clear size
            //println!("clear size");
            inode.clear();
            Some(Arc::new(OSInode::new(
                readable,
                writable,
                inode,
            )))
        } else {
            // create file
            //println!("start create");
            let name = pathv.pop().unwrap();
            //println!("name = {}", name);
            print!("\n");
            if let Some(temp_inode) = cur_inode.find_vfile_bypath(pathv.clone()){
                let attribute = {
                    match type_ {
                        DiskInodeType::Directory=>{ ATTRIBUTE_DIRECTORY }
                        DiskInodeType::File=>{ ATTRIBUTE_ARCHIVE }
                    }
                };
                print!("\n");
                temp_inode.create( name, attribute)
                .map(|inode| {
                    print!("\n");
                    //println!("end create");
                    Arc::new(OSInode::new(
                        readable,
                        writable,
                        inode,
                    ))
                })
            }else{
                // println!("cannot find pah");
                None
            }
        }
    } else {
        //println!("pathv = {:?}", pathv);
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
    //println!("enter cd");
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
    //println!("in cd, pathv = {:?}", pathv);
    if let Some(tar_dir) = cur_inode.find_vfile_bypath(pathv){
        // ! 当inode_id > 2^16 时，有溢出的可能（目前不会发生。。
        0
    }else{
        -1
    }
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