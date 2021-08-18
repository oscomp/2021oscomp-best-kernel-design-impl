![QQ图片20210531114657](./README.assets/logo.png)

# HOIT-FS

We are team "HOIT-23o2" from **Harbin institute of technology, Shenzhen**. We are now developing on proj32-Norflash-Filesystem-on-SylixOS. Keep tuning in.👏

## Docs

- This is the **main doc** for **final stage** submit: [决赛文档-设计、实现与测试](./HoitFS.pdf) 
- This is the **main doc** for **stage 1** submit: [初赛文档-设计开发](./HoitFS设计开发文档.pdf)
- This is the **link** of development doc for stage 1 submit: [README_HOITFS](./README_HOITFS.md)
- This is the **link** of all the reference files: [Reference](./Records/Reference)
- This is the **link** of development records for design and some works: [Dev-Docs](./Records/Dev-Docs)
- This is the **link** of our video description: [初赛HoitFS视频演示](./HoitFS介绍.mp4)
- This is the **link** of our demo video: [决赛HoitFS项目Demo](./HoitFS项目Demo.webm)
- **Note:** [repository](https://github.com/Hoit-23o2/HoitOS.git) in github maintains the **newest** repo, this repo is a replica of that, only for submit.

## Goals & Achievement

The goals we must achieve:

1. Basic read, write, seek, mkdir, mv and so on.
2. Hard and soft link, write balance.
3. How to make sure the lowest cost of power loss.

We have achieved that:  

| 目标编号 | 基本完成情况 | 额外说明                                                     |
| :------: | :----------: | ------------------------------------------------------------ |
|    1     |    已完成    | ① **基本 I/O 功能**已通过脚本测试；                          |
|    2     |    已完成    | ① **软、硬链接**已通过脚本测试；<br/>② **多链表随机**Wear Leveling设计（承接自JFFS2） |
|    3     |    已完成    | ① **CRC**校验实现<br/>② **EBS**校验实现；                    |
|   总计   |  完成度100%  | 基本题目要求均已完成                                         |

Except above, we also achieved that: 

**1.Stability**

通过功能与性能测试，文件系统趋于稳定；

<br>

**2.Spiffs Capability**

除了开发**HoitFS**，我们还完成了**SpifFS**的移植，使得我们能够进行**性能评测**；

<br>

**3.EBS + MT**

我们完成了**EBS**与**多线程**（MT）挂载，使得挂载速率相比未采用该机制有了显著的提升；

<br>

**4.Background GC**

我们加入了**后台GC**机制，使得我们在写入相同大小文件的情况下，能够相对减少**前台GC**次数，从而提升写效率；

<br>

**5.Mergeable Tree**

为了解决小数据写入带来的内存爆炸性增长，我们设计并实现了**Mergeable Tree**，并能够大大缓解小数据写入带来的内存开销；

<br>

## Where to see more details?

Here is our [repository](https://github.com/Hoit-23o2/HoitOS.git) in github. If you like it, please give us a star!😄

## Project Detail

To learn more about our project, visit [here](https://github.com/oscomp/proj32-NorFlash-FileSystem-on-SylixOS)! 

## File Organization

```
|-- Bin                                       -- MINI2440测试程序
|   |-- 2440test_A70.bin
|   `-- u-boot.bin
|-- LICENSE
|-- README.md                                 -- README
|-- README_BAKUP.md                           -- 主仓库README副本
|-- README_HOITFS.md                          -- 主仓库README
|-- Records
|   |-- Meeting                               -- 会议纪要
|   |   |-- 2021-01-12.md                     -- 2021年1月12日纪要
|   |   |-- 2021-01-14.md                     -- 2021年1月14日纪要
|   |   |-- 2021-01-24.md                     -- 2021年1月24日纪要
|   |   |-- 2021-01-31.md                     -- 2021年1月31日纪要
|   |   |-- 2021-02-07.md                     -- 2021年2月07日纪要
|   |   |-- 2021-02-23.md                     -- 2021年2月23日纪要
|   |   |-- 2021-03-01.md                     -- 2021年3月01日纪要
|   |   |-- 2021-03-08.md                     -- 2021年3月08日纪要
|   |   |-- 2021-03-15.md                     -- 2021年3月15日纪要
|   |   |-- 2021-03-23.md                     -- 2021年3月23日纪要
|   |   |-- 2021-03-29.md                     -- 2021年3月29日纪要
|   |   |-- 2021-05-18.md                     -- 2021年5月18日纪要
|   |   |-- 2021-05-31.md                     -- 2021年5月31日纪要
|   |   `-- README.md
|   |-- Dev-Docs                              -- 开发文档
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
|   |   |-- 12-HoitFsEval-Stage1-Part1.md
|   |   |-- README.md
|   |   `-- images
|   |-- Reference                             -- 相关参考文档
|-- SPECIFICATION.md                          -- 代码风格说明
|-- Src                                       -- 主要贡献代码
|   |-- Am29LV160DB-nor-driver                -- Am29LV160DB驱动
|   |-- extfs                                 -- 核心代码：HOITFS、SPIFFS以及FSTESTER工具、对象链表实现等
|   |-- fake-nor                              -- SST39VF1601模拟驱动，前期开发编写，已弃用
|   |-- lFs                                   -- 简单的LFS源码，参考Berkeley LFS实验在SylixOS上予以实现，一个很好的Demo
|   |-- spiffs-master                         -- SPIFFS移植参考代码
|   |-- test-painter                          -- Python自动解析绘图工具，已弃用
|   |-- test-res                              -- HOITFS项目所有测试结果
|   `-- README.md                             -- README
|-- SylixOS -- BSP文件
|   `-- Workspace -- 工作目录
|       `-- SylixBSP -- 源文件
|           |-- bsp -- bsp文件夹
|           |   |-- 2440addr.h                -- 2440声明
|           |   |-- bspInit.c                 -- 硬件初始化
|           |   |-- bspLib.c                  -- 相关库
|           |   |-- bspMap.h                  -- 存储映射
|           |   |-- config.h                  -- 配置
|           |   |-- startup.S                 -- 引导程序
|           |   |-- symbol.c                  -- 符号表
|           |   |-- symbol.h                  -- 符号表
|           |   |-- targetInit.c              -- 初始化
|           |   `-- targetInit.h              -- 初始化
|           |-- driver
|           |   |-- README
|           |   |-- dma                       -- dma驱动
|           |   |-- gpio                      -- gpio驱动
|           |   |-- i2c                       -- i2c驱动
|           |   |-- lcd                       -- lcd驱动
|           |   |-- mtd                       -- mtd文件
|           |   |   |-- nand                  -- nand驱动
|           |   |   `-- nor                   -- nor驱动
|           |   |       |-- Am29LV160DB.h     -- Am29LV160DB相关参数 
|           |   |       |-- fake_nor.c        -- 虚拟Nor驱动
|           |   |       |-- fake_nor.h        -- 虚拟Nor驱动
|           |   |       |-- nor.c             -- 真实Nor驱动
|           |   |       |-- nor.h             -- 真实Nor驱动
|           |   |       |-- nor_cmd.c         -- 命令行实现
|           |   |       |-- nor_cmd.h         -- 命令行实现
|           |   |       |-- nor_util.c        -- 相关工具
|           |   |       `-- nor_util.h        -- 相关工具
|           |   |-- netif                     -- netif驱动
|           |   |-- pm                        -- pm驱动
|           |   |-- rtc                       -- rtc驱动
|           |   |-- sdi                       -- sdi驱动
|           |   |-- timer                     -- timer驱动
|           |   |-- touchscr                  -- touchscr驱动
|           |   `-- tty                       -- tty驱动
|           |-- extfs -- 拓展文件系统
|           |   |-- hoitFs -- HoitFS文件系统
|           |   |   |-- hoitFs.c              -- 上层接口
|           |   |   |-- hoitFs.h
|           |   |   |-- hoitFsCache.c         -- 缓存层
|           |   |   |-- hoitFsCache.h
|           |   |   |-- hoitFsCmd.c           -- 命令行功能额
|           |   |   |-- hoitFsCmd.h
|           |   |   |-- hoitFsFDLib.c         -- FullDNode管理层
|           |   |   |-- hoitFsFDLib.h
|           |   |   |-- hoitFsGC.c            -- GC
|           |   |   |-- hoitFsGC.h
|           |   |   |-- hoitFsLib.c           -- 逻辑层
|           |   |   |-- hoitFsLib.h
|           |   |   |-- hoitFsLog.c           -- 日志层，已弃用
|           |   |   |-- hoitFsLog.h
|           |   |   |-- hoitFsTest.c          -- 测试脚本
|           |   |   |-- hoitFsTest.h
|           |   |   |-- hoitFsTree.c          -- FragTree管理层
|           |   |   |-- hoitFsTree.h
|           |   |   |-- hoitFsTreeUtil.c      -- 红黑树结构工具
|           |   |   |-- hoitFsTreeUtil.h
|           |   |   |-- hoitFsMergeBuffer.c   -- MergeBuffer实现
|           |   |   |-- hoitFsMergeBuffer.h
|           |   |   `-- hoitType.h            -- HoitFS类型声明
|           |   |-- spifFs 
|           |   |   |-- spifFs.c              -- spiffs上层接口
|           |   |   |-- spifFs.h
|           |   |   |-- spifFsCache.c         -- spiffs缓存层
|           |   |   |-- spifFsCache.h 
|           |   |   |-- spifFsCmd.c           -- spiffs命令接口
|           |   |   |-- spifFsCmd.h
|           |   |   |-- spifFsConfig.h        -- spiffs配置
|           |   |   |-- spifFsFDLib.c         -- spiffs文件描述结构管理
|           |   |   |-- spifFsFDLib.h     
|           |   |   |-- spifFsGC.c            -- spiffs GC
|           |   |   |-- spifFsGC.h
|           |   |   |-- spifFsGlue.c          -- spiffs胶水层，用于衔接SylixOS
|           |   |   |-- spifFsGlue.h
|           |   |   |-- spifFsLib.c           -- spiffs逻辑层
|           |   |   |-- spifFsLib.h
|           |   |   |-- spifFsType.h          -- spiffs类型定义
|           |   |   `-- spifFsVerify.c        -- spiffs验证，暂未实现
|           |   |-- tools
|           |       |-- crc                   -- crc模板工具
|           |       |-- fstester              -- norflash文件系统测试工具
|           |       |-- hash                  -- hash模板工具
|           |       `-- list                  -- 对象链表实现
|           `-- user -- 用户程序
|               `-- main.c
|-- Tools -- 工具
|   |-- Mini2440Tools.zip                     -- mini2440工具包
|   |-- auto_complete.py                      -- Eclipse自动补全工具（已废弃）
|   |-- readme_title_generator.py             -- 自动生成README标题工具
|   `-- winhex                                -- WINHEX工具
|-- UpdateREADME.bat                          -- 自动生成README标题工具
|-- images                                    -- 图片
|-- tree.exe                                  -- 文件树生成程序
`-- HoitFS设计开发文档.pdf
```

