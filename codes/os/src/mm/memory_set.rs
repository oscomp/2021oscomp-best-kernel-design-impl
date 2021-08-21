use super::{PageTable, PageTableEntry, PTEFlags};
use super::{VirtPageNum, VirtAddr, PhysPageNum, PhysAddr};          
use super::{FrameTracker, frame_alloc, frame_add_ref, enquire_refcount, print_free_pages};
use super::{VPNRange, StepByOne};
use alloc::collections::BTreeMap;
//use alloc::string::ToString;
use alloc::vec::Vec;
use riscv::register::satp;
use alloc::sync::Arc;
use lazy_static::*;
use spin::Mutex;
use crate::config::*;
use crate::mm::MmapArea;
use crate::monitor::*;
use crate::task::AuxHeader;

extern "C" {
    fn stext();
    fn etext();
    fn srodata();
    fn erodata();
    fn sdata();
    fn edata();
    fn sbss_with_stack();
    fn ebss();
    fn ekernel();
    fn strampoline();
    fn ssignaltrampoline();
}


pub struct KernelToken {
    token:usize
}
impl KernelToken {
    pub fn token(&self)->usize{
        self.token
    }
}

lazy_static! {
    pub static ref KERNEL_SPACE: Arc<Mutex<MemorySet>> = Arc::new(Mutex::new(
        MemorySet::new_kernel()
    ));

    pub static ref KERNEL_TOKEN: Arc<KernelToken> = Arc::new(
        KernelToken{
            token:KERNEL_SPACE.lock().token()
        }
    );
}

lazy_static! {
    pub static ref KERNEL_MMAP_AREA: Arc<Mutex<MmapArea>> = Arc::new(Mutex::new(
        MmapArea::new(VirtAddr::from(KMMAP_BASE), VirtAddr::from(KMMAP_BASE))
    ));
}



pub fn kernel_token() -> usize {
    KERNEL_SPACE.lock().token()
}

pub struct MemorySet {
    page_table: PageTable,
    areas: Vec<MapArea>,
    chunks: ChunkArea,
    stack_chunks: ChunkArea,
    mmap_chunks: Vec<ChunkArea>,
}

impl MemorySet {
    pub fn clone_areas(&self) -> Vec<MapArea> {
        self.areas.clone()
    }
    pub fn new_bare() -> Self {
        Self {
            page_table: PageTable::new(),
            areas: Vec::new(),
            chunks: ChunkArea::new(MapType::Framed,
                                MapPermission::R | MapPermission::W | MapPermission::U),
            mmap_chunks: Vec::new(),
            stack_chunks: ChunkArea::new(MapType::Framed,
                                MapPermission::R | MapPermission::W | MapPermission::U),
        }
    }
    pub fn set_cow(&mut self, vpn: VirtPageNum) {
        self.page_table.set_cow(vpn);
    }
    pub fn reset_cow(&mut self, vpn: VirtPageNum) {
        self.page_table.reset_cow(vpn);
    }
    pub fn set_flags(&mut self, vpn: VirtPageNum, flags: PTEFlags) {
        self.page_table.set_flags(vpn, flags);
    }
    pub fn token(&self) -> usize {
        self.page_table.token()
    }
    /// Assume that no conflicts.
    pub fn insert_framed_area(&mut self, start_va: VirtAddr, end_va: VirtAddr, permission: MapPermission) {
        self.push(MapArea::new(
            start_va,
            end_va,
            MapType::Framed,
            permission,
        ), None);
    }
    pub fn insert_kernel_mmap_area(&mut self, start_va: VirtAddr, end_va: VirtAddr, permission: MapPermission) {
        // println!{"insert kernel mmap_area: {:X} {:X}", start_va.0, end_va.0}
        self.push_mmap(MapArea::new(
            start_va,
            end_va,
            MapType::Framed,
            permission,
        ), None);
    }
    pub fn insert_mmap_area(&mut self, start_va: VirtAddr, end_va: VirtAddr, permission: MapPermission) {
        let mut new_chunk_area = ChunkArea::new(MapType::Framed, permission,);
        new_chunk_area.set_mmap_range(start_va, end_va);
        self.mmap_chunks.push(new_chunk_area);

        //self.push_mmap(MapArea::new(
        //    start_va,
        //    (end_va.0).into(),  // bin lazy (X
        //    MapType::Framed,
        //    permission,
        //), None);
    }
    fn push_mmap(&mut self, mut map_area: MapArea, data: Option<&[u8]>) {
        // println!{"1"}
        map_area.map(&mut self.page_table);
        self.areas.push(map_area);
    }
    pub fn remove_area_with_start_vpn(&mut self, start_vpn: VirtPageNum) {
        if let Some((idx, area)) = self.areas.iter_mut().enumerate()
            .find(|(_, area)| area.vpn_range.get_start() == start_vpn) {
            area.unmap(&mut self.page_table);
            self.areas.remove(idx);
        }

        let chunks_len = self.mmap_chunks.len();
        for i in 0..chunks_len {
            let chunk = &self.mmap_chunks[i];
            if (chunk.mmap_start.0 >> 12) == start_vpn.0 {
                //println!("remove mmap_chunk 0x{:X}", chunk.mmap_start.0);
                self.mmap_chunks.remove(i);
                break;
            }
        }
    }
    fn remap_cow(&mut self, vpn: VirtPageNum, ppn: PhysPageNum, former_ppn: PhysPageNum) {
        self.page_table.remap_cow(vpn, ppn, former_ppn);
    }
    fn push(&mut self, mut map_area: MapArea, data: Option<&[u8]>) {
        // println!{"2"}
        map_area.map(&mut self.page_table);
        if let Some(data) = data {
            map_area.copy_data(&mut self.page_table, data, 0);
        }
        self.areas.push(map_area);
    }
    fn push_mapped(&mut self, mut map_area: MapArea) {
        self.areas.push(map_area);
    }
    fn push_chunk(&mut self, vpn: VirtPageNum) {
        // self.chunks.vpn_table.push(vpn);
        // self.chunks.map_one(&mut self.page_table, vpn);
        self.chunks.push_vpn(vpn, &mut self.page_table)
    }
    fn push_stack_chunk(&mut self, vpn: VirtPageNum) {
        self.stack_chunks.push_vpn(vpn, &mut self.page_table)
    }
    fn push_with_offset(&mut self, mut map_area: MapArea, offset: usize, data: Option<&[u8]>){
        // println!{"3"}
        map_area.map(&mut self.page_table);
        if let Some(data) = data {
            map_area.copy_data(&mut self.page_table, data, offset);
        }
        self.areas.push(map_area);
    }

    /// Mention that trampoline is not collected by areas.
    fn map_trampoline(&mut self) {
        self.page_table.map(
            VirtAddr::from(TRAMPOLINE).into(),
            PhysAddr::from(strampoline as usize).into(),
            PTEFlags::R | PTEFlags::X,
        );
    }

    /// Mention that trampoline is not collected by areas.
    /// Different from trampoline: this dosen't need to be mapped in kernel(executed in user)
    fn map_signal_trampoline(&mut self) {
        self.page_table.map(
            VirtAddr::from(SIGNAL_TRAMPOLINE).into(),
            PhysAddr::from(ssignaltrampoline as usize).into(),
            PTEFlags::R | PTEFlags::X | PTEFlags::U,
        );
    }

    fn map_kernel_shared(&mut self){
        self.page_table.map_kernel_shared();
    }

    /// Without kernel stacks.
    pub fn new_kernel() -> Self {
        let mut memory_set = Self::new_bare();
        // map trampoline
        memory_set.map_trampoline();
        // map kernel sections
        println!(".text [{:#x}, {:#x})", stext as usize, etext as usize);
        println!(".rodata [{:#x}, {:#x})", srodata as usize, erodata as usize);
        println!(".data [{:#x}, {:#x})", sdata as usize, edata as usize);
        println!(".bss [{:#x}, {:#x})", sbss_with_stack as usize, ebss as usize);
        println!("mapping .text section");
        memory_set.push(MapArea::new(
            (stext as usize).into(),
            (etext as usize).into(),
            MapType::Identical,
            MapPermission::R | MapPermission::X,
        ), None);
        println!("mapping .rodata section");
        memory_set.push(MapArea::new(
            (srodata as usize).into(),
            (erodata as usize).into(),
            MapType::Identical,
            MapPermission::R,
        ), None);
        println!("mapping .data section");
        memory_set.push(MapArea::new(
            (sdata as usize).into(),
            (edata as usize).into(),
            MapType::Identical,
            MapPermission::R | MapPermission::W,
        ), None);
        println!("mapping .bss section");
        memory_set.push(MapArea::new(
            (sbss_with_stack as usize).into(),
            (ebss as usize).into(),
            MapType::Identical,
            MapPermission::R | MapPermission::W,
        ), None);
        println!("mapping physical memory");
        memory_set.push(MapArea::new(
            (ekernel as usize).into(),
            MEMORY_END.into(),
            MapType::Identical,
            MapPermission::R | MapPermission::W,
        ), None);
        println!("mapping memory-mapped registers");
        for pair in MMIO {
            memory_set.push(MapArea::new(
                (*pair).0.into(),
                ((*pair).0 + (*pair).1).into(),
                MapType::Identical,
                MapPermission::R | MapPermission::W,
            ), None);
        }
        memory_set
    }
    /// Include sections in elf and trampoline and TrapContext and user stack,
    /// also returns user_sp and entry point.
    pub fn from_elf(elf_data: &[u8]) -> (Self, usize, usize, usize, Vec<AuxHeader>) {
        let mut auxv:Vec<AuxHeader> = Vec::new();
        let mut memory_set = Self::new_bare();
        // map trampoline
        // memory_set.map_trampoline();
        memory_set.map_signal_trampoline();
        // map program headers of elf, with U flag
        let elf = xmas_elf::ElfFile::new(elf_data).unwrap();
        let elf_header = elf.header;
        // let comment_sec = elf.find_section_by_name(".comment").unwrap();
        // println!(".comment offset: {}", comment_sec.offset());
        

        let magic = elf_header.pt1.magic;
        assert_eq!(magic, [0x7f, 0x45, 0x4c, 0x46], "invalid elf!");
        let ph_count = elf_header.pt2.ph_count();
        let mut max_end_vpn = VirtPageNum(0);
        let mut head_va = 0; // top va of ELF which points to ELF header
        // push ELF related auxv
        auxv.push(AuxHeader{aux_type: AT_PHENT, value: elf.header.pt2.ph_entry_size() as usize});// ELF64 header 64bytes
        auxv.push(AuxHeader{aux_type: AT_PHNUM, value: ph_count as usize});
        auxv.push(AuxHeader{aux_type: AT_PAGESZ, value: PAGE_SIZE as usize});
        auxv.push(AuxHeader{aux_type: AT_BASE, value: 0 as usize});
        auxv.push(AuxHeader{aux_type: AT_FLAGS, value: 0 as usize});
        auxv.push(AuxHeader{aux_type: AT_ENTRY, value: elf.header.pt2.entry_point() as usize});
        auxv.push(AuxHeader{aux_type: AT_UID, value: 0 as usize});
        auxv.push(AuxHeader{aux_type: AT_EUID, value: 0 as usize});
        auxv.push(AuxHeader{aux_type: AT_GID, value: 0 as usize});
        auxv.push(AuxHeader{aux_type: AT_EGID, value: 0 as usize});
        auxv.push(AuxHeader{aux_type: AT_PLATFORM, value: 0 as usize});
        auxv.push(AuxHeader{aux_type: AT_HWCAP, value: 0 as usize});
        auxv.push(AuxHeader{aux_type: AT_CLKTCK, value: 100 as usize});
        auxv.push(AuxHeader{aux_type: AT_SECURE, value: 0 as usize});
        auxv.push(AuxHeader{aux_type: AT_NOTELF, value: 0x112d as usize});

        // denotes if .comment should be mapped
        let mut comment_flag = true;

        for ph in elf.program_iter(){
            if ph.get_type().unwrap() == xmas_elf::program::Type::Load {
                let start_va: VirtAddr = (ph.virtual_addr() as usize).into();
                let end_va: VirtAddr = ((ph.virtual_addr() + ph.mem_size()) as usize).into();
                let offset = start_va.0 - start_va.floor().0 * PAGE_SIZE;

                if start_va.0 == 0{
                    comment_flag = false;
                } 

                //println!("[elf] ph={:?}", ph.to_string());
                //println!("[elf] start_va = 0x{:X}; end_va = 0x{:X}, offset = 0x{:X}", ph.virtual_addr() as usize, ph.virtual_addr() + ph.mem_size(), offset);
                let mut map_perm = MapPermission::U;
                let ph_flags = ph.flags();
                if ph_flags.is_read() { map_perm |= MapPermission::R; }
                if ph_flags.is_write() { map_perm |= MapPermission::W; }
                if ph_flags.is_execute() { map_perm |= MapPermission::X; }
                let map_area = MapArea::new(
                    start_va,
                    end_va,
                    MapType::Framed,
                    map_perm,
                );
                //println!("[elf] map elfinput:\n    from 0x{:X} to 0x{:X}", ph.offset(), ph.offset() + ph.file_size());
                max_end_vpn = map_area.vpn_range.get_end();
                
                if offset == 0 {
                    head_va = start_va.into();
                    memory_set.push( 
                        map_area,
                        Some(&elf.input[ph.offset() as usize..(ph.offset() + ph.file_size()) as usize])
                    );
                } else {
                    memory_set.push_with_offset( 
                        map_area,
                        offset,
                        Some(&elf.input[ph.offset() as usize..(ph.offset() + ph.file_size()) as usize])
                    );
                }
            }
        }

        // Get ph_head addr for auxv
        let ph_head_addr = head_va + elf.header.pt2.ph_offset() as usize;
        auxv.push(AuxHeader{aux_type: AT_PHDR, value: ph_head_addr as usize});


        //map user heap
        let max_end_va: VirtAddr = max_end_vpn.into();
        let mut user_heap_bottom: usize = max_end_va.into();
        //guard page
        user_heap_bottom += PAGE_SIZE;
        // let user_heap_top: usize = user_heap_bottom + USER_HEAP_SIZE;
        //maparea1: user_heap
        // memory_set.push(MapArea::new(
        //     user_heap_bottom.into(),
        //     user_heap_top.into(),
        //     MapType::Framed,
        //     MapPermission::R | MapPermission::W | MapPermission::U,
        // ), None);

        // maparea2: TrapContext
        memory_set.push(MapArea::new(
            TRAP_CONTEXT.into(),
            (TRAP_CONTEXT+PAGE_SIZE).into(),
            MapType::Framed,
            MapPermission::R | MapPermission::W,
        ), None);

        // map user stack with U flags
        // maparea3: user_stack
        let max_top_va: VirtAddr = TRAP_CONTEXT.into();
        let mut user_stack_top: usize = TRAP_CONTEXT;
        user_stack_top -= PAGE_SIZE;
        let user_stack_bottom: usize = user_stack_top - USER_STACK_SIZE_MIN; 
        memory_set.push(MapArea::new(
            user_stack_bottom.into(),
            user_stack_top.into(),
            MapType::Framed,
            MapPermission::R | MapPermission::W | MapPermission::U,
        ), None);

        // map signal user stack with U flags
        // maparea4: signal_user_stack
        let mut signal_stack_top: usize = USER_SIGNAL_STACK;
        let signal_stack_bottom: usize = signal_stack_top - SIGNAL_STACK_SIZE;
        memory_set.push(MapArea::new(
            signal_stack_bottom.into(),
            signal_stack_top.into(),
            MapType::Framed,
            MapPermission::R | MapPermission::W | MapPermission::U,
        ), None);
        
        memory_set.map_kernel_shared();

        (memory_set, user_stack_top, user_heap_bottom, elf.header.pt2.entry_point() as usize, auxv)
    }
 
    pub fn from_existed_user(user_space: &MemorySet) -> MemorySet {
        let mut memory_set = Self::new_bare();
        // map trampoline
        // memory_set.map_trampoline();
        memory_set.map_signal_trampoline();
        // copy data sections/trap_context/user_stack
        for area in user_space.areas.iter() {
            let new_area = MapArea::from_another(area);
            memory_set.push(new_area, None);
            // copy data from another space
            for vpn in area.vpn_range {
                let src_ppn = user_space.translate(vpn).unwrap().ppn();
                let dst_ppn = memory_set.translate(vpn).unwrap().ppn();
                dst_ppn.get_bytes_array().copy_from_slice(src_ppn.get_bytes_array());
            }
        }
        for vpn in user_space.chunks.vpn_table.iter() {
            let vpn_copy: VirtPageNum = vpn.0.into();
            memory_set.push_chunk(vpn_copy);
            let src_ppn = user_space.translate(vpn_copy).unwrap().ppn();
            let dst_ppn = memory_set.translate(vpn_copy).unwrap().ppn();
            dst_ppn.get_bytes_array().copy_from_slice(src_ppn.get_bytes_array());
        }
        for vpn in user_space.stack_chunks.vpn_table.iter() {
            let vpn_copy: VirtPageNum = vpn.0.into();
            memory_set.push_chunk(vpn_copy);
            let src_ppn = user_space.translate(vpn_copy).unwrap().ppn();
            let dst_ppn = memory_set.translate(vpn_copy).unwrap().ppn();
            dst_ppn.get_bytes_array().copy_from_slice(src_ppn.get_bytes_array());
        }
        for mmap_chunk in user_space.mmap_chunks.iter() {
            let mut new_mmap_area = ChunkArea::new(mmap_chunk.map_type, mmap_chunk.map_perm);
            new_mmap_area.set_mmap_range(mmap_chunk.mmap_start, mmap_chunk.mmap_end);
            for vpn in mmap_chunk.vpn_table.iter() {
                let vpn_copy: VirtPageNum = vpn.0.into();
                new_mmap_area.push_vpn(vpn_copy, &mut memory_set.page_table);
                let src_ppn = user_space.translate(vpn_copy).unwrap().ppn();
                let dst_ppn = memory_set.translate(vpn_copy).unwrap().ppn();
                dst_ppn.get_bytes_array().copy_from_slice(src_ppn.get_bytes_array());
            }
            memory_set.mmap_chunks.push(new_mmap_area);
        }
        memory_set.map_kernel_shared();
        memory_set
    }

    pub fn from_copy_on_write(user_space: &mut MemorySet, split_addr: usize) -> MemorySet {
        // create a new memory_set
        let mut memory_set = Self::new_bare();
        // This part is not for Copy on Write.
        // Including:   Trampoline
        //              Trap_Context
        //              User_Stack
        // memory_set.map_trampoline();
        memory_set.map_signal_trampoline();
        for area in user_space.areas.iter() {
            let head_vpn = area.vpn_range.get_start();
            let user_split_addr: VirtAddr = split_addr.into();
            if head_vpn < user_split_addr.floor() {
                //skipping the part using CoW
                continue;
            }
            let new_area = MapArea::from_another(area);
            memory_set.push(new_area, None);
            for vpn in area.vpn_range {
                let src_ppn = user_space.translate(vpn).unwrap().ppn();
                let dst_ppn = memory_set.translate(vpn).unwrap().ppn();
                // println!{"mapping {:?} --- {:?}, src: {:?}", vpn, dst_ppn, src_ppn};
                dst_ppn.get_bytes_array().copy_from_slice(src_ppn.get_bytes_array());
            }
        }
        //This part is for copy on write
        let mut parent_areas = &user_space.areas;
        let page_table = &mut user_space.page_table;
        for area in parent_areas.iter() {
            let head_vpn = area.vpn_range.get_start();
            let user_split_addr: VirtAddr = split_addr.into();
            if head_vpn >= user_split_addr.floor() {
                //skipping the part using Coping to new ppn
                continue;
            }
            let mut new_area = MapArea::from_another(area);
            // map the former physical address
            for vpn in area.vpn_range {
                //change the map permission of both pagetable
                // get the former flags and ppn
                let pte = page_table.translate(vpn).unwrap();
                let pte_flags = pte.flags() & !PTEFlags::W;
                let src_ppn = pte.ppn();
                frame_add_ref(src_ppn);
                // change the flags of the src_pte
                page_table.set_flags(vpn, pte_flags);
                page_table.set_cow(vpn);
                // map the cow page table to src_ppn
                memory_set.page_table.map(vpn, src_ppn, pte_flags);
                memory_set.set_cow(vpn);
                new_area.insert_tracker(vpn, src_ppn);
            }
            memory_set.push_mapped(new_area);
        }
        for vpn in user_space.chunks.vpn_table.iter() {
            let vpn_copy: VirtPageNum = vpn.0.into();
            memory_set.push_chunk(vpn_copy);
            let src_ppn = user_space.translate(vpn_copy).unwrap().ppn();
            let dst_ppn = memory_set.translate(vpn_copy).unwrap().ppn();
            dst_ppn.get_bytes_array().copy_from_slice(src_ppn.get_bytes_array());
        }
        for vpn in user_space.stack_chunks.vpn_table.iter() {
            let vpn_copy: VirtPageNum = vpn.0.into();
            memory_set.push_chunk(vpn_copy);
            let src_ppn = user_space.translate(vpn_copy).unwrap().ppn();
            let dst_ppn = memory_set.translate(vpn_copy).unwrap().ppn();
            dst_ppn.get_bytes_array().copy_from_slice(src_ppn.get_bytes_array());
        }
        for mmap_chunk in user_space.mmap_chunks.iter() {
            let mut new_mmap_area = ChunkArea::new(mmap_chunk.map_type, mmap_chunk.map_perm);
            new_mmap_area.set_mmap_range(mmap_chunk.mmap_start, mmap_chunk.mmap_end);
            for vpn in mmap_chunk.vpn_table.iter() {
                let vpn_copy: VirtPageNum = vpn.0.into();
                // memory_set.chunks.vpn_table.push(vpn_copy);
                // memory_set.chunks.map_one(&mut memory_set.page_table, vpn_copy);
                new_mmap_area.push_vpn(vpn_copy, &mut memory_set.page_table);
                let src_ppn = user_space.translate(vpn_copy).unwrap().ppn();
                let dst_ppn = memory_set.translate(vpn_copy).unwrap().ppn();
                dst_ppn.get_bytes_array().copy_from_slice(src_ppn.get_bytes_array());
            }
            memory_set.mmap_chunks.push(new_mmap_area);
        }
        memory_set.map_kernel_shared();
        memory_set
    }

    #[no_mangle]
    pub fn cow_alloc(&mut self, vpn: VirtPageNum, former_ppn: PhysPageNum) -> usize {
        if enquire_refcount(former_ppn) == 1 {
            self.page_table.reset_cow(vpn);
            // change the flags of the src_pte
            self.page_table.set_flags(
                vpn, 
                self.page_table.translate(vpn).unwrap().flags() | PTEFlags::W
            );
            return 0
        }
        let frame = frame_alloc().unwrap();
        let ppn = frame.ppn;
        self.remap_cow(vpn, ppn, former_ppn);
        for area in self.areas.iter_mut() {
            let head_vpn = area.vpn_range.get_start();
            let tail_vpn = area.vpn_range.get_end();
            if vpn <= tail_vpn && vpn >= head_vpn {
                area.data_frames.insert(vpn, frame);
                break;
            }
        }
        0
    }

    pub fn lazy_alloc_heap (&mut self, vpn: VirtPageNum) -> usize {
        self.push_chunk(vpn);
        0
    }

    pub fn lazy_alloc_stack (&mut self, vpn: VirtPageNum) -> usize {
        self.push_stack_chunk(vpn);
        0
    }

    pub fn lazy_mmap (&mut self, stval: VirtAddr) -> isize {
        for mmap_chunk in self.mmap_chunks.iter() {
            if stval >= mmap_chunk.mmap_start && stval < mmap_chunk.mmap_end {
                // read only can also be mapped!!??
                //if (mmap_chunk.map_perm & MapPermission::W).bits == 0 {
                //    return -1;
                //}
                self.push_chunk(stval.floor());
                return 0
            }
        }
        0
    }

    pub fn activate(&self) {
        let satp = self.page_table.token();
        unsafe {
            satp::write(satp);
            llvm_asm!("sfence.vma" :::: "volatile");
        }
    }
    pub fn translate(&self, vpn: VirtPageNum) -> Option<PageTableEntry> {
        self.page_table.translate(vpn)
    }

    // WARNING: This function causes inconsistency between pte flags and 
    //          map_area flags.
    // return -1 if not found, 0 if found
    pub fn set_pte_flags(&mut self, vpn: VirtPageNum, flags: usize) -> isize{
        self.page_table.set_pte_flags(vpn, flags)
    }
    
    pub fn recycle_data_pages(&mut self) {
        self.areas.clear();
    }

    pub fn print_pagetable(&mut self){
        self.page_table.print_pagetable();
    }
}

#[derive(Clone)]
pub struct MapArea {
    vpn_range: VPNRange,
    data_frames: BTreeMap<VirtPageNum, FrameTracker>,
    map_type: MapType,
    map_perm: MapPermission,
}

pub struct ChunkArea {
    vpn_table: Vec<VirtPageNum>,
    data_frames: BTreeMap<VirtPageNum, FrameTracker>,
    map_type: MapType,
    map_perm: MapPermission,
    mmap_start: VirtAddr,
    mmap_end: VirtAddr,
}

impl ChunkArea {
    pub fn new(
        map_type: MapType,
        map_perm: MapPermission
    ) -> Self {
        Self {
            vpn_table: Vec::new(),
            data_frames: BTreeMap::new(),
            map_type,
            map_perm,
            mmap_start: 0.into(),
            mmap_end: 0.into(),
        }
    }

    pub fn set_mmap_range(&mut self, start: VirtAddr, end: VirtAddr) {
        self.mmap_start = start;
        self.mmap_end = end;
    }

    pub fn push_vpn(&mut self, vpn: VirtPageNum, page_table: &mut PageTable) {
        self.vpn_table.push(vpn);
        self.map_one(page_table, vpn);
    }

    pub fn from_another(another: &ChunkArea) -> Self {
        Self {
            vpn_table: another.vpn_table.clone(),
            data_frames: BTreeMap::new(),
            map_type: another.map_type,
            map_perm: another.map_perm,
            mmap_start: another.mmap_start,
            mmap_end: another.mmap_end,
        }
    }

    // Alloc and map one page
    pub fn map_one(&mut self, page_table: &mut PageTable, vpn: VirtPageNum) {
        let ppn: PhysPageNum;
        match self.map_type {
            MapType::Identical => {
                ppn = PhysPageNum(vpn.0);
            }
            MapType::Framed => {
                if let Some(frame) = frame_alloc(){
                    ppn = frame.ppn;
                    self.data_frames.insert(vpn, frame);
                }
                else{
                    panic!("No more memory!");
                }
            }
        }
        let pte_flags = PTEFlags::from_bits(self.map_perm.bits).unwrap();
        // [WARNING]:因为没有map，所以不能使用
        page_table.map(vpn, ppn, pte_flags);
    }
    pub fn unmap_one(&mut self, page_table: &mut PageTable, vpn: VirtPageNum) {
        match self.map_type {
            MapType::Framed => {
                self.data_frames.remove(&vpn);
            }
            _ => {}
        }
        page_table.unmap(vpn);
    }
    
    // Alloc and map all pages
    // pub fn map(&mut self, page_table: &mut PageTable) {
    //     for vpn in self.vpn_table {
    //         self.map_one(page_table, vpn);
    //     }
    // }
    // pub fn unmap(&mut self, page_table: &mut PageTable) {
    //     for vpn in self.vpn_table {
    //         self.unmap_one(page_table, vpn);
    //     }
    // }

}

impl MapArea {
    pub fn new(
        start_va: VirtAddr,
        end_va: VirtAddr,
        map_type: MapType,
        map_perm: MapPermission
    ) -> Self {
        let start_vpn: VirtPageNum = start_va.floor();
        let end_vpn: VirtPageNum = end_va.ceil();
        // [WARNING]:因为没有map，所以不能使用
        //gdb_println!(MAP_ENABLE,"[MapArea new]: start_vpn:0x{:X} end_vpn:0x{:X}", start_vpn.0, end_vpn.0);
        Self {
            vpn_range: VPNRange::new(start_vpn, end_vpn),
            data_frames: BTreeMap::new(),
            map_type,
            map_perm,
        }
    }
    pub fn insert_tracker(&mut self, vpn: VirtPageNum, ppn: PhysPageNum) {
        self.data_frames.insert(vpn, FrameTracker::from_ppn(ppn));
    }
    pub fn from_another(another: &MapArea) -> Self {
        Self {
            vpn_range: VPNRange::new(another.vpn_range.get_start(), another.vpn_range.get_end()),
            data_frames: BTreeMap::new(),
            map_type: another.map_type,
            map_perm: another.map_perm,
        }
    }

    // Alloc and map one page
    pub fn map_one(&mut self, page_table: &mut PageTable, vpn: VirtPageNum) {
        // println!{"map one!!!"}
        let ppn: PhysPageNum;
        match self.map_type {
            MapType::Identical => {
                ppn = PhysPageNum(vpn.0);
            }
            MapType::Framed => {
                if let Some(frame) = frame_alloc(){
                    ppn = frame.ppn;
                    self.data_frames.insert(vpn, frame);
                }
                else{
                    print_free_pages();
                    panic!("No more memory!");
                }
            }
        }
        let pte_flags = PTEFlags::from_bits(self.map_perm.bits).unwrap();
        // [WARNING]:因为没有map，所以不能使用
        //gdb_println!(MAP_ENABLE,"[map_one]: pte_flags:{:?} vpn:0x{:X}",pte_flags,vpn.0);
        page_table.map(vpn, ppn, pte_flags);
    }
    pub fn unmap_one(&mut self, page_table: &mut PageTable, vpn: VirtPageNum) {
        match self.map_type {
            MapType::Framed => {
                self.data_frames.remove(&vpn);
            }
            _ => {}
        }
        page_table.unmap(vpn);
    }
    
    // Alloc and map all pages
    pub fn map(&mut self, page_table: &mut PageTable) {
        for vpn in self.vpn_range {
            self.map_one(page_table, vpn);
        }
    }
    pub fn unmap(&mut self, page_table: &mut PageTable) {
        for vpn in self.vpn_range {
            self.unmap_one(page_table, vpn);
        }
    }
    /// data: start-aligned but maybe with shorter length
    /// assume that all frames were cleared before
    pub fn copy_data(&mut self, page_table: &mut PageTable, data: &[u8], offset:usize) {
        assert_eq!(self.map_type, MapType::Framed);
        let mut start: usize = 0;
        let mut page_offset: usize = offset;
        let mut current_vpn = self.vpn_range.get_start();
        let len = data.len();
        loop { 
            let src = &data[start..len.min(start + PAGE_SIZE - page_offset)];
            let dst = &mut page_table
                .translate(current_vpn)
                .unwrap()
                .ppn()
                .get_bytes_array()[page_offset..(page_offset+src.len())];
            dst.copy_from_slice(src);

            start += PAGE_SIZE - page_offset;
            
            page_offset = 0;
            if start >= len {
                break;
            }
            current_vpn.step();
        }
    }
}

#[derive(Copy, Clone, PartialEq, Debug)]
pub enum MapType {
    Identical,
    Framed,
}

bitflags! {
    pub struct MapPermission: u8 {
        const R = 1 << 1;
        const W = 1 << 2;
        const X = 1 << 3;
        const U = 1 << 4;
    }
}

#[allow(unused)]
pub fn remap_test() {
    let mut kernel_space = KERNEL_SPACE.lock();
    let mid_text: VirtAddr = ((stext as usize + etext as usize) / 2).into();
    let mid_rodata: VirtAddr = ((srodata as usize + erodata as usize) / 2).into();
    let mid_data: VirtAddr = ((sdata as usize + edata as usize) / 2).into();
    assert_eq!(
        kernel_space.page_table.translate(mid_text.floor()).unwrap().writable(),
        false
    );
    assert_eq!(
        kernel_space.page_table.translate(mid_rodata.floor()).unwrap().writable(),
        false,
    );
    assert_eq!(
        kernel_space.page_table.translate(mid_data.floor()).unwrap().executable(),
        false,
    );
    println!("remap_test passed!");
}