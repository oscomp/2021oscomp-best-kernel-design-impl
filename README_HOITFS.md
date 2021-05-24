# HoitOS

> 这里是项目简介

This is the repository of 2021 OS comp， 项目进度安排移步至[Worktile](https://vwpp20210125060242436.worktile.com/mission/projects/600e62ffd6e5f843a97f2182)

![worktile](./images/worktile.png)

## Log

> 这里记录了所有输出文档

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
### Week11
> 本周总结：[第n阶段-内容(yyyy-mm-dd)](./Records/Files/PPT/xxxx.pptx)

[11-HoitFsDev-Stage2-Part1](./Records/Docs/11-HoitFsDev-Stage2-Part1.md)

在这里填写相关内容



## ConfigurationControlBoard(CCB)

> 这里记录了会议纪要

1. [2021-01-12](./Records/CCB/2021-01-12.md)
2. [2021-01-14](./Records/CCB/2021-01-14.md)
3. [2021-01-24](./Records/CCB/2021-01-24.md)
4. [2021-01-31](./Records/CCB/2021-01-31.md)
5. [2021-02-07](./Records/CCB/2021-02-07.md)
6. [2021-02-23](./Records/CCB/2021-02-23.md)
7. [2021-03-01](./Records/CCB/2021-03-01.md)
8. [2021-03-08](./Records/CCB/2021-03-08.md)
9. [2021-03-15](./Records/CCB/2021-03-15.md)

10. [2021-03-23](./Records/CCB/2021-03-23.md)
11. [2021-03-29](./Records/CCB/2021-03-29.md)

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