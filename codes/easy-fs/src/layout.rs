use core::fmt::{Debug, Formatter, Result};
use super::{
    BLOCK_SZ,
    BlockDevice,
    get_block_cache,
};
use alloc::sync::Arc;
use alloc::vec::Vec;
use riscv::interrupt::free;

const EFS_MAGIC: u32 = 0x3b800001;
const INODE_DIRECT_COUNT: usize = 28;
pub const NAME_LENGTH_LIMIT: usize = 26;//27
const INODE_INDIRECT1_COUNT: usize = BLOCK_SZ / 4;
const INODE_INDIRECT2_COUNT: usize = INODE_INDIRECT1_COUNT * INODE_INDIRECT1_COUNT;
const DIRECT_BOUND: usize = INODE_DIRECT_COUNT;
const INDIRECT1_BOUND: usize = DIRECT_BOUND + INODE_INDIRECT1_COUNT;
#[allow(unused)]
const INDIRECT2_BOUND: usize = INDIRECT1_BOUND + INODE_INDIRECT2_COUNT;

#[repr(C)]
pub struct SuperBlock {
    // 可用空间：512B = 128 * u32
    pub magic: u32,
    pub total_blocks: u32,
    // 各个区域的长度
    pub inode_bitmap_blocks: u32,
    pub inode_area_blocks: u32,
    pub data_bitmap_blocks: u32,
    pub data_area_blocks: u32,
    // 余量
    pub free_inodes: u32,
    pub free_blocks: u32,
}

impl Debug for SuperBlock {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result {
        f.debug_struct("SuperBlock")
            .field("total_blocks", &self.total_blocks)
            .field("inode_bitmap_blocks", &self.inode_bitmap_blocks)
            .field("inode_area_blocks", &self.inode_area_blocks)
            .field("data_bitmap_blocks", &self.data_bitmap_blocks)
            .field("data_area_blocks", &self.data_area_blocks)
            .finish()
    }
}

impl SuperBlock {
    pub fn initialize(
        &mut self,
        total_blocks: u32,
        inode_bitmap_blocks: u32,
        inode_area_blocks: u32,
        data_bitmap_blocks: u32,
        data_area_blocks: u32,
        free_inodes: u32,
        free_blocks: u32,
    ) {
        *self = Self {
            magic: EFS_MAGIC,
            total_blocks,
            inode_bitmap_blocks,
            inode_area_blocks,
            data_bitmap_blocks,
            data_area_blocks,
            free_inodes,
            free_blocks,
        }
    }
    pub fn is_valid(&self) -> bool {
        self.magic == EFS_MAGIC
    }

    pub fn add_inode(&mut self) {
        self.free_inodes += 1;
    }
    
    pub fn dec_inode(&mut self) {
        self.free_inodes -= 1;
    }

    pub fn add_block(&mut self) {
        self.free_blocks += 1;
    }
    
    pub fn dec_block(&mut self) {
        self.free_blocks -= 1;
    }

    pub fn get_free_inodes(&self) -> u32{
        self.free_inodes
    }

    pub fn get_free_blocks(&self) -> u32{
        self.free_blocks
    }

}

#[derive(PartialEq,Copy,Clone)]
pub enum DiskInodeType {
    File,
    Directory,
}

type IndirectBlock = [u32; BLOCK_SZ / 4];
type DataBlock = [u8; BLOCK_SZ];

#[repr(C)]
pub struct DiskInode {
    // 最大16540个数据块
    // 
    pub size: u32, // Byte
    pub direct: [u32; INODE_DIRECT_COUNT], //28*512 = 14KB
    pub indirect1: u32,  // 128*512 = 64KB
    pub indirect2: u32,  // 128*128*512 = 8MB
    pub type_: DiskInodeType,
}

impl DiskInode {
    /// indirect1 and indirect2 block are allocated only when they are needed.
    pub fn initialize(&mut self, type_: DiskInodeType) {
        self.size = 0;
        self.direct.iter_mut().for_each(|v| *v = 0);
        self.indirect1 = 0;
        self.indirect2 = 0;
        self.type_ = type_;
    }
    pub fn is_dir(&self) -> bool {
        self.type_ == DiskInodeType::Directory
    }
    #[allow(unused)]
    pub fn is_file(&self) -> bool {
        self.type_ == DiskInodeType::File
    }
    /// Return block number correspond to size.
    pub fn data_blocks(&self) -> u32 {
        Self::_data_blocks(self.size)
    }
    fn _data_blocks(size: u32) -> u32 {
        (size + BLOCK_SZ as u32 - 1) / BLOCK_SZ as u32
    }
    /// Return number of blocks needed include indirect1/2.
    pub fn total_blocks(size: u32) -> u32 {
        let data_blocks = Self::_data_blocks(size) as usize;
        let mut total = data_blocks as usize;
        // indirect1
        if data_blocks > INODE_DIRECT_COUNT {
            total += 1;
        }
        // indirect2
        if data_blocks > INDIRECT1_BOUND {
            total += 1;
            // sub indirect1 (向上取整)
            total += (data_blocks - INDIRECT1_BOUND + INODE_INDIRECT1_COUNT - 1) / INODE_INDIRECT1_COUNT;
        }
        total as u32
    }
    // 计算扩容至new_size需要多少索引块
    pub fn blocks_num_needed(&self, new_size: u32) -> u32 {
        assert!(new_size >= self.size);
        Self::total_blocks(new_size) - Self::total_blocks(self.size)
    }
    pub fn get_block_id(&self, inner_id: u32, block_device: &Arc<dyn BlockDevice>) -> u32 {
        let inner_id = inner_id as usize;
        if inner_id < INODE_DIRECT_COUNT {
            self.direct[inner_id]
        } else if inner_id < INDIRECT1_BOUND {
            get_block_cache(self.indirect1 as usize, Arc::clone(block_device))
                .lock()
                .read(0, |indirect_block: &IndirectBlock| {
                    indirect_block[inner_id - INODE_DIRECT_COUNT]
                })
        } else {
            let last = inner_id - INDIRECT1_BOUND;
            let indirect1 = get_block_cache(
                self.indirect2 as usize,
                Arc::clone(block_device)
            )
            .lock()
            .read(0, |indirect2: &IndirectBlock| {
                indirect2[last / INODE_INDIRECT1_COUNT]
            });
            get_block_cache(
                indirect1 as usize,
                Arc::clone(block_device)
            )
            .lock()
            .read(0, |indirect1: &IndirectBlock| {
                indirect1[last % INODE_INDIRECT1_COUNT]
            })
        }
    }

    pub fn decrease_size(&mut self, new_size: u32, block_device: &Arc<dyn BlockDevice>) -> Option<Vec<u32>>{
        // 查看是否有可以回收的块
        // 有则将块号返回
        let mut old_blocks = self.data_blocks();
        self.size = new_size;
        let mut current_blocks = self.data_blocks();
        if current_blocks == old_blocks {
            return None;
        }
        let mut blk_vec:Vec<u32> = Vec::new();
        
        // 采用和分配一样的顺序，从前往后，这样效率高
        // 回收direct
        let mut ltimes = old_blocks.min(INODE_DIRECT_COUNT as u32);
        while current_blocks < ltimes {
            blk_vec.push(self.direct[current_blocks as usize]);
            // self.direct[blocks_needed as usize] = 0; // 没必要清零
            current_blocks += 1;
        }
        
        // 回收indirect1
        if old_blocks > INODE_DIRECT_COUNT as u32 {
            // 计算偏移量
            current_blocks -= INODE_DIRECT_COUNT as u32;
            old_blocks -= INODE_DIRECT_COUNT as u32;
        }else{
            return Some(blk_vec);
        }
        if current_blocks == 0 { // 回收一级间接块
            blk_vec.push(self.indirect1);
        }
        get_block_cache(
            self.indirect1 as usize,
            Arc::clone(block_device)
        )
        .lock()
        .modify(0, |indirect1: &mut IndirectBlock| {
            ltimes = old_blocks.min(INODE_INDIRECT1_COUNT as u32);
            while current_blocks < ltimes {
                blk_vec.push(indirect1[current_blocks as usize]);
                current_blocks += 1;
            }
        });

        // 回收indirect2
        if old_blocks > INODE_INDIRECT1_COUNT as u32 {
            old_blocks -= INODE_INDIRECT1_COUNT as u32;
            current_blocks -= INODE_INDIRECT1_COUNT as u32;
        } else {
            return Some(blk_vec);
        }
        if current_blocks == 0 { // 二级间接块需要回收
            blk_vec.push(self.indirect2);
        }
        // from (a0, b0) to (a1, b1)
        let mut a0 = current_blocks as usize / INODE_INDIRECT1_COUNT;
        let mut b0 = current_blocks as usize % INODE_INDIRECT1_COUNT;
        let a1 = old_blocks as usize / INODE_INDIRECT1_COUNT;
        let b1 = old_blocks as usize % INODE_INDIRECT1_COUNT;
        get_block_cache(
            self.indirect2 as usize,
            Arc::clone(block_device)
        )
        .lock()
        .modify(0, |indirect2: &mut IndirectBlock| {
            while (a0 < a1) || (a0 == a1 && b0 < b1) {
                // DEBUG
                if b0 == 0 { // 一级间接块需要回收
                    blk_vec.push(indirect2[a0]);
                }
                get_block_cache( 
                    indirect2[a0] as usize,
                    Arc::clone(block_device)
                )
                .lock()
                .modify(0,|indirect1:&mut IndirectBlock|{
                    //修改一级间接块
                    let ltimes = b1.min(INODE_INDIRECT1_COUNT);
                    while b0 < ltimes{
                        blk_vec.push(indirect1[b0]);
                        b0 += 1;
                    }
                });
                // TODO 
                if b0 == INODE_INDIRECT1_COUNT{
                    b0 = 0;
                }
                a0 += 1;  
            } 
        });
        Some(blk_vec)
    }
    
    // new_blocks: 保存本次容量扩充所需要块编号的向量
    // ?: new_blocks 究竟是什么结构，里面包含了为indirect分配的块？
    // 结论：根据blocks_num_needed得到需要的块数以确定
    pub fn increase_size(
        &mut self,
        new_size: u32,
        new_blocks: Vec<u32>,
        block_device: &Arc<dyn BlockDevice>,
    ) {
        let mut current_blocks = self.data_blocks();
        self.size = new_size;
        let mut total_blocks = self.data_blocks();
        let mut new_blocks = new_blocks.into_iter();
        // fill direct
        while current_blocks < total_blocks.min(INODE_DIRECT_COUNT as u32) {
            self.direct[current_blocks as usize] = new_blocks.next().unwrap();
            current_blocks += 1;
        }
        // alloc indirect1
        if total_blocks > INODE_DIRECT_COUNT as u32{
            if current_blocks == INODE_DIRECT_COUNT as u32 {
                // 说明indirect1还未被分配，因此分配一个块
                self.indirect1 = new_blocks.next().unwrap(); 
            }
            current_blocks -= INODE_DIRECT_COUNT as u32;
            total_blocks -= INODE_DIRECT_COUNT as u32;
        } else {
            return;
        }
        // fill indirect1
        get_block_cache(
            self.indirect1 as usize,
            Arc::clone(block_device)
        )
        .lock()
        .modify(0, |indirect1: &mut IndirectBlock| {
            while current_blocks < total_blocks.min(INODE_INDIRECT1_COUNT as u32) {
                indirect1[current_blocks as usize] = new_blocks.next().unwrap();
                current_blocks += 1;
            }
        });
        // alloc indirect2
        if total_blocks > INODE_INDIRECT1_COUNT as u32 {
            if current_blocks == INODE_INDIRECT1_COUNT as u32 {
                self.indirect2 = new_blocks.next().unwrap();
            }
            current_blocks -= INODE_INDIRECT1_COUNT as u32;
            total_blocks -= INODE_INDIRECT1_COUNT as u32;
        } else {
            return;
        }
        // fill indirect2 from (a0, b0) -> (a1, b1)
        let mut a0 = current_blocks as usize / INODE_INDIRECT1_COUNT;
        let mut b0 = current_blocks as usize % INODE_INDIRECT1_COUNT;
        let a1 = total_blocks as usize / INODE_INDIRECT1_COUNT;
        let b1 = total_blocks as usize % INODE_INDIRECT1_COUNT;
        // alloc low-level indirect1
        get_block_cache(
            self.indirect2 as usize,
            Arc::clone(block_device)
        )
        .lock()
        .modify(0, |indirect2: &mut IndirectBlock| {
            while (a0 < a1) || (a0 == a1 && b0 < b1) {
                if b0 == 0 {
                    // 先分配一个一级索引块
                    indirect2[a0] = new_blocks.next().unwrap();
                }
                // fill current
                get_block_cache(
                    indirect2[a0] as usize,
                    Arc::clone(block_device)
                )
                .lock()
                .modify(0, |indirect1: &mut IndirectBlock| {
                    //填写一级索引块
                    indirect1[b0] = new_blocks.next().unwrap();
                });
                // move to next
                b0 += 1;
                if b0 == INODE_INDIRECT1_COUNT {
                    b0 = 0;
                    a0 += 1;
                }
            } 
        });
    }

    /// Clear size to zero and return blocks that should be deallocated.
    ///
    /// We will clear the block contents to zero later.
    pub fn clear_size(&mut self, block_device: &Arc<dyn BlockDevice>) -> Vec<u32> {
        let mut v: Vec<u32> = Vec::new();
        let mut data_blocks = self.data_blocks() as usize;
        self.size = 0;

        //统计当前回收的块数
        let mut current_blocks = 0usize;
        // direct
        while current_blocks < data_blocks.min(INODE_DIRECT_COUNT) {
            v.push(self.direct[current_blocks]);
            self.direct[current_blocks] = 0;
            current_blocks += 1;
        }
        // indirect1 block
        if data_blocks > INODE_DIRECT_COUNT {
            v.push(self.indirect1);
            data_blocks -= INODE_DIRECT_COUNT;
            current_blocks = 0;
        } else {
            return v;
        }
        // indirect1
        get_block_cache(
            self.indirect1 as usize,
            Arc::clone(block_device),
        )
        .lock()
        .modify(0, |indirect1: &mut IndirectBlock| {
            while current_blocks < data_blocks.min(INODE_INDIRECT1_COUNT) {
                v.push(indirect1[current_blocks]);
                //indirect1[current_blocks] = 0;
                current_blocks += 1;
            }
        });
        self.indirect1 = 0;
        // indirect2 block
        if data_blocks > INODE_INDIRECT1_COUNT {
            v.push(self.indirect2);
            data_blocks -= INODE_INDIRECT1_COUNT;
        } else {
            return v;
        }
        // indirect2
        assert!(data_blocks <= INODE_INDIRECT2_COUNT);
        let a1 = data_blocks / INODE_INDIRECT1_COUNT;
        let b1 = data_blocks % INODE_INDIRECT1_COUNT;
        get_block_cache(
            self.indirect2 as usize,
            Arc::clone(block_device),
        )
        .lock()
        .modify(0, |indirect2: &mut IndirectBlock| {
            // full indirect1 blocks
            for i in 0..a1 {
                v.push(indirect2[i]);
                get_block_cache(
                    indirect2[i] as usize,
                    Arc::clone(block_device),
                )
                .lock()
                .modify(0, |indirect1: &mut IndirectBlock| {
                    // 前几个一级索引都是占满的
                    // 因此每个都要回收INODE_INDIRECT1_COUNT块
                    for j in 0..INODE_INDIRECT1_COUNT {
                        v.push(indirect1[j]);
                        //indirect1[j] = 0;
                    }
                });
                //indirect2[i] = 0;
            }
            // last indirect1 block
            if b1 > 0 {
                // 这种情况下，实际是第a1+1个一级索引块
                v.push(indirect2[a1]);
                get_block_cache(
                    indirect2[a1] as usize,
                    Arc::clone(block_device),
                )
                .lock()
                .modify(0, |indirect1: &mut IndirectBlock| {
                    for j in 0..b1 {
                        v.push(indirect1[j]);
                        //indirect1[j] = 0;
                    }
                });
                //indirect2[a1] = 0;
            }
        });
        self.indirect2 = 0;
        v
    }
    pub fn read_at(
        &self,
        offset: usize,
        buf: &mut [u8],
        block_device: &Arc<dyn BlockDevice>,
    ) -> usize {
        let mut start = offset;
        //println!("size = {}",self.size);        
        let end = (offset + buf.len()).min(self.size as usize);
        if start >= end {
            return 0;
        }
        let mut start_block = start / BLOCK_SZ;
        let mut read_size = 0usize;
        loop {
            // calculate end of current block
            // 指文件从头开始到当前块末尾的大小（实际上也是向上对齐
            // 但是不同于前面，这里是对齐的是索引
            // 例如前面，size = 512, 则应该算在一个块
            // 而这里，块内地址范围为0-511，512属于下一个块
            let mut end_current_block = (start / BLOCK_SZ + 1) * BLOCK_SZ;
            end_current_block = end_current_block.min(end);

            // read and update read size
            let block_read_size = end_current_block - start;
            let dst = &mut buf[read_size..read_size + block_read_size];
            get_block_cache(
                self.get_block_id(start_block as u32, block_device) as usize,
                Arc::clone(block_device),
            )
            .lock()
            .read(0, |data_block: &DataBlock| {
                let src = &data_block[start % BLOCK_SZ..start % BLOCK_SZ + block_read_size];
                dst.copy_from_slice(src);
            });
            read_size += block_read_size;
            // move to next block
            if end_current_block == end { break; }
            start_block += 1;
            start = end_current_block;
        }
        read_size
    }
    /// File size must be adjusted before!
    /// Users should call increase_size for that
    pub fn write_at(
        &mut self,
        offset: usize,
        buf: &[u8],
        block_device: &Arc<dyn BlockDevice>,
    ) -> usize {
        let mut start = offset;
        let end = (offset + buf.len()).min(self.size as usize);
        assert!(start <= end);
        let mut start_block = start / BLOCK_SZ;
        let mut write_size = 0usize;
        loop {
            // calculate end of current block
            let mut end_current_block = (start / BLOCK_SZ + 1) * BLOCK_SZ;
            end_current_block = end_current_block.min(end);
            // write and update write size
            let block_write_size = end_current_block - start;
            get_block_cache(
                self.get_block_id(start_block as u32, block_device) as usize,
                Arc::clone(block_device)
            )
            .lock()
            .modify(0, |data_block: &mut DataBlock| {
                let src = &buf[write_size..write_size + block_write_size];
                let dst = &mut data_block[start % BLOCK_SZ..start % BLOCK_SZ + block_write_size];
                dst.copy_from_slice(src);
            });
            write_size += block_write_size;
            // move to next block
            if end_current_block == end { break; }
            start_block += 1;
            start = end_current_block;
        }
        write_size
    }
}

#[repr(C)]
pub struct DirEntry {
    // +1 is for '\0'
    // 大小为32B
    // 规定：如果当前目录项为空，则name[0] = 0
    // 这样效率比较高
    name: [u8; NAME_LENGTH_LIMIT + 1],
    type_: DiskInodeType,
    inode_number: u32,
}

pub const DIRENT_SZ: usize = 32;

impl DirEntry {
    pub fn empty() -> Self {
        Self {
            name: [0u8; NAME_LENGTH_LIMIT + 1],
            type_: DiskInodeType::File,
            inode_number: 0,
        }
    }
    pub fn new(name: &str, type_: DiskInodeType, inode_number: u32) -> Self {
        let mut bytes = [0u8; NAME_LENGTH_LIMIT + 1];
        &mut bytes[..name.len()].copy_from_slice(name.as_bytes());
        Self {
            name: bytes,
            type_,
            inode_number,
        }
    }
    pub fn as_bytes(&self) -> &[u8] {
        unsafe {
            core::slice::from_raw_parts(
                self as *const _ as usize as *const u8,
                DIRENT_SZ,
            )
        }
    }
    pub fn as_bytes_mut(&mut self) -> &mut [u8] {
        unsafe {
            core::slice::from_raw_parts_mut(
                self as *mut _ as usize as *mut u8,
                DIRENT_SZ,
            )
        }
    }
    pub fn name(&self) -> &str {
        let len = (0usize..).find(|i| self.name[*i] == 0).unwrap();
        core::str::from_utf8(&self.name[..len]).unwrap()
    }
    
    pub fn type_(&self) -> DiskInodeType {
        self.type_
    }

    pub fn inode_number(&self) -> u32 {
        self.inode_number
    }
}