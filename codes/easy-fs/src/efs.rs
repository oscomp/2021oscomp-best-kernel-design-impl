use alloc::sync::Arc;
use spin::Mutex;
use super::{
    BlockDevice,
    Bitmap,
    SuperBlock,
    DiskInode,
    DiskInodeType,
    Inode,
    get_block_cache,
    DIRENT_SZ,
    DirEntry,
};
use crate::BLOCK_SZ;
use alloc::vec::Vec;

pub struct EasyFileSystem {
    pub block_device: Arc<dyn BlockDevice>,
    pub inode_bitmap: Bitmap,
    pub data_bitmap: Bitmap,
    inode_area_start_block: u32,
    data_area_start_block: u32,
}

type DataBlock = [u8; BLOCK_SZ];

impl EasyFileSystem {
    pub fn create(
        block_device: Arc<dyn BlockDevice>,
        total_blocks: u32,
        inode_bitmap_blocks: u32,
    ) -> Arc<Mutex<Self>> {
        // calculate block size of areas & create bitmaps
        let inode_bitmap = Bitmap::new(1, inode_bitmap_blocks as usize);
        let inode_num = inode_bitmap.maximum();
        // 存放inode数组的块数（向上对齐
        let inode_area_blocks =
            ((inode_num * core::mem::size_of::<DiskInode>() + BLOCK_SZ - 1) / BLOCK_SZ) as u32;
        let inode_total_blocks = inode_bitmap_blocks + inode_area_blocks;
        let data_total_blocks = total_blocks - 1 - inode_total_blocks;
        // 每个位图块对应512*8=4096个数据块
        // 因此理想情况为，4096 * data_bitmap_blocks + data_bitmap_blocks = data_total_blocks
        // 需要向上取整，否则有的数据块没有对应的位图，会被浪费！
        let data_bitmap_blocks = (data_total_blocks + 4096) / 4097;
        let data_area_blocks = data_total_blocks - data_bitmap_blocks;
        let data_bitmap = Bitmap::new(
            (1 + inode_bitmap_blocks + inode_area_blocks) as usize,
            data_bitmap_blocks as usize,
        );
        let mut efs = Self {
            block_device: Arc::clone(&block_device),
            inode_bitmap,
            data_bitmap,
            inode_area_start_block: 1 + inode_bitmap_blocks,
            data_area_start_block: 1 + inode_total_blocks + data_bitmap_blocks,
        };
        // clear all blocks
        for i in 0..total_blocks {
            get_block_cache(
                i as usize, 
                Arc::clone(&block_device)
            )
            .lock()
            .modify(0, |data_block: &mut DataBlock| {
                for byte in data_block.iter_mut() { *byte = 0; }
            });
        }
        // initialize SuperBlock
        get_block_cache(0, Arc::clone(&block_device))
        .lock()
        .modify(0, |super_block: &mut SuperBlock| {
            super_block.initialize(
                total_blocks,
                inode_bitmap_blocks,
                inode_area_blocks,
                data_bitmap_blocks,
                data_area_blocks,
                inode_num as u32,
                data_area_blocks,
            );
        });
        // write back immediately
        // create a inode for root node "/"
        assert_eq!(efs.alloc_inode(), 0);
        let (root_inode_block_id, root_inode_offset) = efs.get_disk_inode_pos(0);
        get_block_cache(
            root_inode_block_id as usize,
            Arc::clone(&block_device)
        )
        .lock()
        .modify(root_inode_offset, |disk_inode: &mut DiskInode| {
            disk_inode.initialize(DiskInodeType::Directory);
            // DEBUG: 创建根节点的..和.
            let new_size = 2 * DIRENT_SZ;
            // 分配数据块
            let mut v: Vec<u32> = Vec::new();
            for _ in 0..1 {
                v.push(efs.alloc_data());
            }
            disk_inode.increase_size(new_size as u32, v, &block_device);
            // root的../.均指向自身
            let dirent_self = DirEntry::new(".", DiskInodeType::Directory,0);
            let dirent_parent = DirEntry::new("..", DiskInodeType::Directory,0);
            disk_inode.write_at(
                0, 
                dirent_self.as_bytes(), 
                &block_device
            );
            disk_inode.write_at(
                DIRENT_SZ, 
                dirent_parent.as_bytes(), 
                &block_device
            );
            ///////
        });
        Arc::new(Mutex::new(efs))
    }

    pub fn open(block_device: Arc<dyn BlockDevice>) -> Arc<Mutex<Self>> {
        // read SuperBlock
        get_block_cache(0, Arc::clone(&block_device))
        .lock()
        .read(0, |super_block: &SuperBlock| {
            assert!(super_block.is_valid(), "Error loading EFS! magic = {}", super_block.magic);
            let inode_total_blocks =
                super_block.inode_bitmap_blocks + super_block.inode_area_blocks;
            let efs = Self {
                block_device,
                inode_bitmap: Bitmap::new(
                    1,
                    super_block.inode_bitmap_blocks as usize
                ),
                data_bitmap: Bitmap::new(
                    (1 + inode_total_blocks) as usize,
                    super_block.data_bitmap_blocks as usize,
                ),
                inode_area_start_block: 1 + super_block.inode_bitmap_blocks,
                data_area_start_block: 1 + inode_total_blocks + super_block.data_bitmap_blocks,
            };
            Arc::new(Mutex::new(efs))
        })        
    }

    pub fn get_inode( efs: &Arc<Mutex<Self>>, inode_id :u32) -> Inode {
        let block_device = Arc::clone(&efs.lock().block_device);
        Inode::new(
            inode_id,
            Arc::clone(efs),
            block_device,
        )
    }

    pub fn get_disk_inode_pos(&self, inode_id: u32) -> (u32, usize) {
        let inode_size = core::mem::size_of::<DiskInode>();
        let inodes_per_block = (BLOCK_SZ / inode_size) as u32;
        let block_id = self.inode_area_start_block + inode_id / inodes_per_block;
        (block_id, (inode_id % inodes_per_block) as usize * inode_size)
    }

    pub fn get_data_block_id(&self, data_block_id: u32) -> u32 {
        self.data_area_start_block + data_block_id
    }

    // Return ID in the inode area
    pub fn alloc_inode(&mut self) -> u32 {
        // DEGUB: 修改超级块
        get_block_cache(0, Arc::clone(&self.block_device))
        .lock()
        .modify(0, |super_block: &mut SuperBlock| {
            super_block.dec_inode();
        });
        self.inode_bitmap.alloc(&self.block_device).unwrap() as u32
    }

    /// Return a block ID not ID in the data area !
    pub fn alloc_data(&mut self) -> u32 {
        // DEGUB: 修改超级块
        get_block_cache(0, Arc::clone(&self.block_device))
        .lock()
        .modify(0, |super_block: &mut SuperBlock| {
            super_block.dec_block();
        });
        self.data_bitmap.alloc(&self.block_device).unwrap() as u32 + self.data_area_start_block
    }

    pub fn dealloc_data(&mut self, block_id: u32) {
        // DEGUB: 修改超级块
        get_block_cache(0, Arc::clone(&self.block_device))
        .lock()
        .modify(0, |super_block: &mut SuperBlock| {
            super_block.add_block();
        });
        get_block_cache(
            block_id as usize,
            Arc::clone(&self.block_device)
        )
        .lock()
        .modify(0, |data_block: &mut DataBlock| {
            data_block.iter_mut().for_each(|p| { *p = 0; })
        });
        self.data_bitmap.dealloc(
            &self.block_device,
            (block_id - self.data_area_start_block) as usize
        )
    }

    pub fn dealloc_inode(&mut self, inode_id: u32) {
        // 回收inode  
        // 因为初始化inode的时候会清零，所以这里没必要清零
        // DEGUB: 修改超级块
        get_block_cache(0, Arc::clone(&self.block_device))
        .lock()
        .modify(0, |super_block: &mut SuperBlock| {
            super_block.add_inode();
        });
        self.inode_bitmap.dealloc(
            &self.block_device, 
            inode_id as usize
        );
    }

    pub fn free_blocks(&self) -> u32{
        get_block_cache(0, Arc::clone(&self.block_device))
        .lock()
        .read(0, |super_block: &SuperBlock| {
            super_block.get_free_blocks()
        })
    }

    pub fn free_inodes(&self) -> u32{
        get_block_cache(0, Arc::clone(&self.block_device))
        .lock()
        .read(0, |super_block: &SuperBlock| {
            super_block.get_free_inodes()
        })
    }
}