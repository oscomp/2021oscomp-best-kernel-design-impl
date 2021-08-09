use crate::fs::File;
use crate::mm::UserBuffer;

pub struct NullZero {
    is_null:bool
}

impl NullZero {
    pub fn new(is_null:bool)->Self{
        Self{
            is_null
        }
    }
}

impl File for NullZero {
    fn readable(&self) -> bool {
        true
    }

    fn writable(&self) -> bool {    
        true
    }

    fn read(&self, mut buf: UserBuffer) -> usize {
        if self.is_null{
            return 0
        } else {
            // fill with zero
            buf.clear();
            return buf.len()
        }
    }

    fn write(&self, buf: UserBuffer) -> usize {
        return buf.len()
    }
}