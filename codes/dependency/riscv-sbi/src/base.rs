//! SBI Base Functionality, Extension ID 0x10
//!
//! The base of the SBI is designed to be as small as possible.
//! As such, it only contains functionality for probing which SBI extensions
//! are available and for querying the version of the SBI. All functions in
//! the base must be supported by all SBI implementations, so there are no
//! error returns defined.
//!
//! Ref: https://github.com/riscv/riscv-sbi-doc/blob/master/riscv-sbi.adoc#sbi-base-functionality-extension-id-0x10

use super::*;

/// Returns the current SBI specification version.
pub fn get_spec_version() -> SpecVersion {
    let value = sbi_call(0x10, 0, 0, 0, 0).unwrap();
    // The minor number of the SBI specification is encoded in the low 24 bits,
    // with the major number encoded in the next 7 bits.
    // Bit 31 must be 0 and is reserved for future expansion.
    assert_eq!(value & (1 << 31), 0);
    SpecVersion {
        minor: value as u32 & ((1 << 24) - 1),
        major: (value >> 24) as u8,
    }
}

/// Returns the current SBI implementation ID, which is different for every SBI
/// implementation.
///
/// It is intended that this implementation ID allows software
/// to probe for SBI implementation quirks.
pub fn get_impl_id() -> Implementation {
    match sbi_call(0x10, 1, 0, 0, 0).unwrap() {
        0 => Implementation::BerkeleyBootLoader,
        1 => Implementation::OpenSBI,
        2 => Implementation::Xvisor,
        3 => Implementation::KVM,
        t => Implementation::Unknown(t as u8),
    }
}

/// Returns the current SBI implementation version.
pub fn get_impl_version() -> usize {
    sbi_call(0x10, 2, 0, 0, 0).unwrap()
}

/// Returns 0 if the given extension ID is not available,
/// or an extension-specific non-zero value if it is available.
pub fn probe_extension(extension_id: usize) -> usize {
    sbi_call(0x10, 3, extension_id, 0, 0).unwrap()
}

/// Return a value that is legal for the mvendorid CSR.
pub fn get_mvendorid() -> usize {
    sbi_call(0x10, 4, 0, 0, 0).unwrap()
}

/// Return a value that is legal for the marchid CSR.
pub fn get_marchid() -> usize {
    sbi_call(0x10, 5, 0, 0, 0).unwrap()
}

/// Return a value that is legal for the mimpid CSR.
pub fn get_mimpid() -> usize {
    sbi_call(0x10, 6, 0, 0, 0).unwrap()
}

/// SBI specification version.
#[derive(Debug, Copy, Clone, Eq, PartialEq)]
pub struct SpecVersion {
    /// Major version.
    pub major: u8,
    /// Minor version.
    pub minor: u32,
}

/// SBI implementation ID.
#[derive(Debug, Copy, Clone, Eq, PartialEq)]
#[allow(missing_docs)]
pub enum Implementation {
    BerkeleyBootLoader,
    OpenSBI,
    Xvisor,
    KVM,
    Unknown(u8),
}
