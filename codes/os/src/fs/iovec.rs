use alloc::vec::Vec;
use alloc::vec;
use crate::mm::{
    translated_byte_buffer,
    //translated_ref_array,
    translated_array_copy,
};


#[derive(Debug, Copy, Clone)]
#[repr(C)]
pub struct IoVec {
    /// Starting address
    base: *mut u8,
    /// Number of bytes to transfer
    len: usize,
}

pub struct IoVecs(pub Vec<&'static mut [u8]>);

impl IoVecs {
    pub unsafe fn new(
        iov_ptr: *mut IoVec,
        iov_num: usize,
        token: usize,
    )-> Self {
        let mut iovecs: Vec<&'static mut [u8]> = vec![];
        let iovref_vec = translated_array_copy(token, iov_ptr, iov_num);
        iovecs.reserve(iovref_vec.len());
        for iovref in iovref_vec {
            if iovref.len == 0 {
                continue;
            }
            //println!("iov.base = 0x{:X}, iov.len = {}", iovref.base as usize,iovref.len);
            let mut buf:Vec<&'static mut [u8]> = translated_byte_buffer(token, iovref.base, iovref.len);
            iovecs.append(&mut buf);
        }
        Self(iovecs)
    }

    //pub unsafe fn check_and_new(
    //    iov_ptr: *const IoVec,
    //    iov_count: usize,
    //    vm: &MemorySet,
    //    readv: bool,
    //) -> Result<Self, SysError> {
    //    let iovs = vm.check_read_array(iov_ptr, iov_count)?.to_vec();
    //    let mut slices = vec![];
    //    slices.reserve(iovs.len());
    //    // check all bufs in iov
    //    for iov in iovs.iter() {
    //        // skip empty iov
    //        if iov.len == 0 {
    //            continue;
    //        }
    //        if readv {
    //            vm.check_write_array(iov.base, iov.len)?;
    //        } else {
    //            vm.check_read_array(iov.base, iov.len)?;
    //        }
    //        slices.push(slice::from_raw_parts_mut(iov.base, iov.len));
    //    }
    //    Ok(IoVecs(slices))
    //}
//
    //pub fn read_all_to_vec(&self) -> Vec<u8> {
    //    let mut buf = self.new_buf(false);
    //    for slice in self.0.iter() {
    //        buf.extend(slice.iter());
    //    }
    //    buf
    //}
//
    //pub fn write_all_from_slice(&mut self, buf: &[u8]) {
    //    let mut copied_len = 0;
    //    for slice in self.0.iter_mut() {
    //        let copy_len = min(slice.len(), buf.len() - copied_len);
    //        if copy_len == 0 {
    //            continue;
    //        }
//
    //        slice[..copy_len].copy_from_slice(&buf[copied_len..copied_len + copy_len]);
    //        copied_len += copy_len;
    //    }
    //}
//
    ///// Create a new Vec buffer from IoVecs
    ///// For readv:  `set_len` is true,  Vec.len = total_len.
    ///// For writev: `set_len` is false, Vec.cap = total_len.
    //pub fn new_buf(&self, set_len: bool) -> Vec<u8> {
    //    let total_len = self.0.iter().map(|slice| slice.len()).sum::<usize>();
    //    let mut buf = Vec::with_capacity(total_len);
    //    if set_len {
    //        unsafe {
    //            buf.set_len(total_len);
    //        }
    //    }
    //    buf
    //}
}