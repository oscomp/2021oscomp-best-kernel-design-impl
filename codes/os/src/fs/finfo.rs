pub const DT_UNKNOWN:u8 = 0;
pub const DT_DIR:u8 = 4;
pub const DT_REG:u8 = 4; //常规文件

pub const NAME_LIMIT:usize = 128; // TODO:太大了会有跨页问题。。

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
	st_dev  :u64,
    st_ino  :u64,
    st_mode :u32,
    st_nlink:u32,
    st_uid  :u32,
    st_gid  :u32,
    st_rdev :u64,
    __pad   :u64,
    st_size :i64,
    st_blksize   :u32,
    __pad2       :i32,
    st_blocks    :u64,
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
            st_mode :0,
            st_nlink:0,
            st_uid  :0,
            st_gid  :0,
            st_rdev :0,
            __pad   :0,
            st_size :0,
            st_blksize   :0,
            __pad2       :0,
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
        *self = Self {
            st_dev  ,
            st_ino  ,
            st_mode ,
            st_nlink,
            st_uid  :0,
            st_gid  :0,
            st_rdev :0,
            __pad   :0,
            st_size ,
            st_blksize   :0,
            __pad2       :0,
            st_blocks    :0,
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