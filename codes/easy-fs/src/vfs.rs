use super::{
    BlockDevice,
    DiskInode,
    DiskInodeType,
    DirEntry,
    EasyFileSystem,
    DIRENT_SZ,
    get_block_cache,
};
use alloc::sync::Arc;
use alloc::string::String;
use alloc::vec::Vec;
use spin::{Mutex, MutexGuard};

#[derive(Clone)]
pub struct Inode {
    inode_id: u32,
    block_id: usize,
    block_offset: usize,  //记录inode在磁盘上的位置
    fs: Arc<Mutex<EasyFileSystem>>,
    block_device: Arc<dyn BlockDevice>,
}

impl Inode {
    pub fn new(
        inode_id: u32,
        fs: Arc<Mutex<EasyFileSystem>>,
        block_device: Arc<dyn BlockDevice>,
    ) -> Self {
        let (block_id, block_offset) = fs.lock().get_disk_inode_pos(inode_id);
        Self {
            inode_id,
            block_id: block_id as usize,
            block_offset,
            fs,
            block_device,
        }
    }

    pub fn get_id(&self) -> u32{
        self.inode_id
    }

    pub fn get_fs(&self) -> Arc<Mutex<EasyFileSystem>>{
        self.fs.clone()
    }

    pub fn get_type(&self) -> DiskInodeType{
        self.read_disk_inode(|disk_inode: & DiskInode|{
            disk_inode.type_
        })
    }

    fn read_disk_inode<V>(&self, f: impl FnOnce(&DiskInode) -> V) -> V {
        get_block_cache(
            self.block_id,
            Arc::clone(&self.block_device)
        ).lock().read(self.block_offset, f)
    }

    fn modify_disk_inode<V>(&self, f: impl FnOnce(&mut DiskInode) -> V) -> V {
        get_block_cache(
            self.block_id,
            Arc::clone(&self.block_device)
        ).lock().modify(self.block_offset, f)
    }

    fn find_inode_id(
        &self,
        name: &str,
        disk_inode: &DiskInode,
    ) -> Option<(u32,u32)> {
        // 返回inode_id和文件目录项在目录的偏移量
        assert!(disk_inode.is_dir());
        let file_count = (disk_inode.size as usize) / DIRENT_SZ;
        let mut dirent = DirEntry::empty();
        for i in 0..file_count {
            assert_eq!(
                disk_inode.read_at(
                    DIRENT_SZ * i,
                    dirent.as_bytes_mut(),
                    &self.block_device,
                ),
                DIRENT_SZ,
            );
            if dirent.name() == name {
                return Some((dirent.inode_number() as u32, i as u32));
            }
        }
        None
    }

    fn find(&self, name: &str) -> Option<(Arc<Inode>,u32)> {
        // 搜索当前目录下的文件/目录
        let _ = self.fs.lock();
        self.read_disk_inode(|disk_inode| {
            // find_inode_id会检查是否是目录
            self.find_inode_id(name, disk_inode)// 获取Inode编号
            .map(|(inode_id,offset)| { //转换为Inode
                (   Arc::new(Self::new(
                        inode_id,
                        self.fs.clone(),
                        self.block_device.clone(),
                    )),
                    offset
                )
            })
        })
    }

    pub fn find_path(&self, path: Vec<&str>) -> Option<(Arc<Inode>, u32)> {
        // 返回inode和文件的dirent在目录中的偏移
        // 根据路径搜索文件/目录
        // 调用find逐级搜索
        // shell应当保证path有内容!!!这里不对len进行检查以提高效率。
        let len = path.len();
        let mut curr_inode:Arc<Inode> = Arc::new(self.clone());
        let mut curr_offset:u32 = 0;
        for i in 0 .. len {
            // DEBUG
            if path[i] == ""{
                continue;
            }
            if let Some((inode,offset)) = curr_inode.find(path[i]){
                curr_inode = inode;
                curr_offset = offset;
            }else{
                return None;
            }
        }
        Some((curr_inode.clone(), curr_offset))
    }

    // TODO:暂时只能复制文件，不具备递归复制目录的功能DEBUG
    // DEBUG
    pub fn fcopy(&self, mut src_path: Vec<&str>, mut dst_path: Vec<&str>)->bool{
        // 每次读一个块，写一个块，参照readall
        let dst_name = dst_path.pop().unwrap();
        if let Some((src_ino, _)) = self.find_path(src_path){
            let type_ = src_ino.get_type();
            assert_eq!(type_, DiskInodeType::File, "sorry, cannot copy dir!");
            if let Some(( dst_par_ino, _)) = self.find_path(dst_path){
                if let Some(dst_ino) =  dst_par_ino.create(dst_name, type_){
                    // TODO: 此处用循环边读边写。


                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
        false
    }

    // DEBUG
    pub fn fmove(&self, mut src_path: Vec<&str>, mut dst_path: Vec<&str>)->bool{
        // 直接修改目录项
        let src_name = src_path.pop().unwrap();
        let dst_name = dst_path.pop().unwrap();
        if let Some((src_par_ino, _)) = self.find_path(src_path){
            if let Some(( dst_par_ino, _)) = self.find_path(dst_path){
                if let Some((src_ino, offset)) = src_par_ino.find(src_name){
                    let mut fs = self.fs.lock();
                    let src_id = src_ino.get_id();
                    // 将src的目录项无气泡删除
                    src_par_ino.modify_disk_inode( |disk_inode: &mut DiskInode|{
                        let file_count = (disk_inode.size as usize) / DIRENT_SZ;
                        let new_size = (file_count - 1) * DIRENT_SZ;
                        // 移动目录项
                        let mut dirent = DirEntry::empty();
                        // 读出最后一个目录项
                        disk_inode.read_at(
                            new_size,
                            dirent.as_bytes_mut(),
                            &self.block_device,
                        );
                        // 写入被删除的位置
                        disk_inode.write_at(
                            offset as usize,
                            dirent.as_bytes(),
                            &self.block_device,
                        );
                        dst_par_ino.decrease_size(new_size as u32, disk_inode, &mut fs);
                    });
                    // 在dst新建目录项，并与源inode_id绑定
                    dst_par_ino.modify_disk_inode( |disk_inode: &mut DiskInode|{
                        let file_count = (disk_inode.size as usize) / DIRENT_SZ;
                        let new_size = (file_count + 1) * DIRENT_SZ;
                        self.increase_size(new_size as u32, disk_inode, &mut fs);
                        let dirent = DirEntry::new(dst_name, src_ino.get_type(),src_id);
                        disk_inode.write_at(
                            file_count * DIRENT_SZ,
                            dirent.as_bytes(),
                            &self.block_device,
                        );
                    });
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }

    fn remove_file(&self)->bool{
        // 删除当前Inode对应的文件
        // 清空文件内容
        self.clear();
        // 释放inode
        let mut fs = self.fs.lock();
        fs.dealloc_inode(self.inode_id);
        true
    }

    fn remove_dir(&self)->bool{
        // 递归删除当前Inode对应的目录
        // DEBUG
        self.read_disk_inode(|disk_inode| {
            assert!(disk_inode.is_dir());
            let file_count = (disk_inode.size as usize) / DIRENT_SZ;
            let mut dirent = DirEntry::empty();
            // 删除目录下的每一项
            for i in 0..file_count {
                assert_eq!(
                    disk_inode.read_at(
                        DIRENT_SZ * i,
                        dirent.as_bytes_mut(),
                        &self.block_device,
                    ),
                    DIRENT_SZ,
                );
                let temp_inode = Inode::new(
                    dirent.inode_number(),
                    self.fs.clone(), 
                    self.block_device.clone()
                );
                let type_:DiskInodeType =  temp_inode.read_disk_inode(
                    |disk_inode|{
                        disk_inode.type_
                    }
                );
                let result:bool;
                // 根据类别删除
                if type_ == DiskInodeType::File{
                    result = temp_inode.remove_file();
                } else if type_ == DiskInodeType::Directory {
                    result = temp_inode.remove_dir();
                } else {
                    return false;
                }
                if result == false{
                    return false;
                }
            }
            // 清除所有目录项
            self.clear();
            // 释放inode
            let mut fs = self.fs.lock();
            fs.dealloc_inode(self.inode_id);
            true
        })
    }

    pub fn remove(&self, mut path: Vec<&str>, type_: DiskInodeType)->bool{
        // 删除文件/目录
        let result:bool;
        let name = path.pop().unwrap();
        // 获取上级目录
        // ?
        if let Some((par_inode, _)) = self.find_path(path){
            // 搜索目标文件/目录
            if let Some((tar_inode,offset)) = par_inode.find(name){
                if type_ == DiskInodeType::File{
                    result = tar_inode.remove_file();
                } else if type_ == DiskInodeType::Directory{
                    result = tar_inode.remove_dir();
                } else {
                    return false;
                }
                if result == false {
                    return false;
                }
                // DEBUG: 修改目录项，调整目录大小
                // 对fs上锁，避免修改途中有进程对其操作
                let mut fs = self.fs.lock();
                // 修改上级目录的目录项，调整大小
                par_inode.modify_disk_inode(|curr_inode| {
                    let file_count = (curr_inode.size as usize) / DIRENT_SZ;
                    let new_size = (file_count - 1) * DIRENT_SZ;
                    // 移动目录项
                    let mut dirent = DirEntry::empty();
                    // 读出最后一个目录项
                    curr_inode.read_at(
                        new_size,
                        dirent.as_bytes_mut(),
                        &self.block_device,
                    );
                    // 写入被删除的位置
                    curr_inode.write_at(
                        offset as usize,
                        dirent.as_bytes(),
                        &self.block_device,
                    );
                    par_inode.decrease_size(new_size as u32, curr_inode, &mut fs);
                });
            return result;
            } else{
                return false;
            }
        }else{
            false
        }
    }

    
    fn decrease_size(
        &self,
        new_size: u32,
        disk_inode: &mut DiskInode,
        fs: &mut MutexGuard<EasyFileSystem>,
    ){
        // TODO: 修改文件后也要检测是否需要回收块？
        // 因为无气泡压缩删除的设计，目录的大小很好判断
        if new_size > disk_inode.size {
            return;
        }
        // disk_inode释放不需要的块
        if let Some(blk_vec) = disk_inode.decrease_size(new_size, &self.block_device){
            // 控制文件系统回收块
            for block_id in blk_vec {
                fs.dealloc_data(block_id);
            }
        }        
    }

    fn increase_size(
        &self,
        new_size: u32,
        disk_inode: &mut DiskInode,
        fs: &mut MutexGuard<EasyFileSystem>,
    ) {
        if new_size <= disk_inode.size {
            //println!("new_size < disk_inode.size");
            return;
        }
        let blocks_needed = disk_inode.blocks_num_needed(new_size);
        let mut v: Vec<u32> = Vec::new();
        for _ in 0..blocks_needed {
            v.push(fs.alloc_data());
        }
        disk_inode.increase_size(new_size, v, &self.block_device);
    }

    pub fn get_size(&self)->u32{
        self.read_disk_inode(|disk_inode: &DiskInode|{
            disk_inode.size
        })
    }

    pub fn create(&self, name: &str , type_: DiskInodeType) -> Option<Arc<Inode>> {
        let mut fs = self.fs.lock();
        if self.modify_disk_inode(|curr_inode| {
            // assert it is a directory
            assert!(curr_inode.is_dir());
            // has the file been created?
            // 文件和目录也不能同名！
            self.find_inode_id(name, curr_inode)
        }).is_some() {
            return None;
        }
        // create a new file
        // alloc a inode with an indirect block
        let new_inode_id = fs.alloc_inode();
        // initialize inode
        let (new_inode_block_id, new_inode_block_offset) 
            = fs.get_disk_inode_pos(new_inode_id);
        get_block_cache(
            new_inode_block_id as usize,
            Arc::clone(&self.block_device)
        ).lock().modify(new_inode_block_offset, |new_dskinode: &mut DiskInode| {
            new_dskinode.initialize(type_.clone());
            // DEBUG: 为新目录分配一个块并建立.和..两个目录项
            if type_ == DiskInodeType::Directory {
                let new_size = 2 * DIRENT_SZ;
                //println!("0.0._");
                // get_inode/new会获取锁
                // let new_inode = EasyFileSystem::get_inode(&self.fs.clone(), new_inode_id);
                let (block_id, block_offset) = fs.get_disk_inode_pos(new_inode_id);
                let new_inode = Self{
                    inode_id: new_inode_id,
                    block_id: block_id as usize,
                    block_offset,
                    fs: self.fs.clone(),
                    block_device: self.block_device.clone(),
                };
                // increase_size自身不会获取锁，因此不会死锁
                //println!("0.0.0");
                new_inode.increase_size(new_size as u32, new_dskinode, &mut fs);
                let dirent_self = DirEntry::new(".", DiskInodeType::Directory,new_inode_id);
                let dirent_parent = DirEntry::new("..", DiskInodeType::Directory,self.get_id());
                //println!("0.0.1");
                new_dskinode.write_at(
                    0, 
                    dirent_self.as_bytes(), 
                    &self.block_device
                );
                //println!("0.0.2");
                new_dskinode.write_at(
                    DIRENT_SZ, 
                    dirent_parent.as_bytes(), 
                    &self.block_device
                );
            }
        });

        // 对当前目录进行修改
        self.modify_disk_inode(|curr_inode| {
            // append file in the dirent
            let file_count = (curr_inode.size as usize) / DIRENT_SZ;
            let new_size = (file_count + 1) * DIRENT_SZ;
            // increase size
            self.increase_size(new_size as u32, curr_inode, &mut fs);
            // write dirent
            let dirent = DirEntry::new(name, type_,new_inode_id);
            curr_inode.write_at(
                file_count * DIRENT_SZ,
                dirent.as_bytes(),
                &self.block_device,
            );
        });

        // release efs lock manually because we will acquire it again in Inode::new
        drop(fs);
        // return inode
        Some(Arc::new(Self::new(
            new_inode_id,
            self.fs.clone(),
            self.block_device.clone(),
        )))
    }

    pub fn ls(&self) -> Vec<(String, DiskInodeType)> {
        let _ = self.fs.lock();
        self.read_disk_inode(|disk_inode| {
            let file_count = (disk_inode.size as usize) / DIRENT_SZ;
            let mut v: Vec<(String, DiskInodeType)> = Vec::new();
            //let mut tv: Vec<DiskInodeType> = Vec::new();
            for i in 0..file_count {
                let mut dirent = DirEntry::empty();
                assert_eq!(
                    disk_inode.read_at(
                        i * DIRENT_SZ,
                        dirent.as_bytes_mut(),
                        &self.block_device,
                    ),
                    DIRENT_SZ,
                );
                v.push( (String::from(dirent.name()), dirent.type_()) );
            }
            v.sort_by(|a, b| a.0.cmp(&b.0));
            v
        })
    }

    pub fn read_at(&self, offset: usize, buf: &mut [u8]) -> usize {
        let _ = self.fs.lock();
        self.read_disk_inode(|disk_inode| {
            disk_inode.read_at(offset, buf, &self.block_device)
        })
    }

    pub fn write_at(&self, offset: usize, buf: &[u8]) -> usize {
        let mut fs = self.fs.lock();
        self.modify_disk_inode(|disk_inode| {
            //println!("buflen = {}",buf.len());
            self.increase_size((offset + buf.len()) as u32, disk_inode, &mut fs);
            disk_inode.write_at(offset, buf, &self.block_device)
        })
    }

    pub fn clear(&self) {
        let mut fs = self.fs.lock();
        self.modify_disk_inode(|disk_inode| {
            let size = disk_inode.size;
            let data_blocks_dealloc = disk_inode.clear_size(&self.block_device);
            assert!(data_blocks_dealloc.len() == DiskInode::total_blocks(size) as usize);
            for data_block in data_blocks_dealloc.into_iter() {
                fs.dealloc_data(data_block);
            }
        });
    }
}
