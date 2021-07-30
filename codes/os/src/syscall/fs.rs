use core::usize;

use crate::{fs::{/*OSInode,*/
        Kstat, NewStat, FileClass, 
        File, Dirent, MNT_TABLE, IoVec, IoVecs, TTY,
        FileDiscripter
    }, 
    mm::{UserBuffer, translated_byte_buffer, translated_ref, translated_refmut, translated_str}, task::{
        FdTable,
        TaskControlBlockInner,
    },
    gdb_println,
    monitor::*
};
use crate::task::{current_user_token, current_task/* , print_core_info*/};
use crate::fs::{make_pipe, OpenFlags, open, ch_dir, list_files, DiskInodeType};
use alloc::sync::Arc;
use alloc::vec::Vec;
use alloc::string::String;
use spin::mutex::*;

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
        let f: Arc<dyn File + Send + Sync> = match &file.fclass {
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

pub fn sys_writev(fd:usize, iov_ptr: usize, iov_num:usize)->isize{
    let iov_head = iov_ptr as *mut IoVec;
    
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    if fd >= inner.fd_table.len() {
        return -1;
    }
    if let Some(file) = &inner.fd_table[fd] {
        let f: Arc<dyn File + Send + Sync> = match &file.fclass {
            FileClass::Abstr(f)=> {f.clone()},
            FileClass::File(f)=>{f.clone()},
            _ => return -1,
        };
        if !f.writable() {
            return -1;
        }
        drop(inner);
        unsafe {
            let buf = UserBuffer::new( IoVecs::new(
                iov_head,
                iov_num,
                token
            ).0);
            f.write(buf) as isize
        }   
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
        let file: Arc<dyn File + Send + Sync> = match &file.fclass {
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
    //gdb_println!(SYSCALL_ENABLE, "openat: path = {}", path);
    let mut inner = task.acquire_inner_lock();
    
    /////////////////////////////// WARNING ////////////////////////////////
    // 只是测试用的临时处理
    if path.contains("/dev") {
        let fd = inner.alloc_fd();
        inner.fd_table[fd] = Some( FileDiscripter::new(
            false,
            FileClass::Abstr(TTY.clone())));
        return fd as isize
    }
    //if path.contains("|") {
    //    let fd = inner.alloc_fd();
    //    inner.fd_table[fd] = Some( FileDiscripter::new(
    //        false,
    //        FileClass::Abstr(  )
    //    ));
    //    return fd as isize
    //}

    ////////////////////////////////////////////////////////////////////////

    let oflags = OpenFlags::from_bits(flags).unwrap();
    if dirfd == AT_FDCWD {
        if let Some(inode) = open(
            inner.get_work_path().as_str(),
            path.as_str(),
            oflags,
            DiskInodeType::File
        ) {
            let fd = inner.alloc_fd();
            inner.fd_table[fd] = Some( FileDiscripter::new(
                oflags.contains(OpenFlags::CLOEXEC),
                FileClass::File(inode)
            ));
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
            match &file.fclass {
                FileClass::File(f) => {
                    //let oflags = OpenFlags::from_bits(flags).unwrap();
                    // 需要新建文件
                    if oflags.contains(OpenFlags::CREATE){ 
                        if let Some(tar_f) = f.create(path.as_str(), DiskInodeType::File){ //TODO
                            let fd = inner.alloc_fd();
                            inner.fd_table[fd] = Some( FileDiscripter::new(
                                oflags.contains(OpenFlags::CLOEXEC),
                                FileClass::File(tar_f)
                            ));
                            return fd as isize
                        }else{
                            return -1;
                        }
                    }
                    // 正常打开文件
                    if let Some(tar_f) = f.find(path.as_str(), oflags){
                        let fd = inner.alloc_fd();
                        inner.fd_table[fd] = Some( FileDiscripter::new(
                            oflags.contains(OpenFlags::CLOEXEC),
                            FileClass::File(tar_f)
                        ));
                        fd as isize
                    }else{
                        return -1;
                    }
                },
                _ => return -1, // 如果是抽象类文件，不能open
            }
        } else {
            return -1
        }
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

pub fn sys_pipe(pipe: *mut u32) -> isize {
    let task = current_task().unwrap();
    let token = current_user_token();
    let mut inner = task.acquire_inner_lock();
    let (pipe_read, pipe_write) = make_pipe();
    let read_fd = inner.alloc_fd();
    inner.fd_table[read_fd] = Some( FileDiscripter::new(
        false,
        FileClass::Abstr(pipe_read)
    ));
    let write_fd = inner.alloc_fd();
    inner.fd_table[write_fd] = Some( FileDiscripter::new(
        false,
        FileClass::Abstr(pipe_write)
    ));
    *translated_refmut(token, pipe) = read_fd as u32;
    *translated_refmut(token, unsafe { pipe.add(1) }) = write_fd as u32;
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
    // println!("ls");
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    let path = translated_str(token, path);
    let work_path = inner.current_path.clone();
    // println!("work path = {}", work_path);
    // println!("path  = {}, len = {}", path, path.len());
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
    let buf_vec = translated_byte_buffer(token, buf, len);
    let mut userbuf = UserBuffer::new(buf_vec);
    let current_offset:usize = 0;
    if buf as usize == 0 {
        return 0
    } else {
        let cwd = inner.current_path.as_bytes();
        userbuf.write( cwd );
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

    //let mut act_fd = new_fd;
    //if inner.fd_table[new_fd].is_some() {
    //    act_fd = inner.alloc_fd();
    //}
    //let new_fd = inner.alloc_fd();
    inner.fd_table[new_fd] = Some(inner.fd_table[old_fd].as_ref().unwrap().clone());
    new_fd as isize
}

pub fn sys_getdents64(fd:isize, buf: *mut u8, len:usize)->isize{
    //return 0;
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    let buf_vec = translated_byte_buffer(token, buf, len);
    // 使用UserBuffer结构，以便于跨页读写
    let mut userbuf = UserBuffer::new(buf_vec);
    let mut dirent = Dirent::empty();
    if fd == AT_FDCWD {
        let work_path = inner.current_path.clone();
        if let Some(file) = open(
            "/",
            work_path.as_str(),
            OpenFlags::RDONLY,
            DiskInodeType::Directory
        ) {
            let len = file.getdirent(&mut dirent);
            userbuf.write(dirent.as_bytes());
            return 0; //warning
        } else {
            return -1
        }
    } else {
        let fd_usz = fd as usize;
        if fd_usz >= inner.fd_table.len() && fd_usz > FD_LIMIT {
            return -1
        }
        if let Some(file) = &inner.fd_table[fd_usz] {
            match &file.fclass {
                FileClass::File(f) => {
                    let len = f.getdirent(&mut dirent);
                    userbuf.write(dirent.as_bytes());
                    return 0; //warning
                },
                _ => return -1,
            }
        } else {
            return -1
        }
    }
}

pub fn sys_fstat(fd:isize, buf: *mut u8)->isize{
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    let mut buf_vec = translated_byte_buffer(token, buf, core::mem::size_of::<Kstat>());
    // 使用UserBuffer结构，以便于跨页读写
    let mut userbuf = UserBuffer::new(buf_vec);
    let mut kstat = Kstat::empty();
    if fd == AT_FDCWD {
        let work_path = inner.current_path.clone();
        if let Some(file) = open(
            "/",
            work_path.as_str(),
            OpenFlags::RDONLY,
            DiskInodeType::Directory
        ) {
            file.get_fstat(&mut kstat);
            userbuf.write(kstat.as_bytes());
            return 0
        } else {
            return -1
        }
    } else {
        let fd_usz = fd as usize;
        if fd_usz >= inner.fd_table.len() && fd_usz > FD_LIMIT {
            return -1
        }
        if let Some(file) = &inner.fd_table[fd_usz] {
            match &file.fclass {
                FileClass::File(f) => {
                    f.get_fstat(&mut kstat);
                    userbuf.write(kstat.as_bytes());
                    return 0
                },
                _ => return -1,
            }
        } else {
            return -1
        }
    }
}   

pub fn sys_newfstatat(fd:isize, path: *const u8, buf: *mut u8, flag: u32)->isize{
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    let mut buf_vec = translated_byte_buffer(token, buf, core::mem::size_of::<NewStat>());
    // 使用UserBuffer结构，以便于跨页读写
    let mut userbuf = UserBuffer::new(buf_vec);
    let mut stat = NewStat::empty();
    let path = translated_str(token, path);
    
    if fd == AT_FDCWD {
        let work_path = inner.current_path.clone();
        if let Some(file) = open(
            work_path.as_str(),
            path.as_str(),
            OpenFlags::RDONLY,
            DiskInodeType::Directory
        ) {
            file.get_newstat(&mut stat);
            userbuf.write(stat.as_bytes());
            return 0
        } else {
            return -2
        }
    } else {
        let fd_usz = fd as usize;
        if fd_usz >= inner.fd_table.len() && fd_usz > FD_LIMIT {
            return -1
        }
        if let Some(file) = &inner.fd_table[fd_usz] {
            match &file.fclass {
                FileClass::File(f) => {
                    f.get_newstat(&mut stat);
                    userbuf.write(stat.as_bytes());
                    return 0
                },
                _ => return -1,
            }
        } else {
            return -2
        }
    }
}   

// TODO: mode需要研究
pub fn sys_mkdir(dirfd:isize, path: *const u8, mode:u32)->isize{
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
        // DEBUG: 获取dirfd的OSInode
        let fd_usz = dirfd as usize;
        if fd_usz >= inner.fd_table.len() && fd_usz > FD_LIMIT {
            return -1
        }
        if let Some(file) = &inner.fd_table[fd_usz] {
            match &file.fclass {
                FileClass::File(f) => {
                    if let Some(new_dir) = f.create(path.as_str(), DiskInodeType::Directory){
                        return 0;
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

pub fn sys_mount( p_special:*const u8, p_dir:*const u8, p_fstype: *const u8, flags:usize, data: *const u8 )->isize{
    // TODO
    let token = current_user_token();
    let special = translated_str(token, p_special);
    let dir = translated_str(token, p_dir);
    let fstype = translated_str(token, p_fstype);
    MNT_TABLE.lock().mount(special, dir, fstype, flags as u32)
}

pub fn sys_umount( p_special:*const u8, flags:usize )->isize{
    // TODO
    // println!("sys umount");
    let token = current_user_token();
    let special = translated_str(token, p_special);
    MNT_TABLE.lock().umount(special, flags as u32)
}

pub fn sys_clear( path:*const u8 )->isize{
    let task = current_task().unwrap();
    let token = current_user_token();
    // 这里传入的地址为用户的虚地址，因此要使用用户的虚地址进行映射
    let path = translated_str(token, path);
    let mut inner = task.acquire_inner_lock();
    if let Some(inode) = open(
        inner.get_work_path().as_str(),
        path.as_str(),
        OpenFlags::WRONLY,
        DiskInodeType::File
    ) {
        inode.clear();
        return 0
    } else {
        return -1
    }
}


//pub fn sys_linkat(oldfd:i32, old_path:*const u8, newfd:i32, newpath:*const u8, flags:u32)->isize{
//    return 0
//}

pub fn sys_unlinkat(fd:i32, path:*const u8, flags:u32)->isize{
    let task = current_task().unwrap();
    let token = current_user_token();
    // 这里传入的地址为用户的虚地址，因此要使用用户的虚地址进行映射
    let path = translated_str(token, path);
    //print!("\n");
    //println!("unlink: path = {}", path);
    let mut inner = task.acquire_inner_lock();
    //println!("openat: fd = {}", dirfd);

    if let Some(file) = get_file_discpt(fd as isize, path, &inner, OpenFlags::from_bits(flags).unwrap()){
        match file {
            FileClass::File(f)=>{
                f.delete();
                return 0
            }
            _=> return -1
        }    
    } else{
        return -1
    }
}

pub fn sys_ioctl(fd:usize, cmd:u32, arg:usize)->isize{
    let token = current_user_token();
    let task = current_task().unwrap();
    let inner = task.acquire_inner_lock();
    if fd >= inner.fd_table.len() {
        return -1;
    }
    if let Some(file) = &inner.fd_table[fd] {
        let file: Arc<dyn File + Send + Sync> = match &file.fclass {
            FileClass::Abstr(f)=> {f.clone()},
            FileClass::File(f)=>{f.clone()},
            _ => return -1,
        };
        drop(inner);
        return file.ioctl(cmd, arg)
    } else {
        return -1
    }
}



/* fcntl */
/* cmd */
pub const F_DUPFD: u32 = 0; /*  dup the fd using the lowest-numbered
                            available file descriptor greater than or equal to arg.
                            on success, return new fd*/

pub const F_GETFD: u32 = 1; /* fd flag */
pub const F_SETFD: u32 = 2;
pub const F_GETFL: u32 = 3;

pub const F_DUPFD_CLOEXEC: u32 = 1030;  /* Duplicate file descriptor with close-on-exit set.*/

/* arg */
pub const FD_CLOEXEC: u32 = 1;

pub fn fcntl(fd:usize, cmd:u32, arg:usize)->isize{
    let task = current_task().unwrap();
    let mut inner = task.acquire_inner_lock();
    
    if fd > inner.fd_table.len() {
        return -1;
    }

    gdb_println!(SYSCALL_ENABLE,"cmd = {}", cmd);

    if let Some(file) = &mut inner.fd_table[fd] {
        match cmd {
            
            F_DUPFD => {
                return dup_inc(fd, arg, &mut inner.fd_table)
            },
            F_GETFD=> { 
                return file.get_cloexec() as isize
            }
            F_SETFD=> {
                file.set_cloexec((arg & 1) == 1);
                return 0;
            }
            F_DUPFD_CLOEXEC =>{
                let new_fd = dup_inc(fd, arg, &mut inner.fd_table);
                if let Some (new_file) = &mut inner.fd_table[new_fd as usize] {
                    new_file.set_cloexec(true);
                    return new_fd
                } else {
                    return -1
                }
            }
            _=> return 0, // WARNING!!!
        }
    } else {
        return -1;
    }    
}

/* dup the fd using the lowest-numbered available fd >= new_fd */
fn dup_inc( old_fd:usize, new_fd:usize, fd_table: &mut FdTable) -> isize {
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

    let mut act_fd = new_fd;
    if inner.fd_table[new_fd].is_some() {
        act_fd = inner.alloc_fd();
    }
    inner.fd_table[act_fd] = Some(inner.fd_table[old_fd].as_ref().unwrap().clone());
    act_fd as isize
}

pub fn sys_utimensat(fd:usize, path:*const u8, time:usize, flags:u32)->isize{
    let task = current_task().unwrap();
    let token = current_user_token();
    // 这里传入的地址为用户的虚地址，因此要使用用户的虚地址进行映射
    let path = translated_str(token, path);
    //print!("\n");
    //println!("unlink: path = {}", path);
    let mut inner = task.acquire_inner_lock();
    //println!("openat: fd = {}", dirfd);
    if let Some(file) = get_file_discpt(fd as isize, path, &inner,  OpenFlags::from_bits(flags).unwrap() ){
        match file {
            FileClass::File(f)=>{
                return 0
            }
            _=> return -1
        }    
    } else{
        return -2
    }
}


pub fn sys_renameat2( old_dirfd:isize, old_path:*const u8, new_dirfd:isize, new_path:*const u8, flags: u32 )->isize{
    if flags != 0 {
        println!("[sys_renameat2] cannot handle flags != 0");
        //return -1;
    }
    let task = current_task().unwrap();
    let token = current_user_token();
    let inner = task.acquire_inner_lock();
    let oldpath = translated_str(token, old_path);
    let newpath = translated_str(token, new_path);
    // find old file
    //let mut inner = task.acquire_inner_lock();
    if let Some(old_file_class) = get_file_discpt(
        old_dirfd, 
        oldpath, 
        & inner, 
        OpenFlags::RDWR   
    ){
        match old_file_class {
            FileClass::File(oldfile)=>{
                // crate new file
                let oflags= {
                    if oldfile.is_dir() {
                        OpenFlags::CREATE | OpenFlags::RDWR | OpenFlags::DIRECTROY
                    } else {
                        OpenFlags::CREATE | OpenFlags::RDWR
                    }
                };
                
                if let Some(new_file_class) = get_file_discpt(
                    new_dirfd, 
                    newpath, 
                    & inner, 
                    oflags
                ){
                    // copy
                    let first_cluster = oldfile.get_head_cluster();
                    // TODO:
                    match new_file_class {
                        FileClass::File(newfile)=>{
                            newfile.set_head_cluster(first_cluster);
                        }
                        _=> return -1
                    }
                    oldfile.delete();
                } else {
                    return -1;
                }   
            }
            _=> return -1
        }
    } else {
        return -1;
    }
    return 0
}

/* return the num of bytes */
pub fn sys_sendfile(out_fd:isize, in_fd:isize, offset_ptr: *mut usize, count: usize)->isize {
    /* 
        If offset is not NULL, then it points to a variable holding the
        file offset from which sendfile() will start reading data from
        in_fd.  

        When sendfile() returns, 
        *** this variable will be set to the offset of the byte following 
        the last byte that was read. ***  
        
        If offset is not NULL, then sendfile() does not modify the file
        offset of in_fd; otherwise the file offset is adjusted to reflect
        the number of bytes read from in_fd.

        If offset is NULL, then data will be read from in_fd starting at
        the file offset, and the file offset will be updated by the call.
    */
    let task = current_task().unwrap();
    let token = current_user_token();
    let inner = task.acquire_inner_lock();

    //task.kmmap(0, count, 0, 0, in_fd, off);
    
    if let Some(file_in) = &inner.fd_table[in_fd as usize]{
        // file_in exists
        match &file_in.fclass {
            FileClass::File(fin)=>{
                if let Some(file_out) = &inner.fd_table[out_fd as usize]{
                    //file_out exists
                    match &file_out.fclass {
                        FileClass::File(fout)=>{
                            if offset_ptr as usize != 0 { //won't influence file.offset                            
                                let offset = translated_refmut(token, offset_ptr);
                                let data = fin.read_vec(*offset as isize, count);
                                let wlen =  fout.write_all(&data);
                                *offset += wlen;
                                return wlen as isize
                            } else {  //use file.offset
                                let data = fin.read_vec(-1, count);
                                let wlen =  fout.write_all(&data);
                                return wlen as isize
                            }
                        }
                        _=> return -1
                    }
                } else {
                    return -1
                }
            }
            _=> return -1
        }
    } else {
        return -1
    }
}



fn get_file_discpt(fd: isize, path:String, inner: &MutexGuard<TaskControlBlockInner>, oflags: OpenFlags) -> Option<FileClass>{
    let type_ = {
        if oflags.contains(OpenFlags::DIRECTROY) {
            DiskInodeType::Directory
        } else {
            DiskInodeType::File
        }
    };
    if fd == AT_FDCWD {
        if let Some(inode) = open(
            inner.get_work_path().as_str(),
            path.as_str(),
            oflags,
            type_
        ) {
            //println!("find old");
            return Some(FileClass::File(inode))
        } else {
            return None
        }
    } else {    
        let fd_usz = fd as usize;
        if fd_usz >= inner.fd_table.len() && fd_usz > FD_LIMIT {
            return None
        }
        if let Some(file) = &inner.fd_table[fd_usz] {
            match &file.fclass {
                FileClass::File(f) => {
                    if oflags.contains(OpenFlags::CREATE){
                        if let Some(tar_f) = f.create(path.as_str(), type_){
                            return Some(FileClass::File(tar_f))
                        }else{
                            return None
                        }
                    }else{
                        if let Some(tar_f) = f.find(path.as_str(), oflags){
                            return Some(FileClass::File(tar_f))
                        }else{
                            return None
                        }
                    }
                },
                _ => return None, // 如果是抽象类文件，不能open
            }
        } else {
            return None
        }
    }
}