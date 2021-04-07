use easy_fs::{
    BlockDevice,
    EasyFileSystem,
    DiskInodeType,
};
use std::fs::{File, OpenOptions, read_dir};
use std::io::{Read, Write, Seek, SeekFrom};
use std::sync::Mutex;
use std::sync::Arc;
use clap::{Arg, App};

const BLOCK_SZ: usize = 512;

struct BlockFile(Mutex<File>);

impl BlockDevice for BlockFile {
    fn read_block(&self, block_id: usize, buf: &mut [u8]) {
        let mut file = self.0.lock().unwrap();
        file.seek(SeekFrom::Start((block_id * BLOCK_SZ) as u64))
            .expect("Error when seeking!");
        assert_eq!(file.read(buf).unwrap(), BLOCK_SZ, "Not a complete block!");
    }

    fn write_block(&self, block_id: usize, buf: &[u8]) {
        let mut file = self.0.lock().unwrap();
        file.seek(SeekFrom::Start((block_id * BLOCK_SZ) as u64))
            .expect("Error when seeking!");
        assert_eq!(file.write(buf).unwrap(), BLOCK_SZ, "Not a complete block!");
    }
}

fn main() {
    //efs_test();
    easy_fs_pack().expect("Error when packing easy-fs!");
}

fn easy_fs_pack() -> std::io::Result<()> {
    // clap::matches 用于捕获用户输入的参数
    // 在makefile中，命令为
    // @cd ../easy-fs-fuse && cargo run --release \
    // -- -s ../user/src/bin/ \
    // -t ../user/target/riscv64gc-unknown-none-elf/release/
    // 因此得到的参数就是两个路径
    let matches = App::new("EasyFileSystem packer")
        .arg(Arg::with_name("source")
            .short("s") // 对应输入的 -s
            .long("source")//对应输入 --source
            .takes_value(true)
            .help("Executable source dir(with backslash)")
        )
        .arg(Arg::with_name("target")
            .short("t")
            .long("target")
            .takes_value(true)
            .help("Executable target dir(with backslash)")    
        )
        .get_matches();
    let src_path = matches.value_of("source").unwrap();
    let target_path = matches.value_of("target").unwrap();
    println!("src_path = {}\ntarget_path = {}", src_path, target_path);
    
    // 创建fs.img
    let block_file = Arc::new(BlockFile(Mutex::new({
        let f = OpenOptions::new()
            .read(true)
            .write(true)
            .create(true)
            .open(format!("{}{}", target_path, "fs.img"))?;
        f.set_len(8192 * 512).unwrap();
        f
    })));
    
    // 4MiB, at most 4095 files
    // 512*8 = 4095 * file_inode + 1 * root_inode
    let efs = EasyFileSystem::create(
        block_file.clone(),
        8192,//只有4MB。。。
        1,
    );
    let root_inode = Arc::new(EasyFileSystem::get_inode(&efs,0));
    
    // 从host获取应用名
    let apps: Vec<_> = read_dir(src_path)
        .unwrap()
        .into_iter()
        .map(|dir_entry| {
            let mut name_with_ext = dir_entry.unwrap().file_name().into_string().unwrap();
            // 丢弃后缀 从'.'到末尾(len-1)
            name_with_ext.drain(name_with_ext.find('.').unwrap()..name_with_ext.len());
            name_with_ext
        })
        .collect();
    for app in apps {
        // load app data from host file system
        let mut host_file = File::open(format!("{}{}", target_path, app)).unwrap();
        let mut all_data: Vec<u8> = Vec::new();
        host_file.read_to_end(&mut all_data).unwrap();
        // create a file in easy-fs
        let inode = root_inode.create(app.as_str(), DiskInodeType::File).unwrap();
        // write data to easy-fs
        inode.write_at(0, all_data.as_slice());
    }
    // list apps

    for app in root_inode.ls() {
        println!("{}", app.0);
    }
    Ok(())
}

macro_rules! color_text {
    ($text:expr, $color:expr) => {{
        format_args!("\x1b[{}m{}\x1b[0m", $color, $text)
    }};
}

#[test]
fn efs_test() -> std::io::Result<()> {
    let block_file = Arc::new(BlockFile(Mutex::new({
        let f = OpenOptions::new()
            .read(true)
            .write(true)
            .create(true)
            .open("target/fs.img")?;
        f.set_len(8192 * 512).unwrap();
        f
    })));
    EasyFileSystem::create(
        block_file.clone(),
        4096,    
        1,
    );
    let efs = EasyFileSystem::open(block_file.clone());
    println!("freeblk = {}", efs.lock().free_blocks());
    println!("freeinode = {}", efs.lock().free_inodes());
    let root_inode = EasyFileSystem::get_inode(&efs,0);
    root_inode.create("filea",DiskInodeType::File);
    root_inode.create("fileb",DiskInodeType::File);
    for name in root_inode.ls() {
        println!("{}", name.0);
    }
    let (filea,_) = root_inode.find_path(vec!["filea"]).unwrap();
    let greet_str = "Hello, world!";
    filea.write_at(0, greet_str.as_bytes());
    // let mut buffer = [0u8; 512];
    let mut buffer = [0u8; 256];
    let len = filea.read_at(0, &mut buffer);
    assert_eq!(
        greet_str,
        core::str::from_utf8(&buffer[..len]).unwrap(),
    );

    // TODO:目录功能测试
    // 0.1 根目录下文件删除
    
    // 1.1 根目录下目录创建
    println!("0: rw in /dir ... start");
    let dira_inode_id = {
        println!("0.0: create dir");
        let dira_inode = root_inode.create("dira",DiskInodeType::Directory).unwrap();
        // 1.2 根目录下目录内文件创建/读写/删除
        println!("0.1: create file in dir");
        let filec = dira_inode.create("filec",DiskInodeType::File).unwrap();
        println!("0.2: write file. wlen={}", filec.write_at(0, greet_str.as_bytes()));
        let len = filec.read_at(0, &mut buffer);
        println!("0.3: read file. rlen = {}",len);
        assert_eq!(
            greet_str,
            core::str::from_utf8(&buffer[..len]).unwrap(),
        );
        dira_inode.get_id()
        // 到这里，filec和dira_inode被释放
    };
    println!("0.4: open dir");
    let dira_inode = Arc::new(EasyFileSystem::get_inode(&efs.clone(), dira_inode_id));
    println!("0.5: read from file");
    let (filec,_) = dira_inode.find_path(vec!["filec"]).unwrap();
    let mut buffer = [0u8; 233];
    let len = filec.read_at(0, &mut buffer);
    assert_eq!(
        greet_str,
        core::str::from_utf8(&buffer[..len]).unwrap(),
    );
    // 打印根目录内容
    println!("list files in root:");
    let file_vec = root_inode.ls();
    
    for i in 0..file_vec.len(){
        if file_vec[i].1 == DiskInodeType::File{
            print!("{} ", file_vec[i].0);
        } else {
            // TODO: 统一配色！
            print!("{} ", color_text!(file_vec[i].0, 96));
        }
    }
    println!("");
    println!("0: rw in /dir ... pass");

    // 1.3 根目录下目录删除
    
    
    // 2.1 多级目录创建
    // 2.2 多级目录下文件创建/读写/删除
    // 2.3 多级目录删除
    
    // 3.1 目录切换测试: cd ./..
    // 3.2 目录切换测试: 绝对路径
    // 3.3 目录切换测试: 相对路径

    // 4 鲁棒性测试
    // 4.1 尝试操作不存在的文件/目录
    // 4.2 大小超出限制


    // 文件数据块分配回收测试(superblock/inode的size是否及时增减)

    // 随机字符串读写测试
    println!("random str rw test ... start");
    let mut random_str_test = |len: usize| {
        filea.clear();
        assert_eq!(
            filea.read_at(0, &mut buffer),
            0,
        );
        let mut str = String::new();
        use rand;
        // random digit
        for _ in 0..len {
            str.push(char::from('0' as u8 + rand::random::<u8>() % 10));
        }
        filea.write_at(0, str.as_bytes());
        let mut read_buffer = [0u8; 127];
        let mut offset = 0usize;
        let mut read_str = String::new();
        loop {
            let len = filea.read_at(offset, &mut read_buffer);
            if len == 0 {
                break;
            }
            offset += len;
            read_str.push_str(
                core::str::from_utf8(&read_buffer[..len]).unwrap()
            );
        }
        assert_eq!(str, read_str);
    };

    random_str_test(4 * BLOCK_SZ);
    random_str_test(8 * BLOCK_SZ + BLOCK_SZ / 2);
    random_str_test(100 * BLOCK_SZ);
    random_str_test(70 * BLOCK_SZ + BLOCK_SZ / 7);
    random_str_test((12 + 128) * BLOCK_SZ);
    random_str_test(400 * BLOCK_SZ);
    random_str_test(1000 * BLOCK_SZ);
    random_str_test(2000 * BLOCK_SZ);
    println!("random str rw test ... pass");
    Ok(())
}