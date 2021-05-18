#![no_std]
#![no_main]

#[macro_use]
extern crate log;
#[macro_use]
extern crate opensbi_rt;

use log::LevelFilter;
use opensbi_rt::sbi;

#[no_mangle]
extern "C" fn main(hartid: usize, dtb: usize) {
    log::set_max_level(LevelFilter::Info);
    println!("Hello, OpenSBI!");
    println!("hartid={}, dtb={:#x}", hartid, dtb);
    println!("spec_version = {:?}", sbi::base::get_spec_version());
    println!("impl_id      = {:?}", sbi::base::get_impl_id());
    println!("impl_version = {:?}", sbi::base::get_impl_version());
    println!("mvendorid    = {:?}", sbi::base::get_mvendorid());
    println!("marchid      = {:?}", sbi::base::get_marchid());
    println!("mimpid       = {:?}", sbi::base::get_mimpid());
}
