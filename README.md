![QQ图片20210531114657](README.assets/logo.png)

# HOIT-FS

We are team "HOIT-23o2" from harbin institute of technology,shenzhen. We are now developing on proj32-NorFlash-Filesystem-on-SylixOS. Keep tuning in.👏

## Docs

- This is the main doc for stage 1 submit: [初赛报告文档](./HoitFS设计开发文档.pdf)
- This is the link of development doc for stage 1 submit: [README_HOITFS](./README_HOITFS.md)
- **Note:** [repository](https://github.com/Hoit-23o2/HoitOS.git) in github maintains the newest repo, this repo is a replica of that, only for submit.

## Goals & Achievement

The goals we must achieve:

1. Basic read, write, seek, mkdir, mv and so on.
2. Hard and soft link, write balance.
3. How to make sure the lowest cost of power loss.****

We have achieved that:  

| 目标编号 |   基本完成情况   | 额外说明                                                     |
| :------: | :--------------: | ------------------------------------------------------------ |
|    1     | 基本完成（≈90%） | ① **基本 I/O 功能**已通过脚本测试；<br>② 可能潜在未发现的BUG |
|    2     | 基本完成（≈90%） | ① **软、硬链接**已通过脚本测试；<br/>② JFFS2 自带**磨损均衡**机制，可以引入**更多 WL 算法**； |
|    3     | 基本完成（≈80%） | ① 引入日志层，**理论上能够保证强掉电安全**。<br/>     但我们还**未实验评估**其带来的优势；<br/>② 将带来更多写开销， 正在研究解决方案； |
|   总计   | 基本完成（≈90%） | 还有更多优化工作可以完成；                                   |

## Future Works

- **More Stable**

  HoitFS目前可能还不够稳定，我们认为HoitFS部分代码的实现不够Elegant，未来我们会进行统一的Code Review，从而有效提升代码质量；

- **More efficient**

  目前仅用单链表结构来进行块级结构管理，而块选择算法是基于贪心策略完成 的，因此这会带来磨损不平衡问题，在下一步计划中，我们将采用多级链表管理 结构，将对不同的块进行分类，从而缓解磨损不均衡问题；

- **WL & Verify**

  日志系统会带来额外的写开销（我们直接将日志写回Norflash上），这为擦写寿 命本就低的Norflash更添了几分压力。目前的做法是每一次写入都会带来一次日志写开销，这似乎是难以容忍的。我们希望采用一种Sync机制，即我们可以在特定的检查点将日志一并刷入Norflash介质，从而节省写入日志开销，当然，这涉及到对缓存层的修改问题；另外，如何验证及评估日志系统的优势，这还需要进 一步的考量；

- **Multi Tread Implement**

  现在HoitFS的读写I/O均采用单线程机制，我们相信多线程读写将加速读写性能； 事实上，多线程GC也将一并加速垃圾回收效率；

- **Build Boost**

  HoitFS在 Build的时候将扫描整个Norflash，这导致Build速率会较慢，可以通过 设置额外的Summary块来解决这个问题（EBS）；

- **Scalability**

  目前的HoitFS是针对小型Norflash设计，在Scalability上还欠火候。如何解决这一方案呢？介质上索引 + Mountable Tree给了我们很不错的思考。相关工作还未开 始，这将留以后面设计探讨；

- **Efficiency**

  在性能评估方面，HoitFS还未展开很好的工作。目前正准备移植更多的Norflash文件系统至SylixOS，以便进行性能评估；

## Where to see more details?

Here is our [repository](https://github.com/Hoit-23o2/HoitOS.git) in github. If you like it, please give us a star!😄

## Project Detail

To learn more about our project, visit [here](https://github.com/oscomp/proj32-NorFlash-FileSystem-on-SylixOS)! 

## File Organization

```
|-- Bin -- MINI2440测试程序
|   |-- 2440test_A70.bin
|   `-- u-boot.bin
|-- LICENSE
|-- README.md -- README
|-- README_BAKUP.md -- 主仓库README副本
|-- README_HOITFS.md -- 主仓库README
|-- Records
|   |-- CCB -- 会议纪要
|   |   |-- 2021-01-12.md -- 2021年1月12日纪要
|   |   |-- 2021-01-14.md -- 2021年1月14日纪要
|   |   |-- 2021-01-24.md -- 2021年1月24日纪要
|   |   |-- 2021-01-31.md -- 2021年1月31日纪要
|   |   |-- 2021-02-07.md -- 2021年2月07日纪要
|   |   |-- 2021-02-23.md -- 2021年2月23日纪要
|   |   |-- 2021-03-01.md -- 2021年3月01日纪要
|   |   |-- 2021-03-08.md -- 2021年3月08日纪要
|   |   |-- 2021-03-15.md -- 2021年3月15日纪要
|   |   |-- 2021-03-23.md -- 2021年3月23日纪要
|   |   |-- 2021-03-29.md -- 2021年3月29日纪要
|   |   `-- README.md
|   |-- Docs -- 文档
|   |   |-- 00-Prepare.md
|   |   |-- 01-SettleDown.md
|   |   |-- 02-PreResearch-Part1.md
|   |   |-- 02-PreResearch-Part2.md
|   |   |-- 03-DeepResearch-Part1.md
|   |   |-- 03-DeepResearch-Part2.md
|   |   |-- 03-DeepResearch-Part3.md
|   |   |-- 04-SylixOSResearch-Part1.md
|   |   |-- 04-SylixOSResearch-Part2.md
|   |   |-- 04-SylixOSResearch-Part3.md
|   |   |-- 05-SylixOSResearch-Part1.md
|   |   |-- 05-SylixOSResearch-Part2.md
|   |   |-- 05-SylixOSResearch-Part3.md
|   |   |-- 06-SylixOSPractice-Part1.md
|   |   |-- 06-SylixOSPractice-Part2.md
|   |   |-- 06-SylixOSPractice-Part3.md
|   |   |-- 07-SylixOS&Driver&FS-Part1.md
|   |   |-- 07-SylixOS&Driver&FS-Part2.md
|   |   |-- 07-SylixOS&Driver&FS-Part3.md
|   |   |-- 08-SylixOS&Driver&FS-Continue-Part1.assets
|   |   |-- 08-SylixOS&Driver&FS-Continue-Part1.md
|   |   |-- 08-SylixOS&Driver&FS-Continue-Part2.md
|   |   |-- 09-SylixOS&Driver&FS-Again-Part1.md
|   |   |-- 09-SylixOS&Driver&FS-Again-Part2.md
|   |   |-- 09-SylixOS&Driver&FS-Again-Part3.md
|   |   |-- 10-HoitFsDev-Part1.md
|   |   |-- 10-HoitFsDev-Part3.md
|   |   |-- 11-HoitFsDev-CacheTest-Part3.md
|   |   |-- 11-HoitFsDev-Stage2-Part1.md
|   |   |-- README.md
|   |   `-- images
|   |-- Files -- 相关参考文档
|   `-- Trans -- 相关翻译工作
|-- SPECIFICATION.md -- 代码风格说明
|-- Src -- 前期开发工作
|-- SylixOS -- BSP文件
|   `-- Workspace -- 工作目录
|       `-- SylixBSP -- 源文件
|           |-- bsp -- bsp文件夹
|           |   |-- 2440addr.h -- 2440声明
|           |   |-- bspInit.c -- 硬件初始化
|           |   |-- bspLib.c -- 相关库
|           |   |-- bspMap.h -- 存储映射
|           |   |-- config.h -- 配置
|           |   |-- startup.S -- 引导程序
|           |   |-- symbol.c -- 符号表
|           |   |-- symbol.h -- 符号表
|           |   |-- targetInit.c -- 初始化
|           |   `-- targetInit.h -- 初始化
|           |-- driver
|           |   |-- README
|           |   |-- dma -- dma驱动
|           |   |-- gpio -- gpio驱动
|           |   |-- i2c -- i2c驱动
|           |   |-- lcd -- lcd驱动
|           |   |-- mtd -- mtd文件
|           |   |   |-- nand -- nand驱动
|           |   |   `-- nor -- nor驱动
|           |   |       |-- Am29LV160DB.h -- Am29LV160DB相关参数 
|           |   |       |-- fake_nor.c -- 虚拟Nor驱动
|           |   |       |-- fake_nor.h -- 虚拟Nor驱动
|           |   |       |-- nor.c -- 真实Nor驱动
|           |   |       |-- nor.h -- 真实Nor驱动
|           |   |       |-- nor_cmd.c -- 命令行实现
|           |   |       |-- nor_cmd.h -- 命令行实现
|           |   |       |-- nor_util.c -- 相关工具
|           |   |       `-- nor_util.h -- 相关工具
|           |   |-- netif -- netif驱动
|           |   |-- pm -- pm驱动
|           |   |-- rtc -- rtc驱动
|           |   |-- sdi -- sdi驱动
|           |   |-- timer -- timer驱动
|           |   |-- touchscr -- touchscr驱动
|           |   `-- tty -- tty驱动
|           |-- extfs -- 拓展文件系统
|           |   |-- hoitFs -- HoitFS文件系统
|           |   |   |-- hoitFs.c -- 上层接口
|           |   |   |-- hoitFs.h
|           |   |   |-- hoitFsCache.c -- 缓存层
|           |   |   |-- hoitFsCache.h
|           |   |   |-- hoitFsCmd.c -- 命令行功能额
|           |   |   |-- hoitFsCmd.h
|           |   |   |-- hoitFsFDLib.c -- FullDNode管理层
|           |   |   |-- hoitFsFDLib.h
|           |   |   |-- hoitFsGC.c -- GC
|           |   |   |-- hoitFsGC.h
|           |   |   |-- hoitFsLib.c -- 逻辑层
|           |   |   |-- hoitFsLib.h
|           |   |   |-- hoitFsLog.c -- 日志层
|           |   |   |-- hoitFsLog.h
|           |   |   |-- hoitFsMid.c -- 拓展层，还未予以是心啊
|           |   |   |-- hoitFsMid.h
|           |   |   |-- hoitFsTest.c -- 测试脚本
|           |   |   |-- hoitFsTest.h
|           |   |   |-- hoitFsTree.c -- FragTree管理层
|           |   |   |-- hoitFsTree.h
|           |   |   |-- hoitFsTreeUtil.c -- 红黑树结构工具
|           |   |   |-- hoitFsTreeUtil.h
|           |   |   `-- hoitType.h -- HoitFS类型声明
|           |   `-- spifFs -- spiffs留待以后移植，评估
|           `-- user -- 用户程序
|               `-- main.c
|-- Tools -- 工具
|   |-- Mini2440Tools.zip -- mini2440工具包
|   |-- auto_complete.py -- Eclipse自动补全工具（已废弃）
|   |-- readme_title_generator.py -- 自动生成README标题工具
|   `-- winhex -- WINHEX工具
|-- UpdateREADME.bat -- 自动生成README标题工具
|-- images -- 图片
|-- tree.exe -- 文件树生成程序
`-- HoitFS设计开发文档.pdf
```

