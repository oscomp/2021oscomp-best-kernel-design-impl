use alloc::vec::Vec;

#[allow(unused)]
pub fn str2vec(str: &str) -> Vec<&str>{
    str.split('/').collect()
}