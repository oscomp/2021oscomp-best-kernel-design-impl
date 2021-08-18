#### 比赛的系统调用

|      | 系统调用号                | 描述                     | 完成情况 |
| ---- | ------------------------- | ------------------------ | -------- |
| 1    | #define SYS_getcwd 17     |                          | fin      |
| 2    | #define SYS_pipe2 59      |                          | fin      |
| 3    | #define SYS_dup 23        |                          | fin      |
| 4    | #define SYS_dup3 24       |                          | fin      |
| 5    | #define SYS_chdir 49      |                          | fin      |
| 6    | #define SYS_openat 56     |                          | fin      |
|      | 测试点：open              | 使用当前目录下的相对路径 | fin      |
| 7    | #define SYS_close 57      |                          | fin      |
| 8    | #define SYS_getdents64 61 |                          | fin      |
| 9    | #define SYS_read 63       |                          | fin      |
| 10   | #define SYS_write 64      |                          | fin      |
| 13   | #define SYS_mkdirat 34    |                          | fin      |
| 14   | #define SYS_umount2 39    |                          | fin      |
| 15   | #define SYS_mount 40      |                          | fin      |
| 16   | #define SYS_fstat 80      |                          | fin      |

#### 当前工作路径的设计

在rCore里，在进程控制块存储了当前路径的字符串，以栈的形式操作

这样的方法可以适应不同的文件系统，并且也利于返回当前工作路径。如果进程控制块存储的是当前路径的虚拟文件指针，那么用户获得路径就要回溯，但是，.. 并不指向上级目录的目录项，而是直接指向目录的簇，因此无法获得上级的文件名，这会大大降低效率。

因此，我们也决定采用这种方法，在TCB存储字符串。

#### fd表的设计

之前以Rust的dyn将实现了File特性的引用存于fd_table

open_at基于fd获取路径，而File特性并没有这样的接口。因此需要改变TCB的存储对象。

为此设计了文件枚举类，其结构如下：

```rust
pub enum FileClass {
    File (Arc<OSInode>), // 指向真实的文件
    Abstr (Arc<dyn File + Send + Sync>),  // 指向抽象文件，比如stdio
}
```

Rust也采用了类似的设计，但是他们的stdio也为文件，文件对象为串口设备（大概

我们对普通文件和抽象实现的File接口并不一致，因此将他们区分处理

为了支持设备文件，我们对还File trait进行了扩展，新增ioctl方法

#### 部分系统调用实现

文件系统大部分系统调用都是易于实现的，难点主要在于文件信息获取(fstat、getdent)以及挂载。

##### UserBuffer

许多文件系统的syscall会传入用户的buffer。难点在于，buffer可能是跨页的，因此不能仅对buffer指针强转然后操作。为此我们定义了UserBuffer结构：

```Rust
pub struct UserBuffer {
    pub buffers: Vec<&'static mut [u8]>,
}
impl UserBuffer {
    pub fn new(buffers: Vec<&'static mut [u8]>) -> Self;
    pub fn len(&self) -> usize;
    pub fn write(&mut self, buff: &[u8])->usize;
    pub fn read(&self, buff:&mut [u8])->usize;
}
```

其为u8数组引用的向量，这样在内核操作的时候可以将存在于不同页的buffer分离存储。

read/write等系统调用都使用了该结构。

##### getdent/fstat

​		这两个系统调用比较类似，也有共同的难点。对于这两个系统调用，用户会传入一个buffer的指针，内核需要以结构体的形式填入需要的信息。难点在于，buffer可能是跨页的，因此不能通过页表翻译而直接填写，而是要像read/write一样使用UserBuffer。

​		另一个问题在于，这两个系统调用要使用结构体，且是与C语言实现的用户程序共用结构体。幸运的是，Rust提供了#[repr(C)]标记以支持C格式的结构体。

##### 挂载

​		我们的操作系统暂不支持设备管理，事实上，目前也仅支持SD Card这一块设备。目前我们所作的就是维护挂载表。