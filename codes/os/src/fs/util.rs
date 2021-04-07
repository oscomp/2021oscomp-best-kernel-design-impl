use alloc::vec::Vec;

pub fn str2vec(str: &str) -> Vec<&str>{
    str.split('/').collect()
}