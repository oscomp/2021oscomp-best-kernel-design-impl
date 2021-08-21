use super::{
    BlockDevice,
    fat32_manager::*,
    layout::*,
    get_info_cache,
    CacheMode,
    println,
    //print
};
use alloc::sync::Arc;
use alloc::string::String;
use alloc::vec::Vec;
use spin::RwLock;


#[derive(Clone)]
pub struct VFile {
    name:String,
    short_sector: usize, 
    short_offset: usize, //文件短目录项所在扇区和偏移
    long_pos_vec: Vec<(usize, usize)>, // 长目录项的位置<sector, offset>
    //first_cluster: u32,
    attribute:u8,
    //size:u32,
    fs: Arc<RwLock<FAT32Manager>>,
    block_device: Arc<dyn BlockDevice>,
}

// QUES 文件的复制、移动、删除(尤其目录)的加锁方式需要考证。。。
// 理论上，操作的时候通过cache锁short_dirent就有很好的效果

// 以数组的形式读目录项 QUES:是否有更好的？
// type DirEntryArray = [ShortDirEntry; 16]; // 一般是16...

impl VFile{
    pub fn new(
        name:String,
        short_sector: usize, 
        short_offset: usize,
        long_pos_vec:Vec<(usize, usize)>, 
        //first_cluster: u32,
        attribute:u8,
        size:u32,
        fs: Arc<RwLock<FAT32Manager>>,
        block_device: Arc<dyn BlockDevice>
    )->Self{
        Self {
            name,
            short_sector, 
            short_offset, 
            long_pos_vec,
            //first_cluster,
            attribute,
            //size,
            fs,
            block_device
        }
    }

    pub fn clear_cache(&self){
        let fat = self.fs.read();
        fat.cache_write_back();
    }

    pub fn get_name(&self)->&str {
        self.name.as_str()
    }

    pub fn get_attribute(&self)->u8{
        self.attribute
    }

    pub fn get_size(&self)->u32{
        self.read_short_dirent(|se:&ShortDirEntry|{
            se.get_size()
        })
    }

    pub fn get_fs(&self) -> Arc<RwLock<FAT32Manager>>{
        self.fs.clone()
    }

    pub fn is_dir(&self)->bool{
        if 0 != (self.attribute & ATTRIBUTE_DIRECTORY) {
            true
        }else{
            false   
        }
    }

    pub fn is_short(&self)->bool {
        if self.long_pos_vec.len() == 0 {
            true
        } else {
            false
        }
    }

    pub fn read_short_dirent<V>(&self, f: impl FnOnce(&ShortDirEntry) -> V)->V{ 
        if self.short_sector == 0 {
            let root_dirent = self.fs.read().get_root_dirent();
            let rr = root_dirent.read();
            f(& rr)
        } else {
            get_info_cache(
                self.short_sector,
                self.block_device.clone(),
                CacheMode::READ
            ).read().read(self.short_offset, f)
        }
    }

    fn modify_long_dirent<V>(&self, index:usize ,f: impl FnOnce(&mut LongDirEntry) -> V)->V {
        let (sector, offset) = self.long_pos_vec[index];
        get_info_cache(
            sector,
            self.block_device.clone(),
            CacheMode::READ
        ).write().modify(offset, f)
    }

    pub fn modify_short_dirent<V>(&self, f: impl FnOnce(&mut ShortDirEntry) -> V)->V{
        if self.short_sector == 0 {
            //println!("[fs]: modify vroot dent");
            let root_dirent = self.fs.read().get_root_dirent();
            let mut rw = root_dirent.write();
            f(&mut rw)
        } else {
            get_info_cache(
                self.short_sector,
                self.block_device.clone(),
                CacheMode::READ
            ).write().modify(self.short_offset, f)
        }   
    }

    /* 返回sector和offset */
    pub fn get_pos(&self, offset:usize) -> ( usize, usize){
        let (_, sec, off) = self.read_short_dirent(|s_ent: &ShortDirEntry|{
            s_ent.get_pos(
                offset, 
                &self.fs, 
                &self.fs.read().get_fat(), 
                &self.block_device)
        });
        (sec, off)
    }

    fn find_long_name(
        &self, 
        name: &str,
        dir_ent: &ShortDirEntry
    )->Option<VFile>{
        let name_vec = self.fs.read().long_name_split(name);
        let mut offset:usize = 0;
        let mut long_ent = LongDirEntry::empty();
        let long_ent_num = name_vec.len();
        let mut long_pos_vec:Vec<(usize, usize)> = Vec::new(); 
        let name_last =  name_vec[long_ent_num-1].clone();
        let mut step:usize = long_ent_num;
        for i in (long_ent_num - 2) ..0 {
            if name_last == name_vec[i] {
                step = step - i - 1;
                break;
            }
        }
        step = 1;
        loop{
            long_pos_vec.clear();
            // 读取offset处的目录项
            let mut read_sz = dir_ent.read_at(
                offset, 
                long_ent.as_bytes_mut(), 
                &self.fs, 
                &self.fs.read().get_fat(), 
                &self.block_device
            );
            if read_sz != DIRENT_SZ || long_ent.is_empty() {return None}
            if long_ent.get_name_raw() == name_last
            && long_ent.attribute() == ATTRIBUTE_LFN {
                // 匹配：如果名一致，且第一字段为0x4*，获取该order，以及校验和
                let mut order = long_ent.get_order();
                let l_checksum = long_ent.get_checksum(); 
                if order & 0x40 == 0 || order == 0xE5 { offset += step * DIRENT_SZ; continue; }
                order = order ^ 0x40;
                if order as usize != long_ent_num { offset += step * DIRENT_SZ; continue; }
                // 如果order也匹配，开一个循环继续匹配长名目录项
                let mut is_match = true;
                for i in 1..order as usize {
                    read_sz = dir_ent.read_at(
                        offset + i * DIRENT_SZ, 
                        long_ent.as_bytes_mut(), 
                        &self.fs, 
                        &self.fs.read().get_fat(), 
                        &self.block_device
                    );
                    if read_sz != DIRENT_SZ {return None}
                    if long_ent.get_name_raw() != name_vec[ long_ent_num - 1 - i ] 
                    || long_ent.attribute() != ATTRIBUTE_LFN {
                        is_match = false;
                        break;
                    }
                }
                if is_match {  // 如果成功，读短目录项，进行校验
                    let mut short_ent = ShortDirEntry::empty();
                    let s_off = offset + long_ent_num * DIRENT_SZ;
                    read_sz = dir_ent.read_at(
                        s_off, 
                        short_ent.as_bytes_mut(), 
                        &self.fs, 
                        &self.fs.read().get_fat(), 
                        &self.block_device
                    );
                    if read_sz != DIRENT_SZ {return None}
                    if short_ent.is_valid() && l_checksum == short_ent.checksum(){
                        let ( short_sector, short_offset) = self.get_pos(s_off);
                        for i in 0..order as usize { // 存入长名目录项位置了，第一个在栈顶
                            let pos = self.get_pos(offset + i);
                            long_pos_vec.push(pos);
                        }
                        return Some(
                            VFile::new(
                                String::from(name),
                                short_sector, 
                                short_offset, 
                                long_pos_vec,
                                //short_ent.first_cluster(),
                                short_ent.attribute(),
                                short_ent.get_size(),
                                self.fs.clone(),
                                self.block_device.clone(),
                            )
                        )
                    } else {
                        return None // QUES
                    }
                } else {
                    offset += step * DIRENT_SZ;
                    continue;
                }
            } else {
                offset+= step * DIRENT_SZ;
            }
        }
    }

    fn find_short_name(
        &self, 
        name:&str, 
        dir_ent: &ShortDirEntry
    ) -> Option<VFile> {
        let name_upper = name.to_ascii_uppercase();     
        let mut short_ent = ShortDirEntry::empty();
        let mut offset = 0;
        let mut read_sz:usize;
        loop {
            read_sz = dir_ent.read_at(
                offset, 
                short_ent.as_bytes_mut(), 
                &self.fs, 
                &self.fs.read().get_fat(), 
                &self.block_device
            );
            if read_sz != DIRENT_SZ || short_ent.is_empty() {
                return None
            }else{
                if short_ent.is_valid() && name_upper == short_ent.get_name_uppercase() {
                    let (short_sector, short_offset) = self.get_pos(offset);
                    let long_pos_vec:Vec<(usize, usize)> = Vec::new(); 
                    return Some(
                        VFile::new(
                            String::from(name),
                            short_sector, 
                            short_offset, 
                            long_pos_vec,
                            short_ent.attribute(),
                            short_ent.get_size(),
                            self.fs.clone(),
                            self.block_device.clone(),
                        )
                    )
                } else {
                    offset += DIRENT_SZ;
                    continue;
                }
            }
        }
    }

    /* 根据名称搜索当前目录下的文件 */
    pub fn find_vfile_byname(
        &self,
        name: &str,
    ) -> Option<VFile> { 
        assert!( self.is_dir() );
        let mut name_and_ext: Vec<&str> = name.split(".").collect();
        let name_ = name_and_ext[0].as_bytes();
        if name_and_ext.len() == 1 {
            name_and_ext.push("");
        } 
        let ext_ = name_and_ext[1].as_bytes();
        // FAT32目录没有大小，只能搜，read_at已经做了完善的适配
        self.read_short_dirent(|short_ent:&ShortDirEntry|{
            if name_.len() > 8 || ext_.len() > 3 { //长文件名
                return self.find_long_name(name, short_ent)
            } else { // 短文件名
                return self.find_short_name(name, short_ent) 
            }
        })
    }
    
    /* 根据路径递归搜索文件 */
    pub fn find_vfile_bypath(&self, path: Vec<&str>)-> Option<Arc<VFile>>{
        let _ = self.fs.read(); // 获取读锁
        let len = path.len();
        if len == 0{
            return Some( Arc::new(self.clone()) );
        }
        let mut current_vfile = self.clone();
        for i in 0 .. len {
            // DEBUG
            // print!("\n");
            if path[i] == "" || path[i] == "."{
                continue;
            }
            if let Some(vfile) = current_vfile.find_vfile_byname(path[i]) {
                current_vfile = vfile;
            }else{
                return None;
            }
        }
        Some(Arc::new(current_vfile))
    }

    /* WAITING 既然目录都没有大小，那暂时没必要做这个 */
    #[allow(unused)]
    fn decrease_size(){

    }

    fn increase_size(
        & self,
        new_size: u32,
    ) {  // TODO: return sth when cannot increase
        //println!("===================== in increase =======================");
        //println!("file: {}, newsz = {}", self.get_name(), new_size);
        //println!("try lock");
        let first_cluster = self.first_cluster();
        let old_size = self.get_size();
        let manager_writer = self.fs.write();
        //println!("get lock");
        if new_size <= old_size {
            //println!("oldsz > newsz");
            return;
        }
        let needed = manager_writer.cluster_num_needed(old_size, new_size, self.is_dir(), first_cluster);
        //println!("needed = {}", needed);
        if needed == 0{
            if !self.is_dir() {
                //self.size = new_size;
                self.modify_short_dirent(|se:&mut ShortDirEntry|{
                    se.set_size(new_size);
                });
            }  
            return;
        }   
        
        //println!("first cluster = {} nxt = {}", first_cluster, manager_writer.get_fat().read().get_next_cluster(first_cluster, self.block_device.clone()));
        if let Some(cluster) = manager_writer.alloc_cluster(needed) {
            //println!("*** cluster alloc = {}",cluster);
            if first_cluster == 0 { //未分配簇
                drop(manager_writer);
                self.modify_short_dirent(|se:&mut ShortDirEntry|{
                    se.set_first_cluster(cluster);
                });
                //println!("fc = {}",self.first_cluster());
                //println!("================== increase end ====================");
            }else{  // 已经分配簇
                //let fs_reader = self.fs.read();
                //println!("[fs-inc]: file: {}, newsz = {}", self.get_name(), new_size);
                //println!("  cluster alloc = {}",cluster);
                let fat = manager_writer.get_fat();
                //println!("try lock1");
                let fat_writer = fat.write();
                //println!("get lock1");
                let final_cluster = fat_writer.final_cluster(first_cluster, self.block_device.clone());
                assert_ne!( cluster, 0);
                fat_writer.set_next_cluster(final_cluster, cluster, self.block_device.clone());
                //let allc = fat_writer.get_all_cluster_of(first_cluster, self.block_device.clone());
                //println!("  finish set next cluster, cluster chain:{:?}", allc);
                drop(manager_writer);
            }
            //self.size = new_size;
            self.modify_short_dirent(|se:&mut ShortDirEntry|{
                se.set_size(new_size);
            });
        } else {
            panic!("SD Card no space!!!");
        }
    }

    /* 
    pub fn set_first_cluster(&mut self, first_cluster:u32) {
        self.first_cluster = first_cluster;
        self.modify_short_dirent(|se:&mut ShortDirEntry|{
            se.set_first_cluster(first_cluster);
        });
    }*/

    /* 在当前目录下创建文件 */ 
    pub fn create(& self, name: &str, attribute: u8) -> Option<Arc<VFile>> {
        // 检测同名文件
        assert!(self.is_dir());
        //if self.find_vfile_byname(name).is_some() {  
        //    //println!("already exist：{}",name);
        //    return None
        //}
        let manager_reader = self.fs.read();
        let (name_, ext_) = manager_reader.split_name_ext(name);
        // 搜索空处
        let mut dirent_offset:usize;
        if let Some(offset) = self.find_free_dirent(){
            dirent_offset = offset;
        } else {
            return None
        }
        let mut short_ent = ShortDirEntry::empty();
        if name_.len() > 8 || ext_.len() > 3 { 
            // 长文件名拆分
            let mut v_long_name = manager_reader.long_name_split(name);
            let long_ent_num = v_long_name.len();
            let mut long_ent = LongDirEntry::empty();
            // 生成短文件名及对应目录项
            let short_name = manager_reader.generate_short_name(name);
            let (name_bytes, ext_bytes) = manager_reader.short_name_format(short_name.as_str());
            short_ent.initialize(&name_bytes, &ext_bytes, attribute);
            let check_sum = short_ent.checksum();
            //println!("*** aft checksum");
            drop(manager_reader);
            // 写长名目录项
            for i in 0..long_ent_num {
                let mut order:u8 = (long_ent_num - i) as u8;
                if i == 0 {
                    order |= 0x40;
                }
                long_ent.initialize(
                    v_long_name.pop().unwrap().as_bytes(), 
                    order, 
                    check_sum
                );
                assert_eq!(
                    self.write_at(dirent_offset, long_ent.as_bytes_mut()),
                    DIRENT_SZ
                );
                dirent_offset += DIRENT_SZ;
            }
        } else { // 短文件名格式化
            let (name_bytes, ext_bytes) = manager_reader.short_name_format(name);
            short_ent.initialize(&name_bytes, &ext_bytes, attribute);
            short_ent.set_case(ALL_LOWER_CASE);
            drop(manager_reader);
        }
        // 写短目录项
        assert_eq!(
            self.write_at(dirent_offset, short_ent.as_bytes_mut()),
            DIRENT_SZ
        );
        
        // 如果是目录类型，需要创建.和..

        if let Some(vfile) = self.find_vfile_byname(name){
            if attribute & ATTRIBUTE_DIRECTORY != 0 {
                let manager_reader = self.fs.read();
                let (name_bytes,ext_bytes) = manager_reader.short_name_format(".");
                let mut self_dir = ShortDirEntry::new(&name_bytes,&ext_bytes, ATTRIBUTE_DIRECTORY);
                let (name_bytes,ext_bytes) = manager_reader.short_name_format("..");
                let mut par_dir = ShortDirEntry::new(&name_bytes,&ext_bytes, ATTRIBUTE_DIRECTORY);
                drop(manager_reader);
                par_dir.set_first_cluster(self.first_cluster());

                vfile.write_at(0, self_dir.as_bytes_mut());
                vfile.write_at(DIRENT_SZ, par_dir.as_bytes_mut());
                let first_cluster = vfile.read_short_dirent(|se: &ShortDirEntry|{
                    se.first_cluster()
                }); 
                self_dir.set_first_cluster(first_cluster);
                vfile.write_at(0, self_dir.as_bytes_mut());
            }
            return Some(Arc::new(vfile))
        } else {
            None
        }   
    }


    pub fn first_cluster(&self)->u32{
        self.read_short_dirent(|se:& ShortDirEntry|{
            se.first_cluster()
        })
    }

    pub fn set_first_cluster(&self, clu:u32){
        self.modify_short_dirent(|se:&mut ShortDirEntry|{
            se.set_first_cluster(clu);
        })
    }

    /* 获取当前目录下的所有文件名以及属性，以Vector形式返回 */
    // 如果出现错误，返回None
    pub fn ls(&self)-> Option<Vec<(String, u8)>>{   
        if !self.is_dir() {
            return None
        } 
        let mut list:Vec<(String, u8)> = Vec::new();
        // DEBUG
        let mut offset:usize = 0;
        let mut short_ent =  ShortDirEntry::empty();
        loop {
            let mut read_sz = self.read_short_dirent(|curr_ent:&ShortDirEntry|{
                curr_ent.read_at(
                    offset, 
                    short_ent.as_bytes_mut(),
                    &self.fs,
                    &self.fs.read().get_fat(),
                    &self.block_device
                )
            });
            // 检测是否结束或被删除
            if read_sz != DIRENT_SZ || short_ent.is_empty() { 
                return Some(list)
            }
            if short_ent.is_deleted() { offset += DIRENT_SZ; continue; }
            if short_ent.is_long() {  // 长文件名
                let (_, long_ent_list, _) = unsafe { 
                    short_ent.as_bytes_mut().align_to_mut::<LongDirEntry>()
                };
                // DEBUG
                let mut long_ent = long_ent_list[0];
                let mut order = long_ent.get_order(); //^ 0x40;
                if order & 0x40 == 0 {
                    offset += DIRENT_SZ; continue;
                } else {
                    order = order^0x40;
                }
                let mut name = long_ent.get_name_raw();
                #[allow(unused)]
                for i in 1..order as usize{
                    offset += DIRENT_SZ;
                    read_sz =  self.read_short_dirent(|curr_ent:&ShortDirEntry|{
                        curr_ent.read_at(
                            offset, 
                            long_ent.as_bytes_mut(),
                            &self.fs,
                            &self.fs.read().get_fat(),
                            &self.block_device
                        )
                    });
                    if read_sz != DIRENT_SZ || long_ent.is_empty() || long_ent.is_deleted() { 
                        return Some(list)
                    }
                    // 若无误，把该段名字放在name最前
                    name.insert_str(0, long_ent.get_name_raw().as_str());
                }
                // 从短文件获取类型
                offset += DIRENT_SZ;
                read_sz =  self.read_short_dirent(|curr_ent:&ShortDirEntry|{
                    curr_ent.read_at(
                        offset, 
                        long_ent.as_bytes_mut(),
                        &self.fs,
                        &self.fs.read().get_fat(),
                        &self.block_device
                    )
                });
                if read_sz != DIRENT_SZ || long_ent.is_empty() || long_ent.is_deleted() { 
                    return Some(list)
                }
                list.push( (name, long_ent.attribute()) );
                offset += DIRENT_SZ;
                continue;
            } else { // 短文件名
                list.push((short_ent.get_name_lowercase(), short_ent.attribute()));  
                offset += DIRENT_SZ;
                continue; 
            }
        }
    }

    /* 获取目录中offset处目录项的信息 TODO:之后考虑和stat复用
    * 返回<name, offset, firstcluster,attributes>
    */
    pub fn dirent_info(&self, off:usize) -> Option<(String, u32, u32, u8)>{
        if !self.is_dir() {
            return None
        } 
        let mut long_ent = LongDirEntry::empty();
        let mut offset = off;
        let mut name = String::new(); 
        let mut is_long = false;
        //let mut order:u8 = 0;
        loop {
            let read_sz = self.read_short_dirent(|curr_ent:&ShortDirEntry|{
                curr_ent.read_at(
                    offset, 
                    long_ent.as_bytes_mut(),
                    &self.fs,
                    &self.fs.read().get_fat(),
                    &self.block_device
                )
            });
            if read_sz != DIRENT_SZ || long_ent.is_empty() { 
                return None;
            }
            if long_ent.is_deleted() { //if meet delete ent, search should be restart
                offset += DIRENT_SZ; 
                name.clear();
                is_long = false; continue; 
            }
            // 名称拼接
            if long_ent.attribute() != ATTRIBUTE_LFN {
                let (_, se_array, _) = unsafe{
                    long_ent.as_bytes_mut().align_to_mut::<ShortDirEntry>()
                };
                let short_ent = se_array[0];
                if !is_long {
                    name = short_ent.get_name_lowercase();
                } 
                //println!("---{}", short_ent.get_name_lowercase());
                let attribute = short_ent.attribute();
                let first_cluster = short_ent.first_cluster();
                offset += DIRENT_SZ;
                return Some((name, offset as u32, first_cluster,attribute))
            } else {
                is_long = true;
                //order += 1;
                name.insert_str(0, long_ent.get_name_format().as_str());
                //println!("--{}", long_ent.get_name_format().as_str());
            }
            offset += DIRENT_SZ; 
        }
    }

    /* 获取目录中offset处目录项的信息 TODO:之后考虑和stat复用
    * 返回<size, atime, mtime, ctime>
    */
    pub fn stat(&self)->( i64, i64, i64, i64, u64 ){
        self.read_short_dirent(|sde:&ShortDirEntry|{
            let (_,_,_,_,_,_,ctime) = sde.get_creation_time();
            let (_,_,_,_,_,_,atime) = sde.get_accessed_time();
            let (_,_,_,_,_,_,mtime) = sde.get_modification_time();
            let mut size = sde.get_size();
            let first_clu = sde.first_cluster();
            if self.is_dir() {
                let fs_reader = self.fs.read();
                let fat = fs_reader.get_fat();
                let fat_reader = fat.read();
                let cluster_num = fat_reader.count_claster_num( first_clu, self.block_device.clone());
                size = cluster_num * fs_reader.bytes_per_cluster();
                //println!("{} {}",cluster_num, fs_reader.bytes_per_cluster());
            }
            (size as i64, atime as i64, mtime as i64, ctime as i64, first_clu as u64)
        })
    }
    
    /* ls精简版，上面那个又臭又长，但这个不保证可靠 */
    // DEBUG
    pub fn ls_lite(&self)-> Option<Vec<(String, u8)>>{   
        if !self.is_dir() {
            return None
        } 
        let mut list:Vec<(String, u8)> = Vec::new();
        let mut long_ent = LongDirEntry::empty();
        let mut offset = 0;
        let mut name = String::new(); 
        let mut is_long = false;
        loop {
            let read_sz = self.read_short_dirent(|curr_ent:&ShortDirEntry|{
                curr_ent.read_at(
                    offset, 
                    long_ent.as_bytes_mut(),
                    &self.fs,
                    &self.fs.read().get_fat(),
                    &self.block_device
                )
            });
            if read_sz != DIRENT_SZ || long_ent.is_empty() { 
                return Some(list)
            }
            if long_ent.is_deleted() { offset += DIRENT_SZ; is_long = false; continue; }
            // 名称拼接
            if long_ent.attribute() != ATTRIBUTE_LFN { // 短文件名
                let (_, se_array, _) = unsafe{
                    long_ent.as_bytes_mut().align_to_mut::<ShortDirEntry>()
                };
                let short_ent = se_array[0];
                if is_long {
                    is_long = false;
                    list.push( (name.clone(), short_ent.attribute()) );
                } else {
                    list.push( (short_ent.get_name_lowercase(), short_ent.attribute()) )
                }
                name.clear();
            } else { // 长文件名，开始拼接
                is_long = true;
                name.insert_str(0, long_ent.get_name_format().as_str());
            }
            offset += DIRENT_SZ; 
        }
    }


    pub fn read_at(&self, offset: usize, buf: &mut [u8])->usize{
        self.read_short_dirent(|short_ent: &ShortDirEntry|{
            short_ent.read_at(
                offset, 
                buf, 
                &self.fs,
                &self.fs.read().get_fat(), 
                &self.block_device
            )
        })
    }   

    pub fn write_at(& self, offset: usize, buf: & [u8])->usize{
        self.increase_size((offset + buf.len()) as u32  );
        self.modify_short_dirent(|short_ent: &mut ShortDirEntry|{
            short_ent.write_at(
                offset, 
                buf, 
                &self.fs, 
                &self.fs.read().get_fat(), 
                &self.block_device
            )
        })
    }

    pub fn clear(&self){
        // 难点:长名目录项也要修改
        let first_cluster:u32 = self.first_cluster();
        if self.is_dir() || first_cluster == 0 {
            return;
        }
        for i in 0..self.long_pos_vec.len() {
            self.modify_long_dirent(i , |long_ent: &mut LongDirEntry|{
                long_ent.clear();
            });
        }
        self.modify_short_dirent(|short_ent:&mut ShortDirEntry|{
            short_ent.clear();
        });
        let all_clusters = self.fs.read()
            .get_fat().read()
            .get_all_cluster_of(first_cluster, self.block_device.clone());
        //self.fs.write().dealloc_cluster(all_clusters);
        let fs_reader = self.fs.read();
        fs_reader.dealloc_cluster(all_clusters);
        //fs_reader.cache_write_back();
    }

    /* 查找可用目录项，返回offset，簇不够也会返回相应的offset，caller需要及时分配 */
    fn find_free_dirent(&self)->Option<usize>{
        if !self.is_dir() {
            return None
        }
        let mut offset = 0;
        loop {
            if (offset/DIRENT_SZ)%5 == 0 {
                // print!("\n");
            }
            let mut tmp_dirent = ShortDirEntry::empty();
            let read_sz = self.read_short_dirent(|short_ent:&ShortDirEntry|{
                short_ent.read_at(
                    offset, 
                    tmp_dirent.as_bytes_mut(), 
                    &self.fs, 
                    &self.fs.read().get_fat(), 
                    &self.block_device
                )
            });
            if tmp_dirent.is_empty() || read_sz == 0{
                return Some(offset)
            }
            offset += DIRENT_SZ;
        }
    }

    pub fn creation_time(&self) -> (u32,u32,u32,u32,u32,u32,u64){
        self.read_short_dirent(|sde:&ShortDirEntry|{
            sde.get_creation_time()
        })
    }
    
    pub fn accessed_time(&self) -> (u32,u32,u32,u32,u32,u32,u64){
        self.read_short_dirent(|sde:&ShortDirEntry|{
            sde.get_accessed_time()
        })
    }
    
    pub fn modification_time(&self) -> (u32,u32,u32,u32,u32,u32,u64){
        self.read_short_dirent(|sde:&ShortDirEntry|{
            sde.get_modification_time()
        })
    }

    pub fn set_delete_bit(&self){
        self.modify_short_dirent(|se:&mut ShortDirEntry|{
            se.delete();
        })
    }

    /* WAITING 目前只支持删除自己*/
    pub fn remove(&self)->usize{
        //self.modify_short_dirent(|sdent: &mut ShortDirEntry|{
        //    sdent.delete();
        //});
        let first_cluster:u32 = self.first_cluster();
        for i in 0..self.long_pos_vec.len() {
            self.modify_long_dirent(i , |long_ent: &mut LongDirEntry|{
                long_ent.delete();
            });
        }
        //println!("[fs]: rm file");
        self.modify_short_dirent(|short_ent:&mut ShortDirEntry|{
            short_ent.delete();
        });
        let all_clusters = self.fs.read()
            .get_fat().read()
            .get_all_cluster_of(first_cluster, self.block_device.clone());
        self.fs.write().dealloc_cluster(all_clusters.clone());
        //self.fs.write().cache_write_back();
        return all_clusters.len()
    }

    /* WAITING */
    #[allow(unused)]
    fn remove_file(){

    }

    /* WAITING */
    #[allow(unused)]
    fn remove_dir(){

    }
}

/* WAITING */
#[allow(unused)]
pub fn fcopy(){

}

/* WAITING */
#[allow(unused)]
pub fn fmove(){
    
}