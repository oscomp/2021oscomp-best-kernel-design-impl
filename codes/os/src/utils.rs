

pub fn log2(num:usize) -> usize{
    let mut num = num;
    if num == 0{
        return 0;
    }
    for i in 0..64{
        if num > 0 {
            num = num >> 1;
        }
        else{
            return i-1;
        }
    }
    64
}
