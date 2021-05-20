use core::usize;

use crate::{
    fs::{OSInode, FileClass, File}, 
    mm::{
    UserBuffer,
    translated_byte_buffer,
    translated_refmut,
    translated_str,
}};
use crate::task::{current_user_token, current_task, print_core_info};
use crate::fs::{make_pipe, OpenFlags, open, ch_dir, list_files, DiskInodeType};
use alloc::sync::Arc;
use alloc::vec::Vec;
use alloc::string::String;

//use easy_fs::DiskInodeType;
const AT_FDCWD:isize = -100;
const FD_LIMIT:usize = 128;

pub fn sys_write(fd: usize, buf: *const u8, len: usize) -> isize {
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    if fd >= inner.fd_table.len() {
        return -1;
    }
    if let Some(file) = &inner.fd_table[fd] {
        let f: Arc<dyn File + Send + Sync> = match file {
            FileClass::Abstr(f)=> {f.clone()},
            FileClass::File(f)=>{f.clone()},
            _ => return -1,
        };
        if !f.writable() {
            return -1;
        }
        drop(inner);
        f.write(
            UserBuffer::new(translated_byte_buffer(token, buf, len))
        ) as isize
    } else {
        -1
    }
}

pub fn sys_read(fd: usize, buf: *const u8, len: usize) -> isize {
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    if fd >= inner.fd_table.len() {
        return -1;
    }
    if let Some(file) = &inner.fd_table[fd] {
        let file: Arc<dyn File + Send + Sync> = match file {
            FileClass::Abstr(f)=> {f.clone()},
            FileClass::File(f)=>{f.clone()},
            _ => return -1,
        };
        if !file.readable() {
            return -1;
        }
        // release Task lock manually to avoid deadlock
        drop(inner);
        file.read(
            UserBuffer::new(translated_byte_buffer(token, buf, len))
        ) as isize
    } else {
        -1
    }
}

// TODO:文件所有权
pub fn sys_open_at(dirfd: isize, path: *const u8, flags: u32, mode: u32) -> isize {
    let task = current_task().unwrap();
    let token = current_user_token();
    // 这里传入的地址为用户的虚地址，因此要使用用户的虚地址进行映射
    let path = translated_str(token, path);
    let mut inner = task.acquire_inner_lock();
    println!("openat: fd = {}", dirfd);
    if dirfd == AT_FDCWD {
        println!("cwd = {}", inner.get_work_path().as_str());
        if let Some(inode) = open(
            inner.get_work_path().as_str(),
            path.as_str(),
            OpenFlags::from_bits(flags).unwrap(),
            DiskInodeType::File
        ) {
            let fd = inner.alloc_fd();
            inner.fd_table[fd] = Some(FileClass::File(inode));
            fd as isize
        } else {
            -1
        }
    } else {    
        let fd_usz = dirfd as usize;
        if fd_usz >= inner.fd_table.len() && fd_usz > FD_LIMIT {
            return -1
        }
        if let Some(file) = &inner.fd_table[fd_usz] {
            // TODO
            match file {
                FileClass::File(f) => {
                    if let Some(tar_f) = f.find(path.as_str(), OpenFlags::from_bits(flags).unwrap()){
                        let fd = inner.alloc_fd();
                        inner.fd_table[fd] = Some(FileClass::File(tar_f));
                        fd as isize
                    }else{
                        return -1;
                    }
                },
                _ => return -1,
            }
        } else {
            return -1
        }
    }
    
}

// TODO
pub fn sys_open( path: *const u8, flags: u32 ) -> isize {
    let task = current_task().unwrap();
    let token = current_user_token();
    // 这里传入的地址为用户的虚地址，因此要使用用户的虚地址进行映射
    let path = translated_str(token, path);
    let mut inner = task.acquire_inner_lock();
    if let Some(inode) = open(
        inner.get_work_path().as_str(),
        path.as_str(),
        OpenFlags::from_bits(flags).unwrap(),
        DiskInodeType::File
    ) {
        let fd = inner.alloc_fd();
        inner.fd_table[fd] = Some(FileClass::File(inode));
        fd as isize
    } else {
        -1
    }
}

pub fn sys_close(fd: usize) -> isize {
    let task = current_task().unwrap();
    let mut inner = task.acquire_inner_lock();
    if fd >= inner.fd_table.len() {
        return -1;
    }
    if inner.fd_table[fd].is_none() {
        return -1;
    }
    inner.fd_table[fd].take();
    0
}

pub fn sys_pipe(pipe: *mut usize) -> isize {
    let task = current_task().unwrap();
    let token = current_user_token();
    let mut inner = task.acquire_inner_lock();
    let (pipe_read, pipe_write) = make_pipe();
    let read_fd = inner.alloc_fd();
    inner.fd_table[read_fd] = Some(FileClass::Abstr(pipe_read));
    let write_fd = inner.alloc_fd();
    inner.fd_table[write_fd] = Some(FileClass::Abstr(pipe_write));
    *translated_refmut(token, pipe) = read_fd;
    *translated_refmut(token, unsafe { pipe.add(1) }) = write_fd;
    0
}

pub fn sys_dup(fd: usize) -> isize {
    let task = current_task().unwrap();
    let mut inner = task.acquire_inner_lock();
    if fd >= inner.fd_table.len() {
        return -1;
    }
    if inner.fd_table[fd].is_none() {
        return -1;
    }
    let new_fd = inner.alloc_fd();
    inner.fd_table[new_fd] = Some(inner.fd_table[fd].as_ref().unwrap().clone());
    new_fd as isize
}

pub fn sys_chdir(path: *const u8) -> isize{
    //print_core_info();
    let token = current_user_token();
    let task = current_task().unwrap();
    let mut inner = task.acquire_inner_lock();
    let path = translated_str(token, path);
    let mut work_path = inner.current_path.clone();
    //println!("work path = {}", work_path);
    //println!("path  = {}, len = {}", path, path.len());
    //println!("curr inode id = {}", curr_inode_id);
    let new_ino_id = ch_dir(work_path.as_str(), path.as_str()) as isize;
    //println!("new inode id = {}", new_ino_id);
    if new_ino_id >= 0 {
        //inner.current_inode = new_ino_id as u32;
        // TODO:修改current_path
        if path.chars().nth(0).unwrap() == '/' {
            inner.current_path = path.clone();
        } else {
            work_path.push('/');
            work_path.push_str(path.as_str());
            let mut path_vec: Vec<&str> = work_path.as_str().split('/').collect();
            let mut new_pathv: Vec<&str> = Vec::new(); 
            for i in 0..path_vec.len(){
                if path_vec[i] == "" || path_vec[i] == "." {
                    continue;
                }
                if path_vec[i] == ".." {
                    new_pathv.pop();
                    continue;
                } 
                new_pathv.push(path_vec[i]);
            }
            let mut new_wpath = String::new();
            for i in 0..new_pathv.len(){
                new_wpath.push('/');
                new_wpath.push_str(new_pathv[i]);
            }
            if new_pathv.len() == 0 {
                new_wpath.push('/');
            }
            //println!("after cd workpath = {}", new_wpath);
            inner.current_path = new_wpath.clone();
        }
        new_ino_id
    }else{
        new_ino_id
    }
}

pub fn sys_ls(path: *const u8) -> isize{
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    let path = translated_str(token, path);
    let work_path = inner.current_path.clone();
    //println!("work path = {}", work_path);
    //println!("path  = {}, len = {}", path, path.len());
    list_files(work_path.as_str(), path.as_str());
    //list_files(inner.current_inode);
    //list_files(inner.current_inode);
    0
}

/*
* buf：用于保存当前工作目录的字符串。当buf设为NULL，由系统来分配缓存区。
*/
pub fn sys_getcwd(buf: *mut u8, len: usize)->isize{
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    let mut buf_vec = translated_byte_buffer(token, buf, len);
    let mut current_offset:usize = 0;
    if buf as usize == 0 {
        return 0
    } else {
        // TODO:目前假设长度不超过一页
        for slice_ptr2 in buf_vec.iter_mut() {
            //let wlen = slice_ptr.len().min(len - current_offset);
            unsafe{
                let cwd = inner.current_path.as_bytes();
                for i in 0..cwd.len(){
                    (*slice_ptr2)[i] = cwd[i];
                }
            }
        }
        return buf as isize
    }
}

pub fn sys_dup3( old_fd: usize, new_fd: usize )->isize{
    let task = current_task().unwrap();
    let mut inner = task.acquire_inner_lock();
    if  old_fd >= inner.fd_table.len() || new_fd > FD_LIMIT {
        return -1;
    }
    if inner.fd_table[old_fd].is_none() {
        return -1;
    }
    if new_fd >= inner.fd_table.len() {
        for i in inner.fd_table.len()..(new_fd + 1) {
            inner.fd_table.push(None);
        }
    }
    //let new_fd = inner.alloc_fd();
    inner.fd_table[new_fd] = Some(inner.fd_table[old_fd].as_ref().unwrap().clone());
    new_fd as isize
}

pub fn sys_getdents64(){

}

// TODO: mode需要研究
pub fn sys_mkdir(dirfd:isize, path:*const u8, mode:u32)->isize{
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    let path = translated_str(token, path);
    
    if dirfd == AT_FDCWD {
        let work_path = inner.current_path.clone();
        if let Some(inode) = open(
            inner.get_work_path().as_str(),
            path.as_str(),
            OpenFlags::CREATE,
            DiskInodeType::Directory
        ) {
            return 0
        } else {
            return -1
        }
    } else {
        // TODO: 获取dirfd的OSInode
        //let work_dir = inner.fd_table[dirfd as usize].unwrap() as OSInode;
        if let Some(inode) = open(
            "/",
            path.as_str(),
            OpenFlags::CREATE,
            DiskInodeType::Directory
        ) {
            return 0
        } else {
            return -1
        }
    }
}

pub fn sys_mount( special:*const u8, dir:*const u8, fstype: *const u8, flags:usize, data: *const u8 )->isize{
    // TODO
    0
}

pub fn sys_umount( special:*const u8, flags:usize )->isize{
    // TODO
    0
}

pub fn sys_fstat(){

}   

