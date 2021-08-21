use super::{
    BLOCK_SZ,
    BlockDevice,
};
use alloc::collections::VecDeque;
use alloc::sync::Arc;
use lazy_static::*;
use spin::RwLock;
#[allow(unused)]
use riscv::register::time;

pub struct BlockCache {
    pub cache: [u8; BLOCK_SZ],
    block_id: usize,
    block_device: Arc<dyn BlockDevice>,
    modified: bool,
    #[allow(unused)]
    time_stamp: usize,
}

impl BlockCache {
    /// Load a new BlockCache from disk.
    pub fn new(
        block_id: usize, 
        block_device: Arc<dyn BlockDevice>
    ) -> Self {
        let mut cache = [0u8; BLOCK_SZ];
        //println!("cache new: blk_id = {}", block_id);
        block_device.read_block(block_id, &mut cache);
        // TODO: 时间戳
        //let mut time_stamp = time::read();
        let time_stamp = 0;
        Self {
            cache,
            block_id,
            block_device,
            modified: false,
            time_stamp,
        }
    }

    fn addr_of_offset(&self, offset: usize) -> usize {
        &self.cache[offset] as *const _ as usize
    }

    pub fn get_ref<T>(&self, offset: usize) -> &T where T: Sized {
        let type_size = core::mem::size_of::<T>();
        assert!(offset + type_size <= BLOCK_SZ);
        let addr = self.addr_of_offset(offset);
        unsafe { &*(addr as *const T) } 
    }

    pub fn get_mut<T>(&mut self, offset: usize) -> &mut T where T: Sized {
        let type_size = core::mem::size_of::<T>();
        assert!(offset + type_size <= BLOCK_SZ);
        self.modified = true;
        let addr = self.addr_of_offset(offset);
        unsafe { &mut *(addr as *mut T) }
    }

    pub fn read<T, V>(&self, offset: usize, f: impl FnOnce(&T) -> V) -> V {
        f(self.get_ref(offset))
    }

    pub fn modify<T, V>(&mut self, offset:usize, f: impl FnOnce(&mut T) -> V) -> V {
        f(self.get_mut(offset))
    }

    pub fn sync(&mut self) {
        if self.modified {
            //println!("drop cache, id = {}", self.block_id);
            self.modified = false;
            self.block_device.write_block(self.block_id, &self.cache);
        }
    }
}

impl Drop for BlockCache {
    fn drop(&mut self) {
        self.sync()
    }
}


// 0-info扇区
// 1-2 FAT1
// 3-4 FAT2
// 5-7 DirEntry
// 8-19 DATA

const BLOCK_CACHE_SIZE: usize = 20;
//const DIRENT_CACHE_SIZE: usize = 4;
pub struct BlockCacheManager {
    start_sec: usize,
    limit: usize,
    queue: VecDeque<(usize, Arc<RwLock<BlockCache>>)>,
}

impl BlockCacheManager {
    pub fn new(limit:usize) -> Self {
        Self { 
            start_sec: 0,
            limit,
            queue: VecDeque::new() 
        }
    }

    pub fn set_start_sec(&mut self, new_start_sec: usize){
        self.start_sec = new_start_sec;
    }

    pub fn get_start_sec(&self)->usize {
        self.start_sec
    }

    pub fn read_block_cache(
        &self,
        block_id: usize,
        //block_device: Arc<dyn BlockDevice>,
    ) -> Option<Arc<RwLock<BlockCache>>>{
        if let Some(pair) = self.queue
            .iter()
            .find(|pair| pair.0 == block_id) {
                Some(Arc::clone(&pair.1))
        }else{
            None
            // substitute
            // if self.queue.len() == BLOCK_CACHE_SIZE {
            //     // from front to tail
            //     if let Some((idx, _)) = self.queue
            //         .iter()
            //         .enumerate()
            //         .find(|(_, pair)| Arc::strong_count(&pair.1) == 1) {
            //         self.queue.drain(idx..=idx);
            //     } else {
            //         panic!("Run out of BlockCache!");
            //     }
            // }
            // // load block into mem and push back
            // let block_cache = Arc::new(RwLock::new(
            //     BlockCache::new(block_id, Arc::clone(&block_device))
            // ));
            // self.queue.push_back((block_id, Arc::clone(&block_cache)));
            // //println!("blkcache: {:?}", block_cache.read().cache);
            // Some(block_cache)
        }
    }

    pub fn get_block_cache(
        &mut self,
        block_id: usize,
        block_device: Arc<dyn BlockDevice>,
    ) -> Arc<RwLock<BlockCache>> {
        if let Some(pair) = self.queue
            .iter()
            .find(|pair| pair.0 == block_id) {
                Arc::clone(&pair.1)
        } else {
            // substitute
            if self.queue.len() == self.limit/*BLOCK_CACHE_SIZE*/ {
                // from front to tail
                if let Some((idx, _)) = self.queue
                    .iter()
                    .enumerate()
                    .find(|(_, pair)| Arc::strong_count(&pair.1) == 1) {
                    self.queue.drain(idx..=idx);
                } else {
                    panic!("Run out of BlockCache!");
                }
            }
            // load block into mem and push back
            let block_cache = Arc::new(RwLock::new(
                BlockCache::new(block_id, Arc::clone(&block_device))
            ));
            self.queue.push_back((block_id, Arc::clone(&block_cache)));
            //println!("blkcache: {:?}", block_cache.read().cache);
            block_cache
        }
    }

    pub fn drop_all(&mut self){
        self.queue.clear();
    }
}



lazy_static! {
    pub static ref DATA_BLOCK_CACHE_MANAGER: RwLock<BlockCacheManager> = RwLock::new(
        BlockCacheManager::new(1034)
    );
}

lazy_static! {
    pub static ref INFO_CACHE_MANAGER: RwLock<BlockCacheManager> = RwLock::new(
        BlockCacheManager::new(10)
    );
}

#[derive(PartialEq,Copy,Clone,Debug)]
pub enum CacheMode {
    READ,
    WRITE,
}

/* 仅用于访问文件数据块，不包括目录项 */
pub fn get_block_cache(
    block_id: usize,
    block_device: Arc<dyn BlockDevice>,
    rw_mode: CacheMode,
) -> Arc<RwLock<BlockCache>> {
    let phy_blk_id = DATA_BLOCK_CACHE_MANAGER.read().get_start_sec() + block_id;
    if rw_mode == CacheMode::READ {
        // make sure the blk is in cache
        if let Some(blk) = INFO_CACHE_MANAGER.read().read_block_cache(phy_blk_id){
            return blk
        }
        DATA_BLOCK_CACHE_MANAGER.write().get_block_cache(phy_blk_id, block_device);
        DATA_BLOCK_CACHE_MANAGER.read().read_block_cache(phy_blk_id).unwrap()
    } else {
        if let Some(blk) = INFO_CACHE_MANAGER.read().read_block_cache(phy_blk_id){
            return blk
        }
        DATA_BLOCK_CACHE_MANAGER.write().get_block_cache(phy_blk_id, block_device)
    }
}

/* 用于访问保留扇区，以及目录项 */
pub fn get_info_cache(
    block_id: usize,
    block_device: Arc<dyn BlockDevice>,
    rw_mode: CacheMode,
) -> Arc<RwLock<BlockCache>> {
    let phy_blk_id = INFO_CACHE_MANAGER.read().get_start_sec() + block_id;
    if rw_mode == CacheMode::READ {
        // make sure the blk is in cache
        if let Some(blk) = DATA_BLOCK_CACHE_MANAGER.read().read_block_cache(phy_blk_id){
            return blk
        }
        INFO_CACHE_MANAGER.write().get_block_cache(phy_blk_id, block_device);
        INFO_CACHE_MANAGER.read().read_block_cache(phy_blk_id).unwrap()
    } else {
        if let Some(blk) = DATA_BLOCK_CACHE_MANAGER.read().read_block_cache(phy_blk_id){
            return blk
        }
        INFO_CACHE_MANAGER.write().get_block_cache(phy_blk_id, block_device)
    }
}

pub fn set_start_sec(start_sec: usize){
    INFO_CACHE_MANAGER.write().set_start_sec(start_sec);
    DATA_BLOCK_CACHE_MANAGER.write().set_start_sec(start_sec);
}

pub fn write_to_dev(){  
    INFO_CACHE_MANAGER.write().drop_all();
    DATA_BLOCK_CACHE_MANAGER.write().drop_all();
}   


/* 
pub fn get_dirent_block_cache(
    block_id: usize,
    block_device: Arc<dyn BlockDevice>
) -> Arc<Mutex<BlockCache>> {
    DATA_BLOCK_CACHE_MANAGER.lock().get_block_cache(block_id, block_device)
}
*/


/* 
enum CacheType {
    INFO,
    FAT1,
    FAT2,
    DIRENT,
    DATA,
}

fn type_to_range(type_: CacheType)->Range<usize>{
    match type_ {
        CacheType::INFO => {0..1}
        CacheType::FAT1 => {1..3}
        CacheType::FAT2 => {3..5}
        CacheType::DIRENT => {5..8}
        CacheType::DATA => {8..19}
    }      
}*/

