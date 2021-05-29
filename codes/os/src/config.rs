#[allow(unused)]

// pub const O_RDONLY: usize = 0x000;
// pub const O_WRONLY: usize = 0x001;
// pub const O_RDWR: usize = 0x002; // 可读可写
// //#define O_CREATE 0x200
// pub const O_CREATE: usize = 0x40;
// pub const O_DIRECTORY: usize = 0x0200000;

// for mmap
pub const PROT_NONE: usize = 0;
pub const PROT_READ: usize = 1;
pub const PROT_WRITE: usize = 2;
pub const PROT_EXEC: usize = 4;
pub const PROT_GROWSDOWN: usize = 0x01000000;
pub const PROT_GROWSUP: usize = 0x02000000;

pub const MAP_FILE: usize = 0;
pub const MAP_SHARED: usize = 0x01;
pub const MAP_PRIVATE: usize = 0x02;
pub const MAP_FAILED: isize = -1;

pub const USER_STACK_SIZE: usize = 4096 * 2;
pub const USER_HEAP_SIZE: usize = 4096 * 16;
pub const KERNEL_STACK_SIZE: usize = 4096 * 2;
pub const KERNEL_HEAP_SIZE: usize = 0x20_0000;
pub const MEMORY_END: usize = 0x80800000;
pub const PAGE_SIZE: usize = 0x1000;
pub const PAGE_SIZE_BITS: usize = 0xc;

pub const MMAP_BASE: usize = 0x70000000;
pub const TRAMPOLINE: usize = usize::MAX - PAGE_SIZE + 1;
pub const TRAP_CONTEXT: usize = TRAMPOLINE - PAGE_SIZE;

#[cfg(feature = "board_k210")]
pub const CLOCK_FREQ: usize = 403000000 / 62;

#[cfg(feature = "board_qemu")]
pub const CLOCK_FREQ: usize = 12500000;

#[cfg(feature = "board_qemu")]
pub const MMIO: &[(usize, usize)] = &[
    (0x10001000, 0x1000),
];

#[cfg(feature = "board_k210")]
pub const MMIO: &[(usize, usize)] = &[
    // we don't need clint in S priv when running
    // we only need claim/complete for target0 after initializing
    (0x0C00_0000, 0x3000),      /* PLIC      */
    (0x0C20_0000, 0x1000),      /* PLIC      */
    (0x3800_0000, 0x1000),      /* UARTHS    */
    (0x3800_1000, 0x1000),      /* GPIOHS    */
    (0x5020_0000, 0x1000),      /* GPIO      */
    (0x5024_0000, 0x1000),      /* SPI_SLAVE */
    (0x502B_0000, 0x1000),      /* FPIOA     */
    (0x502D_0000, 0x1000),      /* TIMER0    */
    (0x502E_0000, 0x1000),      /* TIMER1    */
    (0x502F_0000, 0x1000),      /* TIMER2    */
    (0x5044_0000, 0x1000),      /* SYSCTL    */
    (0x5200_0000, 0x1000),      /* SPI0      */
    (0x5300_0000, 0x1000),      /* SPI1      */
    (0x5400_0000, 0x1000),      /* SPI2      */
];