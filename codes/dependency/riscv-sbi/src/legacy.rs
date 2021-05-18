//! Legacy SBI Extension, Extension IDs 0x00 through 0x0F
//!
//! Ref: https://github.com/riscv/riscv-sbi-doc/blob/master/riscv-sbi.adoc#legacy-sbi-extension-extension-ids-0x00-through-0x0f
use crate::hart_mask::HartMask;

#[inline(always)]
fn sbi_call(which: usize, arg0: usize, arg1: usize, arg2: usize, arg3: usize) -> usize {
    // allows to build code in RISC-V and test code on other platforms
    #[allow(clippy::match_single_binding)]
    match () {
        #[cfg(target = "riscv")]
        () => unsafe {
            let ret;
            asm!(
                "ecall",
                lateout("a0") ret,
                in("a0") arg0,
                in("a1") arg1,
                in("a2") arg2,
                in("a3") arg3,
                in("a7") which,
                options(nostack)
            );
            ret
        },
        #[cfg(not(target = "riscv"))]
        () => unimplemented!("{} {} {} {} {}", which, arg0, arg1, arg2, arg3),
    }
}

/// Write data present in `ch` to debug console.
///
/// Unlike [`console_getchar`], this SBI call will block if there remain any pending characters
/// to be transmitted or if the receiving terminal is not yet ready to receive the byte.
/// However, if the console doesn’t exist at all, then the character is thrown away.
///
/// [`console_getchar`]: console_getchar
pub fn console_putchar(ch: usize) {
    sbi_call(SBI_CONSOLE_PUTCHAR, ch, 0, 0, 0);
}

/// Read a byte from debug console
///
/// Returns the byte on success, or -1 for failure.
pub fn console_getchar() -> usize {
    sbi_call(SBI_CONSOLE_GETCHAR, 0, 0, 0, 0)
}

/// Puts all the harts to shut down state from supervisor point of view.
///
/// This SBI call doesn’t return.
pub fn shutdown() -> ! {
    sbi_call(SBI_SHUTDOWN, 0, 0, 0, 0);
    unreachable!()
}

/// Programs the clock for next event after `stime_value` time.
///
/// This function also clears the pending timer interrupt bit.
///
/// If the supervisor wishes to clear the timer interrupt without scheduling the next timer event,
/// it can either request a timer interrupt infinitely far into the future (i.e., (uint64_t)-1),
/// or it can instead mask the timer interrupt by clearing `sie.STIE`.
pub fn set_timer(stime_value: u64) {
    #[cfg(target_pointer_width = "32")]
    sbi_call(
        SBI_SET_TIMER,
        stime_value as usize,
        (stime_value >> 32) as usize,
        0,
        0,
    );
    #[cfg(target_pointer_width = "64")]
    sbi_call(SBI_SET_TIMER, stime_value as usize, 0, 0, 0);
}

/// Clears the pending IPIs if any.
///
/// The IPI is cleared only in the hart for which this SBI call is invoked.
pub fn clear_ipi() {
    sbi_call(SBI_CLEAR_IPI, 0, 0, 0, 0);
}

/// Send an inter-processor interrupt to all the harts defined in `hart_mask`.
///
/// `hart_mask` is a virtual address that points to a bit-vector of harts. The bit vector is
/// represented as a sequence of unsigned longs whose length equals the number of harts in the
/// system divided by the number of bits in an unsigned long, rounded up to the next integer.
pub fn send_ipi(hart_mask: &HartMask) {
    sbi_call(SBI_SEND_IPI, hart_mask.as_ptr() as usize, 0, 0, 0);
}

/// Instructs remote harts to execute `FENCE.I` instruction.
///
/// N.B. `hart_mask` is as described in [`send_ipi`].
///
/// [`send_ipi`]: send_ipi
pub fn remote_fence_i(hart_mask: &HartMask) {
    sbi_call(SBI_REMOTE_FENCE_I, hart_mask.as_ptr() as usize, 0, 0, 0);
}

/// Instructs the remote harts to execute one or more `SFENCE.VMA` instructions,
/// covering the range of virtual addresses between `start` and `size`.
pub fn remote_sfence_vma(hart_mask: &HartMask, start: usize, size: usize) {
    sbi_call(
        SBI_REMOTE_SFENCE_VMA,
        hart_mask.as_ptr() as usize,
        start,
        size,
        0,
    );
}

/// Instruct the remote harts to execute one or more `SFENCE.VMA` instructions,
/// covering the range of virtual addresses between `start` and `size`.
/// This covers only the given `ASID`.
pub fn remote_sfence_vma_asid(hart_mask: &HartMask, start: usize, size: usize, asid: usize) {
    sbi_call(
        SBI_REMOTE_SFENCE_VMA_ASID,
        hart_mask.as_ptr() as usize,
        start,
        size,
        asid,
    );
}

const SBI_SET_TIMER: usize = 0;
const SBI_CONSOLE_PUTCHAR: usize = 1;
const SBI_CONSOLE_GETCHAR: usize = 2;
const SBI_CLEAR_IPI: usize = 3;
const SBI_SEND_IPI: usize = 4;
const SBI_REMOTE_FENCE_I: usize = 5;
const SBI_REMOTE_SFENCE_VMA: usize = 6;
const SBI_REMOTE_SFENCE_VMA_ASID: usize = 7;
const SBI_SHUTDOWN: usize = 8;
