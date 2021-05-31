# HoitOS

> 这里是项目简介

This is the repository of 2021 OS comp， 项目进度安排移步至[Worktile](https://vwpp20210125060242436.worktile.com/mission/projects/600e62ffd6e5f843a97f2182)

![worktile](README_HOITFS.assets/worktile.png)

## Log

> 这里记录了所有输出文档：
>
> `Week0 ~ Week8`均为调研阶段，这个阶段主要用于学习以及熟悉SylixOS；
>
> `Week9 ~ Week10`为开发准备阶段，这一阶段搭建好了自底向上的开发环境，HoitFS进入**第一个里程碑**；
>
> `Week11 ~ Week17`为开发阶段，这一阶段完成了大量编码工作，并在Week17时交付初稿，HoitFS进入**第二个里程碑**；
>
> 待续……

### Week0

[00-Prepare](./Records/Docs/00-Prepare.md)

主要挑选合适的项目， 并对各项目进行再评估；

### Week1

[01-SettleDown](./Records/Docs/01-SettleDown.md)

定夺项目Proj32，开始着手预研工作；

### Week2

> 本周总结：[第一阶段预研工作(2021-01-24)](./Records/Files/PPT/pre-research-stage2.pptx)

[02-PreResearch-Part1](./Records/Docs/02-PreResearch-Part1.md)

主要介绍Flash、基于Flash的文件系统：JFFS、JFFS2、YAFFS、SPIFFS等、SylixOS的IDE配置、TPSFS的掉电安全机制等；

[02-PreResearch-Part2](./Records/Docs/02-PreResearch-Part2.md)

与Part1类似，额外添加了Github上提供的一种**写平衡** 算法；

### Week3

> 本周总结：[第二阶段预研工作(2021-01-31)](./Records/Files/PPT/pre-research-stage2.pptx)

[03-DeepResearch-Part1](./Records/Docs/03-DeepResearch-Part1.md)

主要确定了Why Norflash、板子选型，研究了F2FS的基本结构以及其优势、研究了基于ramFs的SylixOS注册文件系统的一般方式、学习了关于**/mnt**、**/dev**目录的意义，以及驱动相关的概念，最后通过UCS的Lab7，深入理解了LFS的写操作、更新操作、GC操作等；

[03-DeepResearch-Part2](./Records/Docs/03-DeepResearch-Part2.md)

主要研究了MTD的结构、详细方法、NorFlash驱动模板框架等；

[03-DeepResearch-Part3](./Records/Docs/03-DeepResearch-Part3.md)

除了研究F2FS文件系统，还特别研究了SylixOS中Yaffs文件系统的装载工作；

- NAT
- GC
- Adaptive Logging
- Multi Log

### Week4

> 本周总结：[第三阶段-SylixOS研究(2021-02-14)](./Records/Files/PPT/sylixos-research-stage.pptx)

[04-SylixOSResearch-Part1](./Records/Docs/04-SylixOSResearch-Part1.md)

简要经历了SylixOS中内核模块的开发流程，研究了Linux中文件系统的注册与挂载，完成了内核模块的注册、卸载，完成了Linux内核开发的VScode配置；

[04-SylixOSResearch-Part2](./Records/Docs/04-SylixOSResearch-Part2.md)

主要研究了Linux中的文件系统以及文件系统的挂载，研究了SylixOS中ramFs的实现；

[04-SylixOSResearch-Part3](./Records/Docs/04-SylixOSResearch-Part3.md)

主要研究了SylixOS中Yaffs文件系统的驱动安装、系统挂载流程，至于Yaffs如何对设备进行读写操作还需进一步考察；

### Week5

> 本周总结：[第四阶段-SylixOS进阶研究(2021-02-23)](./Records/Files/PPT/sylixos-further-research-stage.pptx)

[05-SylixOSResearch-Part1](./Records/Docs/05-SylixOSResearch-Part1.md)

主要研究了SylixOS中的BSP，以及验证并确定开发流程，另外对mini2440虚拟机的norflash进行了一定的研究，无果；

[05-SylixOSResearch-Part2](./Records/Docs/05-SylixOSResearch-Part2.md)

主要研究了JFFS2的概貌，掌握几种数据结构的含义，这是JFFS2源码研究的第一部分；

[05-SylixOSResearch-Part3](./Records/Docs/05-SylixOSResearch-Part3.md)

主要研究了SylixOS中的Yaffs2与MTD设备交互过程，但是蒋老师说这部分对项目来说没什么太大的作用；

### Week6

> 本周总结：[第五阶段-基于SylixOS的实践研究(2021-03-02)](./Records/Files/PPT/sylixos-practice.pptx)

[06-SylixOSPractice-Part1](./Records/Docs/06-SylixOSPractice-Part1.md)

在SylixOS BSP中模拟SST39VF1601 norflash，提供了**接口**与**用例**，并提供了norflash坏块检测方法，研究了mini2440串口连接方法；

[06-SylixOSPractice-Part2](./Records/Docs/06-SylixOSPractice-Part2.md)

主要研究了JFFS2的文件、目录创建、写入操作，需要**再做总结**；

[06-SylixOSPractice-Part3](./Records/Docs/06-SylixOSPractice-Part3.md)

根据ramFs，在BSP项目中开发simpleFs，能够使SylixOS访问simpleFs的所有API，包括mount、mkdir、open等等，这里介绍修改内容，以及访问流程；

> 在Week7，一定要完成在mini2440上对Norflash进行访问控制，这将成为第一个里程碑；另外，需要验证norflash模拟器的可行性，等待；完成第一代文件系统设计初稿……加油啊！！！

### Week7

> 本周总结：[第六阶段-NorFlash驱动、基于SylixOS的文件系统设计 - 第一部分(2021-03-09)](./Records/Files/PPT/sylixos-driver-fs-stage1.pptx)

[07-SylixOS&Driver&FS-Part1](./Records/Docs/07-SylixOS&Driver&FS-Part1)

主要工作是在mini2440真实板子上烧录SylixOS，包括烧录至内存以及烧录至nandflash上，关于如何访问norflash，还在等待翼辉信息的回复中；

[07-SylixOS&Driver&FS-Part2](./Records/Docs/07-SylixOS&Driver&FS-Part1)

主要工作是研究透了JFFS2的工作方式，包括文件的系统的挂载、文件的创建、打开、关闭、删除等，还需进一步的总结；

[07-SylixOS&Driver&FS-Part3](./Records/07-SylixOS&Driver&FS-Part3.md)

对RamFS进行了深入了解，基于FAKE NOR完成LFS FS的基本create、mount操作；

### Week8

> 本周总结：[第七阶段-NorFlash驱动、基于SylixOS的文件系统设计 - 第二部分(2021-03-16)](./Records/Files/PPT/sylixos-driver-fs-stage2.pptx)

[08-SylixOS&Driver&FS-Continue-Part1](./Records/Docs/08-SylixOS&Driver&FS-Continue-Part1.md)

主要工作是能够通过NorFlash启动SylixOS，并编写了NorFlash驱动程序，能够对NorFlash进行读写擦除操作，提供API文档；

[08-SylixOS&Driver&FS-Continue-Part2](./Records/Docs/08-SylixOS&Driver&FS-Continue-Part2.md)

自行设计了基于JFFS2的文件系统，去掉了一些细节，总结了基本思想，基本可以开工，还需要进行修改，谨记F2FS的内容

- NAT
- GC
- Adaptive Logging
- Multi Log

### Week9 - 里程碑

> 本周总结：[第八阶段-NorFlash驱动、基于SylixOS的文件系统设计 - 第三部分(2021-03-23)](./Records/Files/PPT/sylixos-driver-fs-stage3.pptx)

[09-SylixOS&Driver&FS-Again-Part1](./Records/Docs/09-SylixOS&Driver&FS-Again-Part1.md)

完成最终NorFlash驱动，基于`Am29LV160DB` Nor Flash。集成**Fake Nor Flash**与**True Nor Flash**。本文主要介绍了**实现原理**；

[09-SylixOS&Driver&FS-Again-Part2](./Records/Docs/09-SylixOS&Driver&FS-Again-Part2.md)

搭建HoitFS基础框架，为HoitFS V0.1打下基础；

[09-SylixOS&Driver&FS-Again-Part3](./Records/Docs/09-SylixOS&Driver&FS-Again-Part3.md)

基本完成基于Fake Nor的LFS FS，包括了mount、unmount、ls等，**打通SylixOS**构建文件系统的通路；

### Week10

> 本周总结：[第九阶段-HoitFS正式开发-阶段一(2021-03-29)](./Records/Files/PPT/hoitfs-dev-stage1.pptx)

[10-HoitFsDev-Part1](./Records/Docs/10-HoitFsDev-Part1.md)

设计HoitFS基本层次；制定里程碑计划；根据SylixOS报告与夏老师的建议，主要需要在开发过程中深挖nand与nor flash的区别，从而对症下药，其次，对于掉电安全部分还需要进行相关调研（主要是**写、修改、删除**）部分；
[10-HoitFsDev-Part3](./Records/Docs/10-HoitFsDev-Part3.md)

完成LFS的read、write、seek、rename等基本功能，彻底打通文件系统与`SylixOS`的I/O交互流程；

### Week11~17 - 里程碑

> 本周总结：第11~17周处于开发阶段，文档以及会议工作落了下来，由于学校事情也比较多，因此开发时间不是很够……不过好在交出了初稿
>
> HoitFS V1.0：[初赛报告文档.pdf](./HoitFS设计开发文档.pdf)
>
> HoitFS 答辩：[初赛PPT](./Records/Files/PPT/hoitfs-swb-final-presentation.pptx)

[11-HoitFsDev-Stage2-Part1](./Records/Docs/11-HoitFsDev-Stage2-Part1.md)

完成HoitFS FragTree API接口文档；总结GC方法；总结测试方法等；然后便是记录了一些联调坑点；

[11-HoitFsDev-CacheTest-Part3](./Records/Docs/11-HoitFsDev-CacheTest-Part3.md)

完成测试脚本；



## ConfigurationControlBoard(CCB)

> 这里记录了会议纪要

1. [2021-01-12](./Records/CCB/2021-01-12.md)：分析OS竞赛各赛题内容以及难度评估，并进行投票确定题目。
2. [2021-01-14](./Records/CCB/2021-01-14.md)：进入第一阶段调研任务，**确定调研方向**（熟悉开发套件、赛题平台SylixOS以及相关文件系统和算法），统一文档范式。
3. [2021-01-24](./Records/CCB/2021-01-24.md)：展示初步调研成果，包括Norflash各种**常用文件系统**、**MTD**简介和Dummy block原理。确定采用**日志结构**文件系统。
4. [2021-01-31](./Records/CCB/2021-01-31.md)：在2021-01-24会议的基础上进一步调研，包括**SylixOS开发平台**实验方法，**JFFS2**原理和相关LInux源码分析，以及MTD核心与使用方法。并且完成第一次与赛题组老师的沟通。
5. [2021-02-07](./Records/CCB/2021-02-07.md)：针对文件系统与操作系统**交互方式**进行进一步调研。一方面调查Linux虚拟文件系统结构以及文件系统挂载方式，另一方面调查SylixOS中Yaffs2和Ramfs的挂载与运作。此外还在Linux上实现简单的日志文件系统。
6. [2021-02-23](./Records/CCB/2021-02-23.md)：进入第二阶段实践任务，使用赛题组提供的开发工具，尝试进行**BSP开发**。另外，继续进行文件系统调研工作，进一步调研了**Jffs2挂载**流程，以及Yaffs2与MTD的交互方式。
7. [2021-03-01](./Records/CCB/2021-03-01.md)：完成JFFS2调研工作，理清JFF2的**基本读写**实现以及**垃圾回收**机制。此外，还实现了**norflash模拟驱动**程序以及相应的API文档。开始移植**简易LFS**到SylixOS的工作。
8. [2021-03-08](./Records/CCB/2021-03-08.md)：打通SylixOS系统**烧录**到mini 2440开发版的流程，完成相应文档。完成JFFS2文件系统基本功能介绍文档。
9. [2021-03-15](./Records/CCB/2021-03-15.md)：实现对mini 2440 的**nor flash** 的使用，能够通过nor flash启动 SylixOS。初步设计**HoitFs系统框架**。理解文件系统在**SylixOS上的挂载流程**。
10. [2021-03-23](./Records/CCB/2021-03-23.md)：根据初步设计**HoitFs系统框架**，编写框架代码。完成LFS在SylixOS中的挂载与卸载。
11. [2021-03-29](./Records/CCB/2021-03-29.md)：完成HoitFs**红黑树**模块代码。完成HoitFs系统框架部分代码。完成**LFS**到SylixOS上的移植。
12. [2021-05-18](./Records/CCB/2021-05-18.md)：在这次会议开始之前，已经完成了HoitFs基本功能的开发。本次会议主要针对开发中尚未解决的部分Bug以及文件系统功能完善方面，讨论未来**工作目标**和规划未来**工作流程**。

## Basic Develop Method

> 这里记录了利用Git开发的基本流程

**1. Get Start**

```shell
# clone project
git clone https://github.com/Hoit-23o2/HoitOS.git
```

**2. Basic Development**

```shell
# pull project
git pull origin main

...

# add some modification
git add .
# commit
git commit -m "一些描述"
# REMEMBER MUST "pull" before "push"
git pull origin main
# push
git push origin main
```

## Specification

**代码规范**请参考[这里](./SPECIFICATION.md)。