# 2021-03-08 ~ 2021-03-15 SylixOS&Driver&FS-Continue

> 本周由于挑战杯事情耽搁了，不一定能够按时完成。
>
> 1. 主要研究如何访问mini2440上的Norflash；
> 3. 设计一个文件系统；
>
> 参考文献：
>
> 1. [裸机操作S3C2440 NorFlash](https://blog.csdn.net/Mculover666/article/details/104115535)
> 2. [mini2440 NorFlash芯片手册](../Files/Am29LV160DB.pdf)
> 3. [S3C2440 NorFlash裸机编程](https://blog.csdn.net/m0_37633745/article/details/103188617)

## 利用U-boot访问Norflash

1. 利用和[07-SylixOS&Driver&FS-Part1.md](./07-SylixOS&Driver&FS-Part1.md)相同的方法——利用Jlink向NorFlash烧录U-boot；

2. 进入Shell；

3. 输入下述序列，即可查看NorFlash的CFI信息：

   ```shell
   mw.w aa 98			# 进入CFI模式，在aa处写入0x98
   md.w 20 3			# 查看Q.R.Y Magic，读出0x20、0x22、0x24三个字
   md.w 4e 1			# 查看大小：2^21 次方，从0x4e处读出一个字
   mw.w 00 f0			# 退出CFI模式，向00处写入f0
   ```

   ![image-20210311230459261](G:\MyProject\Project.HoitOS\HoitOS\Records\Docs\images\mini2440-nor-cfi.png)



## 利用NorFlash启动SylixOS

> 感谢蒋老师的回复：
>
> Q：
>
> 蒋老师，你好，咱们SylixOs是烧录到mini2440的nandflash上，但是从nand flash启动没办法看到norflash，如果直接把系统烧到norflash上又太大装不了，那还该如何访问norflash呢？
>
> A：
>
> 有一种方法，我不知道可不可行，就是你用nand起来，nand的前4k不是负责搬运镜像么。你把nand的前4k拷贝出来烧录到nor的前4k里面，然后从nor启动是不是就可以搬运了。
>
> Q：
>
> 那直接用u-boot呢？
>
> A：
>
> 那也可以，用uboot也就是占用一部分空间。然后你映射文件系统时跳过这段地址；

很简单，只需要修改bootcmd即可：

```shell
//设置启动环境变量：从nand的0x60000处读取0x4ce000字节，写入ram的0x30000000处，然后CPU跳转至0x30000000运行；
setenv bootcmd "nand read 0x30000000 0x60000 0x4ce000; go 0x30000000"
//保存环境变量
saveenv
```

原理很简单，因为上述命令可以在RAM中成功执行，而NorFlash正是RAM-Like器件，因此毫无疑问，它也能执行上述指令。

## 在SylixOS中访问mini2440的NorFlash

> 感谢陈老师的回复：
>
> Q：
>
> 流程是这样的
>
> 1. 在nor上烧录uboot
> 2. 修改nor上的uboot的bootcmd, 也就是用老师你之前的方法
> 3. bootcmd将位于nand的SylixOS加载到30000000处
>
> 4. 通过上面链接的代码，想要查看norflash的cfi，但是提示写错误
>
> A：
>
> 在SylixOS中，访问的地址是虚拟地址，需要做映射。`API_VmmIoRemap`用这个接口

关键在于通过`API_VmmIoRemap`将地址0到2MB的地址映射到某个虚拟地址上：

```c
VOID nor_show_cfi(){
    NOR_FLASH_BASE = (UINT32)API_VmmIoRemap2(0, 2 * 1024 * 1024);
    printf("### Show CFI\n");
    ...
}
```

结果如图所示：

![image-20210312151304969](G:\MyProject\Project.HoitOS\HoitOS\Records\Docs\images\mini2440-nor.png)

## NorFlash驱动设计

### 一个很棘手的问题

这里有一个疑问，我们在Uboot中利用如下指令序列向内存中写的时候，按道理来说地址`mw.w 00 f0`是U-boot的起始地址，然而断电后再次启动时，U-boot仍能正常工作，这是为什么呢？

```shell
mw.w aa 98			# 进入CFI模式
md.w 20 3			# 查看Q.R.Y Magic
md.w 4e 1			# 查看大小：2^21 次方
mw.w 00 f0			# 退出CFI模式
```

事实上，我们向NorFlash的写入操作当且仅当我们输入指令解锁序列解锁后才能执行写操作，而在这之前，NorFlash均可以被看成一个纯净的Ram来使用。

### mini2440 NorFlash手册探索

通过查阅mini2440手册可以发现，我们是Am29LV160DB NorFlash；

![image-20210312163601714](G:\MyProject\Project.HoitOS\HoitOS\Records\Docs\images\mini2440-nroflash-analysis.png)

另外，这里再P24的地方是芯片指令说明，根据指令集，我们可以很容易知道mini2440的读写、擦除时序。根据这个时序，再加上参考[S3C2440 NorFlash裸机编程](https://blog.csdn.net/m0_37633745/article/details/103188617)代码，就可以开始着手在SylixOS中进行NorFlash驱动开发了。

![image-20210312165016203](G:\MyProject\Project.HoitOS\HoitOS\Records\Docs\images\mini2440-instruction-set.png)



## Am29LV160DB  NorFlash驱动

本驱动集成了Fake NorFlash以及真实的NorFlash，可以用于在`Real-Simulator`上模拟以及在`Am29LV160DB` NorFlash上运行。 

### 使用方法

#### 初始化

首先将[nor](../../Src/nor)文件夹放入`xxBSP/SylixOS/driver/mtd/`目录下（推荐放进mtd目录）。在**bspInit.c**中导入nor文件夹下的`nor.h`，然后利用`ctrl + F`在`xxBSP/SylixOS/bsp/bspInit.c`文件下找到`nand_init()`函数。接下来将介绍两种**Am29LV160DB  NorFlash驱动**的两种启动方式。

##### 以Fake方式启动

1. 在`createDevEx('\n')`下写上`nor_init(INIT_FAKE_NOR)`和`scan_nor()`；

2. 编译BSP工程，即可以Fake NorFlash的方式启动Am29LV160DB  NorFlash驱动；

3. 启动后结果如下：

   ![image-20210320140034549](G:\MyProject\Project.HoitOS\HoitOS\Records\Docs\images\am29-fake-nor-init.png)

##### 以True方式启动

1. 在`createDevEx('\n')`下写上`nor_init(INIT_TRUE_NOR)`和`scan_nor()`；
2. 编译BSP工程，即可以**真实Am29LV160DB  NorFlash驱动**的方式启动驱动；
3. 启动后结果如下：

#### API

Am29LV160DB  NorFlash驱动`nor.c`的API访问方式有两种，一是通过命令行，二是通过`nor.h`的函数头进行代码访问。

##### 通过命令行访问

无论是FAKE模式还是TRUE模式，`nor.c`的命令前缀均为`fls`，键入`fls -h`即可获取所有命令：

![image-20210320150057189](G:\MyProject\Project.HoitOS\HoitOS\Records\Docs\images\am29-nor-help.png)

- **概貌（summary）：**`fls [-s|-S]`、`fls`；
- **写（write）：**`fls [-w|-W] offset content size`、`fls [-w|-W] offset -o [o|Overwrite]`、`fls [-w|-W] offset -o [k|keep]`；
- **读（read）：**`fls [-r|-R] offset size`；
- **擦除（erase）：**`fls [-e|-E] offset`、`fls [-e|-E] offset -o [r|region]`、`fls [-e|-E] offset -o [s|sect|sector]`、`fls [-e|-E] offset -o [c|chip]`
- **重置（reset）**`fls -reset`
- **测试（test）**`fls [-t|-T]`

其中，`[xx|yy]`代表既可以是`xx`也可以是`yy`。

##### 通过代码访问

可供使用的API包括一些有用的宏定义或内联函数以及一些与`nor flash`读写操作相关的方法：

**内联函数或宏定义：**

- **GET_SECTOR_NO（offset）**：根据offset获取其所属sector号；
- **GET_SECTOR_OFFSET（sector_no）**：根据sector号获取该sector的起始地址的offset；
- **GET_SECTOR_SIZE（sector_no）**：根据sector号获取该sector的大小，单位为**KB**；
- **GET_SECTOR_REGION（sector_no）**：根据sector号获取该sector所属的region号，**注意，**在Am29LV160DB中，不再有`Block`定义，内存划分大小为：**CHIP > REGION > SECTOR**；  
- **IS_SECTOR_DIRTY（sector_no）**：根据sector号判断该sector是否已被写脏，即**判断该sector是否为非全1**；
- **NOR_FLASH_START_OFFSET**：获取NorFlash可写的起始地址，这之前的地址用于存放UBOOT文件；

**可用方法：**

- **VOID**  **nor_init**(**ENUM_NOR_INIT_FLAG** nor_init_flag)：![image-20210524172313314](08-SylixOS&Driver&FS-Continue-Part1.assets/image-20210524172313314.png)；

  用例：

  ```c
  #ifdef FAKE_NOR
  nor_init(INIT_FAKE_NOR);
  #else
  nor_init(INIT_TRUE_NOR);
  #endif
  ```

- **VOID**  **scan_nor**(**VOID**)：模拟扫描`nor flash`；

  用例：

  ```c
  scan_nor();
  ```

- **UINT8 **erase_nor**(**UINT** offset, **ERASE_OPTIONS** ops)：擦除`offset`处的内容，有三种选项：

  - ERASE_SECTOR：计算`offset`处所在的sector，擦除该sector；
  - ERASE_REGION：计算`offset`处所在的region，擦除该region（**在Am29LV160DB 中**，**共有4个region，每个region的组成不尽相同，可通过查阅其手册了解具体分布方式**）；
  - ERASE_CHIP：擦除整个`nor flash`，~~此时`offset`参数无效~~；

  用例：

  ```c
  UINT offset = NOR_FLASH_START_OFFSET;
  /* 擦除Sector */
  erase_nor(offset, ERASE_SECTOR);
  /* 擦除REGION */
  erase_nor(offset, ERASE_REGION);
  /* 擦除Chip */
  erase_nor(offset, ERASE_CHIP);
  ```

- **UINT8** **write_nor**(**UINT** offset, **PCHAR** content, **UINT** size_bytes, **ENUM_WRITE_OPTIONS** ops)：向`offset`处写入大小为`size_bytes`的`content`，有两种方式：

  - WRITE_KEEP：保留原来Sector中已有的内容；
  - WRITE_OVERWRITE：覆盖原来Sector中已有的内容；

  用例：

  ```c
  PCHAR conetnt = "hello power nor, and you know deadpool loves his star!";
  UINT offset = NOR_FLASH_START_OFFSET;
  /* 以WRITE_OVERWRITE方式写入content */
  write_nor(offset, content, lib_strlen(content), WRITE_OVERWRITE);
  
  PCHAR conetnt = "hello power nor, and you know deadpool loves his star!";
  UINT offset = NOR_FLASH_START_OFFSET + 1;
  /* 以WRITE_KEEP方式写入content */
  write_nor(offset, content, lib_strlen(content), WRITE_KEEP);
  /* 
  	此时NOR_FLASH_START_OFFSET处的内容应该是：
  	"hhello power nor, and you know deadpool loves his star!";
  */
  
  PCHAR conetnt = "hello power nor, and you know deadpool loves his star!";
  UINT offset = NOR_FLASH_START_OFFSET;
  /* 以WRITE_OVERWRITE方式再次写入content */
  write_nor(offset, content, lib_strlen(content), WRITE_OVERWRITE);
  /* 
  	此时NOR_FLASH_START_OFFSET处的内容应该是：
  	"hello power nor, and you know deadpool loves his starr!";
  */
  ```

- **UINT8** **read_nor**(**UINT** offset, **PCHAR** content, **UINT** size_bytes)：读取`offset`处的`size_bytes`的内容至`content`

  用例：

  ```c
  UINT offset = NOR_FLASH_START_OFFSET;
  PCHAR content = (PCHAR)lib_malloc(6);
  read_nor(NOR_FLASH_START_OFFSET, content, 5);
  *(content + 5) = '\0';
  printf("%s", content);
  ```

#### 注意事项

1. 写入、擦除起始地址应该通过宏`NOR_FLASH_START_OFFSET`获得；

2. 在使用`ERASE_CHIP`或`ERASE_REGION`的时候，会跳过**NOR_FLASH_START_OFFSET**之前的sector，并显示**no permission**，结果如下：

   ![image-20210320152451473](G:\MyProject\Project.HoitOS\HoitOS\Records\Docs\images\am29-nor-erase-chip.png)



