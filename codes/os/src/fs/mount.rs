//use core::f32::MANTISSA_DIGITS;
use alloc::vec::Vec;
use alloc::string::String;
use lazy_static::*;
use spin::Mutex;
use alloc::sync::Arc;

pub struct MountTable {
    mnt_list:Vec<(String, String, String)>//special,dir,fstype
}

impl MountTable {

    pub fn mount(&mut self, special:String,dir:String,fstype:String,)->isize{
        self.mnt_list.push((special, dir, fstype));
        return 0
    }

    pub fn umount(&mut self, special:String, flags:u32)->isize{
        let len = self.mnt_list.len();
        for i in 0..len {
            if self.mnt_list[i].0 == special {
                self.mnt_list.remove(i);
                return 0
            }
        }
        return -1
    }
}

lazy_static! {
    pub static ref MNT_TABLE: Arc<Mutex<MountTable>> = {
        let mnt_table = MountTable {
            mnt_list: Vec::new(),
        };
        Arc::new(Mutex::new( mnt_table ))
    };
}