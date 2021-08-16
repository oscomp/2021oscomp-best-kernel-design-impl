mod heap_allocator;
mod address;
mod frame_allocator;
mod page_table;
mod memory_set;
mod vma;

use page_table::PTEFlags;
use address::VPNRange;
pub use address::{PhysAddr, VirtAddr, PhysPageNum, VirtPageNum, StepByOne};
pub use frame_allocator::{FrameTracker, frame_alloc, frame_dealloc,add_free, print_free_pages, frame_add_ref, enquire_refcount};
pub use page_table::{
    PageTable,
    PageTableEntry,
    translated_byte_buffer,
    translated_str,
    translated_ref,
    translated_refmut,
    translated_ref_array,
    translated_array_copy,
    copy_from_user,
    copy_to_user,
    UserBuffer,
    UserBufferIterator,
};
pub use vma::{MmapArea, MmapSpace};
pub use memory_set::{MemorySet, KERNEL_SPACE, KERNEL_MMAP_AREA, KERNEL_TOKEN,  MapPermission, kernel_token};
pub use memory_set::remap_test;
pub use heap_allocator::HEAP_ALLOCATOR;

pub fn init() {
    heap_allocator::init_heap();
    frame_allocator::init_frame_allocator();
    KERNEL_SPACE.lock().activate();
    // KERNEL_SPACE.lock().print_pagetable();
}

pub fn init_othercore(){
    KERNEL_SPACE.lock().activate();
}

