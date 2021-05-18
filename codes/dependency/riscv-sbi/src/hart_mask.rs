use core::mem::size_of;
extern crate alloc;

use alloc::{vec, vec::Vec};

#[derive(Debug, Clone)]
/// Hart mask for selecting harts in many extensions
pub struct HartMask {
    inner: Vec<usize>,
    max_hart_id: usize,
}

impl HartMask {
    /// Create a new hart mask with no harts selected
    pub fn none(max_hart_id: usize) -> HartMask {
        let (i, _) = split_index_usize(max_hart_id);
        let len = i + 1;
        HartMask {
            inner: vec![0usize; len],
            max_hart_id,
        }
    }

    /// Create a new hart mask with all harts selected
    pub fn all(max_hart_id: usize) -> HartMask {
        let (i, _) = split_index_usize(max_hart_id);
        let len = i + 1;
        HartMask {
            inner: vec![usize::max_value(); len],
            max_hart_id,
        }
    }

    /// Set one hart with id `hart_id`
    pub fn set(&mut self, hart_id: usize) {
        assert!(hart_id <= self.max_hart_id);
        let (i, j) = split_index_usize(hart_id);
        self.inner[i] |= 1 << j;
    }

    /// Unset one hart with id `hart_id`
    pub fn clear(&mut self, hart_id: usize) {
        assert!(hart_id <= self.max_hart_id);
        let (i, j) = split_index_usize(hart_id);
        self.inner[i] &= !(1 << j);
    }

    pub(crate) fn as_ptr(&self) -> *const usize {
        self.inner.as_ptr()
    }
}

#[inline]
fn split_index_usize(index: usize) -> (usize, usize) {
    let bits_in_usize = size_of::<usize>() * 8;
    (index / bits_in_usize, index % bits_in_usize)
}
