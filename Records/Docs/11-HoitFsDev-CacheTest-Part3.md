## Cache测试

1. 分配八个块，写满八个块，读取八个块。
2. flush八个块成功。
3. 测试cache分配块满之后换块成功。



## 文件树脚本

1.open函数

头文件：

```c
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
#include "../SylixOS/shell/include/ttiny_shell.h"
#include "sys/ioctl.h"
```

函数调用：

```C
iFd = open(FileName, O_WRONLY | O_CREAT | O_TRUNC, DEFAULT_FILE_PERM);
```

2.mkdir函数

头文件：

```c
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
#include "../SylixOS/shell/include/ttiny_shell.h"
#include "sys/ioctl.h"
```

函数调用

```c
mode = DEFAULT_DIR_PERM;
mode &= ~S_IFMT;
iFd = open(pcDirName, O_RDWR | O_CREAT | O_EXCL, S_IFDIR | mode);   /*  排他性创建  */
```

3.测试指令

```powershell
mount -t hoitfs 0 /mnt/hoitfs
mount -t ramfs 100000 /mnt/ram
umount /mnt/ram
cd /apps/FileTreeTest
/apps/FileTreeTest/FileTreeTest
ls /mnt/hoit
/* 或者直接使用指令 */
hoit -t ftt -h
hoit -t ftt -t 4 3 2
```

4.手动测试

```powershell
mkdir Newdir0
touch Newfile0
echo Hello_hoitfs1234 >
cat 
```

5.出现问题

创建13个文件和4个目录久报错了。

### 重叠写测试

1. 测试指令

```powershell
mount -t hoitfs 0 /mnt/hoitfs
mount -t ramfs 10000 /mnt/ram
umount /mnt/ram
cd /apps/FileOverWriteTest/
/apps/FileOverWriteTest/FileOverWriteTest/
cat /mnt/ram/OverWriteTest
/*或直接使用指令*/
hoit -t fot
```



### 软硬链接

1.软链接函数

```
/*
	pcLinkDst:源文件路径（文件系统中）
	pcSymPath:软链接路径（文件系统中）
	似乎是自动查找文件处于哪个文件系统中，然后进行软链接
*/
INT  symlink (CPCHAR  pcLinkDst, CPCHAR  pcSymPath)
```



## 待测试

## mount参数

> pcDevName = '\0' //就是hoitfs 后面那个参数
>
> pcVolName = "/mnt/hoitfs\0"
>
> pcFileSystem = "hoitfs\0"
>
> pcOption = LW_NULL

