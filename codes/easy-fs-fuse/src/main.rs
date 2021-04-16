use easy_fs::{
    BlockDevice,
    EasyFileSystem,
    DiskInodeType,
    Inode,
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
    
    let dir0 = root_inode.create("dir0", DiskInodeType::Directory).unwrap();
    let file0 = dir0.create("file0", DiskInodeType::File).unwrap();     
    let test_str = "hello world!";
    file0.write_at(0, test_str.as_bytes());

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
    let root_inode = Arc::new(EasyFileSystem::get_inode(&efs,0));
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

    // 简易读写测试
    let simple_rwtest = |file_ino: Arc<Inode>, name: &str|{
        println!("*** simple r/w test");
        println!("  name = {}",name);
        println!("  1: write file. wlen={}", file_ino.write_at(0, greet_str.as_bytes()));
        let mut buffer = [0u8; 256];
        let len = file_ino.read_at(0, &mut buffer);
        println!("  2: read file. rlen = {}",len);
        assert_eq!(
            greet_str,
            core::str::from_utf8(&buffer[..len]).unwrap(),
        );
        println!("*** simple r/w test pass");
    };

    let list_apps = |dir_ino: Arc<Inode>, name:&str|{
        println!("list files in {}:",name);
        let file_vec = dir_ino.ls();
        for i in 0..file_vec.len(){
            if file_vec[i].1 == DiskInodeType::File{
                print!("{} ", file_vec[i].0);
            } else {
                // TODO: 统一配色！
             print!("{} ", color_text!(file_vec[i].0, 96));
            }
        }
        println!("");
        println!("");
    };


    // TODO:目录功能测试
    // 0.1 根目录下文件删除
    println!("*** after create filea fileb");
    println!("freeblk = {}", efs.lock().free_blocks());
    println!("freeinode = {}", efs.lock().free_inodes());
    assert_eq!(root_inode.remove(vec!["filea"], DiskInodeType::File), true, "fail to remove filea");
    println!("*** after delete filea");
    println!("freeblk = {}", efs.lock().free_blocks());
    println!("freeinode = {}", efs.lock().free_inodes());
    list_apps(root_inode.clone(),"root");
    assert_eq!(root_inode.remove(vec!["fileb"], DiskInodeType::File), true, "fail to remove fileb");
    println!("*** after delete filea fileb, before create dira");
    println!("freeblk = {}", efs.lock().free_blocks());
    println!("freeinode = {}", efs.lock().free_inodes());
    list_apps(root_inode.clone(),"root");

    // 1.1 根目录下目录创建
    println!("\n-----------------------------------------");  
    println!("0: rw in /dir ... start");
    let dira_inode_id = {
        println!("0.0: create dir");
        let dira_inode = root_inode.create("dira",DiskInodeType::Directory).unwrap();
        // 1.2 根目录下目录内文件创建/读写/删除
        println!("0.1: create file in dir");
        let filec = dira_inode.create("filec",DiskInodeType::File).unwrap();
        dira_inode.create("filed",DiskInodeType::File).unwrap();
        simple_rwtest(filec, "filec");
        dira_inode.get_id()
        // 到这里，filec和dira_inode被释放
    };
    println!("0.4: open dir");
    let dira_inode = Arc::new(EasyFileSystem::get_inode(&efs.clone(), dira_inode_id));
    println!("0.5: read from file");
    let (filec,_) = dira_inode.find_path(vec!["filec"]).unwrap();
    let len = filec.read_at(0, &mut buffer);
    assert_eq!(
        greet_str,
        core::str::from_utf8(&buffer[..len]).unwrap(),
    );
    // 打印根目录内容
    list_apps(root_inode.clone(), "root");
    println!("before create /dira/dir1");
    println!("freeblk = {}", efs.lock().free_blocks());
    println!("freeinode = {}", efs.lock().free_inodes());
    println!("0: rw in /dir ... {}",color_text!("pass",92));  
    
    println!("\n-----------------------------------------");  
    println!("1: rwd in multi-dir ... start");
    // 2.1 多级目录创建
    let dira_dir1 = dira_inode.create("dir1",DiskInodeType::Directory).unwrap();
    // 打印dira内容
    list_apps(dira_inode.clone(), "dira");
    
    // 2.2 多级目录下文件创建/读写/删除
    {
        dira_dir1.create("file0", DiskInodeType::File).unwrap();
    }
    let (dira_dir1_file0,_) = root_inode.find_path(vec!["dira","dir1","file0"]).unwrap();
    simple_rwtest(dira_dir1_file0.clone(), "/dira/dir1/file0");
    list_apps(dira_dir1.clone(), "dir1");
    println!("freeblk = {}", efs.lock().free_blocks());
    println!("freeinode = {}", efs.lock().free_inodes());
    // 2.3 多级目录删除
    println!("1.1: remove /dira/dir1");
    assert_eq!(root_inode.remove(vec!["dira","dir1"], DiskInodeType::Directory), true, "fail to remove dir1");
    list_apps(dira_inode.clone(), "dira");
    println!("freeblk = {}", efs.lock().free_blocks());
    println!("freeinode = {}", efs.lock().free_inodes());
    //simple_rwtest(dira_dir1_file0, "/dira/dir1/file0");
    println!("1: rwd in multi-dir ... {}\n",color_text!("pass",92));
    
    // 3.1 目录切换测试: cd ./..
    // 3.2 目录切换测试: 绝对路径
    // 3.3 目录切换测试: 相对路径
    // (需要通过内核测试)

    // 4 移动
    println!("\n-----------------------------------------");  
    println!("3: move test ... start");
    // 4.1 目录移动: 将/dira移动至/dir1/dirm(rename)
    let dir1 = root_inode.create("dir1",DiskInodeType::Directory).unwrap();
    list_apps(dira_inode.clone(), "/dira");
    assert_eq!(root_inode.fmove(vec!["dira"], 0, vec!["dir1","dirm"]),true,"move failed");
    println!("3.1: move /dira to /dir1/dirm (rename)");
    list_apps(dir1.clone(), "/dir1");
    let (dirm,_) = dir1.find_path(vec!["dirm"]).unwrap();
    list_apps(dirm.clone(), "/dir1/dirm");
    list_apps(root_inode.clone(), "root");
    let (filec,_) = root_inode.find_path(vec!["dir1","dirm","filec"]).unwrap();
    let len = filec.read_at(0, &mut buffer);
    assert_eq!(
        greet_str,
        core::str::from_utf8(&buffer[..len]).unwrap(),
    );
    // 4.2 目录移动：将/dir1/dirm移动至/(不重命名)
    println!("3.2: move /dir1/dirm to / (without rename)");
    assert_eq!(dir1.fmove(vec!["dirm"], 0, vec![""]),true,"move failed");
    let (dirm,_) = root_inode.find_path(vec!["dirm"]).unwrap();
    list_apps(dir1.clone(), "/dir1");
    list_apps(root_inode.clone(), "root");
    list_apps(dirm.clone(), "/dirm");
    let (filec,_) = root_inode.find_path(vec!["dirm","filec"]).unwrap();
    let len = filec.read_at(0, &mut buffer);
    assert_eq!(
        greet_str,
        core::str::from_utf8(&buffer[..len]).unwrap(),
    );
    // 4.3 文件移动: 将filec移动至/
    println!("3.3: move /dirm/filec to /");
    assert_eq!(dirm.fmove(vec!["filec"], 0, vec![""]),true,"move failed");
    list_apps(dirm.clone(), "dirm");
    list_apps(root_inode.clone(), "root");
    let (filec,_) = root_inode.find_path(vec!["filec"]).unwrap();
    let len = filec.read_at(0, &mut buffer);
    assert_eq!(
        greet_str,
        core::str::from_utf8(&buffer[..len]).unwrap(),
    );
    println!("3: move test ... {}\n",color_text!("pass",92));

    // 5 复制
    println!("\n-----------------------------------------");  
    println!("4: copy test ... start");
    // 5.1 文件复制
    println!("4.1 copy file");
    // 打印块分配情况
    println!("freeblk = {}", efs.lock().free_blocks());
    println!("freeinode = {}", efs.lock().free_inodes());
    // 复制
    root_inode.fcopy(vec!["filec"], 0, vec!["dirm"]);
    // 打印块分配情况
    println!("freeblk = {}", efs.lock().free_blocks());
    println!("freeinode = {}", efs.lock().free_inodes());
    // 检查复制后的原文件，打印所在目录
    list_apps(root_inode.clone(), "root");
    let (filec,_) = root_inode.find_path(vec!["filec"]).unwrap();
    let len = filec.read_at(0, &mut buffer);
    assert_eq!(
        greet_str,
        core::str::from_utf8(&buffer[..len]).unwrap(),
    );
    // 检查复制后的新文件，打印所在目录
    list_apps(dirm.clone(), "dirm");
    let (filec,_) = root_inode.find_path(vec!["dirm","filec"]).unwrap();
    let len = filec.read_at(0, &mut buffer);
    assert_eq!(
        greet_str,
        core::str::from_utf8(&buffer[..len]).unwrap(),
    );

    // 5.2 目录复制
    println!("4: copy test ... {}\n",color_text!("to be continue",33));
    //println!("4: move test ... {}\n",color_text!("pass",92));

    // 6 鲁棒性测试
    println!("\n-----------------------------------------");  
    println!("5: Robust test ... start");
    // 6.1 尝试操作不存在的文件/目录
    if dira_inode.find_path(vec!["dir1"]).is_some() {
        panic!("find unexist dir!");
    }
    
    if root_inode.find_path(vec!["filea"]).is_some(){
        panic!("find unexist file!");
    }
    // 6.2上级目录复制到子目录
    let dirn = dirm.create("dirn",DiskInodeType::Directory).unwrap();
    assert_eq!( root_inode.fmove(vec!["dirm"],0,vec!["dirm","dirn"]), false);
    // 目录复制到原地
    assert_eq!( root_inode.fmove(vec!["dirm"],0,vec!["dirm"]), false);
    // 文件复制到原地
    assert_eq!( root_inode.fmove(vec!["filec"],0,vec!["filec"]), false);
    // 6.3 大小超出限制
    println!("5: Robust test ... to be continue\n");

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
    println!("random str rw test ... {}",color_text!("pass",92));
    Ok(())
}