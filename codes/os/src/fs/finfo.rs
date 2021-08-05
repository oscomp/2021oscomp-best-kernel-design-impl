//use bitflags::*;
use alloc::vec::Vec;

pub const DT_UNKNOWN:u8 = 0;
pub const DT_DIR:u8 = 4;
pub const DT_REG:u8 = 4; //常规文件

pub const NAME_LIMIT:usize = 128; // TODO:太大了会有跨页问题。。

pub const S_IFMT    :u32 = 0170000;   //bit mask for the file type bit field
pub const S_IFSOCK  :u32 = 0140000;   //socket
pub const S_IFLNK   :u32 = 0120000;   //symbolic link
pub const S_IFREG   :u32 = 0100000;   //regular file
pub const S_IFBLK   :u32 = 0060000;   //block device
pub const S_IFDIR   :u32 = 0040000;   //directory
pub const S_IFCHR   :u32 = 0020000;   //character device
pub const S_IFIFO   :u32 = 0010000;   //FIFO

pub const S_ISUID:u32 = 04000;   //set-user-ID bit (see execve(2))
pub const S_ISGID:u32 = 02000;   //set-group-ID bit (see below)
pub const S_ISVTX:u32 = 01000;   //sticky bit (see below)

pub const S_IRWXU:u32 = 00700;   //owner has read, write, and execute permission
pub const S_IRUSR:u32 = 00400;   //owner has read permission
pub const S_IWUSR:u32 = 00200;   //owner has write permission
pub const S_IXUSR:u32 = 00100;   //owner has execute permission

pub const S_IRWXG:u32 = 00070;   //group has read, write, and execute permission
pub const S_IRGRP:u32 = 00040;   //group has read permission
pub const S_IWGRP:u32 = 00020;   //group has write permission
pub const S_IXGRP:u32 = 00010;   //group has execute permission

pub const S_IRWXO:u32 = 00007;   //others (not in group) have read, write,and execute permission
pub const S_IROTH:u32 = 00004;   //others have read permission
pub const S_IWOTH:u32 = 00002;   //others have write permission
pub const S_IXOTH:u32 = 00001;   //others have execute permission





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
    
    pub fn as_bytes(&self) -> &[u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts(
                self as *const _ as usize as *const u8,
                size,
            )
        }
    }
    
    pub fn as_bytes_mut(&mut self) -> &mut [u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts_mut(
                self as *mut _ as usize as *mut u8,
                size,
            )
        }
    }
}

#[repr(C)]
pub struct Kstat {
	st_dev  :u64,   /* ID of device containing file */
    st_ino  :u64,   /* Inode number */
    st_mode :u32,   /* File type and mode */
    st_nlink:u32,   /* Number of hard links */
    st_uid  :u32,
    st_gid  :u32,
    //__pad   :u64,
    //st_size :i64,
    st_blksize   :u32,
    //__pad2       :i32,
    st_blocks    :u64,
    st_rdev :u64,   /* Device ID (if special file) */
    st_atime_sec :i64, 
    st_atime_nsec:i64,  
    st_mtime_sec :i64,  
    st_mtime_nsec:i64,   
    st_ctime_sec :i64,  
    st_ctime_nsec:i64,  
}

impl Kstat {
    pub fn empty()->Self{
        Self{
            st_dev  :0,
            st_ino  :0,
            st_mode :0100777,
            st_nlink:0,
            st_uid  :0,
            st_gid  :0,
            st_rdev :0,
            //__pad   :0,
            //st_size :0,
            st_blksize   :512,
            //__pad2       :0,
            st_blocks    :0,
            st_atime_sec :0, 
            st_atime_nsec:0,  
            st_mtime_sec :0,  
            st_mtime_nsec:0,   
            st_ctime_sec :0,  
            st_ctime_nsec:0,  
        }
    }

    pub fn new_abstract()->Self{
        Self{
            st_dev  :5,//5
            st_ino  :1,
            st_mode :0100777,
            st_nlink:1,
            st_uid  :0,
            st_gid  :0,
            st_rdev :0x0000000400000040,
            //__pad   :0,
            //st_size :0,
            st_blksize   :512,
            //__pad2       :0,
            st_blocks    :0,
            st_atime_sec :0, 
            st_atime_nsec:0,  
            st_mtime_sec :0,  
            st_mtime_nsec:0,   
            st_ctime_sec :0,  
            st_ctime_nsec:0,  
        }
    }

    // 目前仅填充用户测试需要的成员
    pub fn fill_info(&mut self, 
        st_dev  :u64,
        st_ino  :u64,
        st_mode :u32,
        st_nlink:u32,
        //st_uid  :u32,
        //st_gid  :u32,
        //st_rdev :u64,
        //__pad   :u64,
        st_size :i64,
        //st_blksize   :u32,
        //__pad2       :i32,
        //st_blocks    :u64,
        st_atime_sec :i64, 
        //st_atime_nsec:i64,  
        st_mtime_sec :i64,  
        //st_mtime_nsec:i64,   
        st_ctime_sec :i64,  
        //st_ctime_nsec:i64,  
    ) {
        let st_blocks = ( st_size as u64 + self.st_blksize as u64 - 1)
                            / self.st_blksize as u64;
        *self = Self {
            st_dev  ,
            st_ino  ,
            st_mode ,
            st_nlink,
            st_uid  :0,
            st_gid  :0,
            st_rdev :0,
            //__pad   :0,
            //st_size ,
            st_blksize   :512,
            //__pad2       :0,
            st_blocks    ,
            st_atime_sec , 
            st_atime_nsec:0,  
            st_mtime_sec ,  
            st_mtime_nsec:0,   
            st_ctime_sec ,  
            st_ctime_nsec:0,  
        };
    }

    pub fn as_bytes(&self) -> &[u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts(
                self as *const _ as usize as *const u8,
                size,
            )
        }
    }
    
    pub fn as_bytes_mut(&mut self) -> &mut [u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts_mut(
                self as *mut _ as usize as *mut u8,
                size,
            )
        }
    }
}

#[derive(Debug)]
#[repr(C)]
pub struct NewStat{
    st_dev  :u64,   /* ID of device containing file */
    st_ino  :u64,   /* Inode number */
    st_mode :u32,   /* File type and mode */
    st_nlink:u64,   /* Number of hard links */
    st_uid  :u32,
    st_gid  :u32,
    st_rdev :u64,   /* Device ID (if special file) */
    st_size :u64,         /* Total size, in bytes */
    st_blksize   :u32,    /* Block size for filesystem I/O */
    st_blocks    :u64,    /* Number of 512B blocks allocated */
    st_atime_sec :i64,    
    st_atime_nsec:i64,  
    st_mtime_sec :i64,  
    st_mtime_nsec:i64,   
    st_ctime_sec :i64,  
    st_ctime_nsec:i64,  
}

impl NewStat {
    pub fn empty()->Self{
        Self{
            st_dev  :0,
            st_ino  :0,
            st_mode :0,
            st_nlink:0,
            st_uid  :0,
            st_gid  :0,
            st_rdev :0,
            st_size :0,
            st_blksize   :512,
            st_blocks    :0,
            st_atime_sec :0, 
            st_atime_nsec:0,  
            st_mtime_sec :0,  
            st_mtime_nsec:0,   
            st_ctime_sec :0,  
            st_ctime_nsec:0,  
        }
    }

    // 目前仅填充用户测试需要的成员
    pub fn fill_info(&mut self, 
        st_dev  :u64,
        st_ino  :u64,
        st_mode :u32,
        st_nlink:u64,
        //st_uid  :u32,
        //st_gid  :u32,
        //st_rdev :u64,
        st_size :i64,
        //st_blksize   :u32,
        //st_blocks    :u64,
        st_atime_sec :i64, 
        //st_atime_nsec:i64,  
        st_mtime_sec :i64,  
        //st_mtime_nsec:i64,   
        st_ctime_sec :i64,  
        //st_ctime_nsec:i64,  
    ) {
        let st_blocks = ( st_size as u64 + self.st_blksize as u64 - 1)
                            / self.st_blksize as u64;

        *self = Self {
            st_dev  ,
            st_ino  ,
            st_mode ,
            st_nlink,
            st_uid  :0,
            st_gid  :0,
            st_rdev :0,
            st_size : st_size as u64,
            st_blksize :self.st_blksize, //TODO:real blksize
            st_blocks ,
            st_atime_sec , 
            st_atime_nsec:0,  
            st_mtime_sec ,  
            st_mtime_nsec:0,   
            st_ctime_sec ,  
            st_ctime_nsec:0,  
        };
    }

    pub fn as_bytes(&self) -> &[u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts(
                self as *const _ as usize as *const u8,
                size,
            )
        }
    }
    
    pub fn as_bytes_mut(&mut self) -> &mut [u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts_mut(
                self as *mut _ as usize as *mut u8,
                size,
            )
        }
    }
}

#[repr(C)]
pub struct FdSet {
    fd_list:[u64;16],
}

impl FdSet {
    pub fn new()->Self{
        Self{
            fd_list:[0;16]
        }
    }

    fn check_fd(fd:usize)->bool{
        if fd < 1024 {
            return true
        } else {
            return false
        }
    }

    pub fn set_fd(&mut self, fd:usize){
        if Self::check_fd(fd) {
            let index = fd >> 8;  // fd/64
            let offset = fd - (index << 8);  // fd%64
            self.fd_list[index] |= 1<<offset;
        }
    }

    pub fn clear_fd(&mut self, fd:usize){
        if Self::check_fd(fd){
            let index = fd >> 8;
            let offset = fd - (index << 8);
            self.fd_list[index] &= (0<<offset) & 0xFFFFFFFFFFFFFFFF;
        }
    }

    pub fn clear_all(&mut self){
        for i in 0..16 {
            self.fd_list[i] = 0;
        }
    }

    pub fn count(&mut self)->usize{
        let fd_vec = self.get_fd_vec();
        fd_vec.len()
    }

    pub fn get_fd_vec(&self)->Vec<usize>{
        let mut fd_v = Vec::new();
        for i in 0..16 {
            let mut tmp = self.fd_list[i];
            for off in 0..64 {
                let fd_bit = tmp & 1;
                if fd_bit == 1{
                    fd_v.push( (i << 8) + off );  // index*64 + offset 
                }
                tmp = tmp >> 1;
            }
        }
        fd_v
    }

    pub fn as_bytes(&self) -> &[u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts(
                self as *const _ as usize as *const u8,
                size,
            )
        }
    }
    
    pub fn as_bytes_mut(&mut self) -> &mut [u8] {
        let size = core::mem::size_of::<Self>();
        unsafe {
            core::slice::from_raw_parts_mut(
                self as *mut _ as usize as *mut u8,
                size,
            )
        }
    }
}