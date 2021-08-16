# 2021-01-12 Prepare

> 竞赛官网：https://os.educg.net/
>
> 功能赛道项目选择：https://github.com/oscomp



[TOC]

## 成员

- 潘延麒

- 胡智胜

- 张楠

  

## 赛题目标

题目要与OS Kernel实现相关，这样才能将本次竞赛与我们实验课上所学结合起来，并且能够进一步加深我们对OS的理解，这无论对于现在还是未来都是有价值的。对于那些只实现部分功能或是**只实现优化**的Project，我们将采取最低优先级考虑。**后面的内容不分先后**。



## [proj2-os-kernels-by-history](https://github.com/oscomp/proj2-os-kernels-by-history)

**1. 大概内容：**

基于以下OS拓展

- （基于Rust语言）：https://github.com/rcore-os/rCore-Tutorial-v3
- （基于C语言）：https://github.com/DeathWish5/appdir

包括但不限于：

- **支持虚存的换入换出功能（Log，xv6中有参考实现）**
- **支持多核（Log，xv6中有参考实现）**
- **支持日志文件系统（Log，xv6中有参考实现）**
- **支持网络**
- **体现OS各种功能、Bugs和跨硬件能力**：图形能力、架构支持能力、**教学设计步骤**、文档

**2. 平台：** Risc-V

**3. 虚拟硬件： ** QEMU

**4. 物理硬件： ** Kendryte K210



## [proj12-shell-enhancement-on-SylixOS](https://github.com/oscomp/proj12-shell-enhancement-on-SylixOS)

**1. 大概内容：**

SylixOS是什么？SylixOS是国内拿得出手的OS，全权由国人自己研发，本项目应该是与**翼辉公司**接轨的。如果完成，很有可能直接收到Offer。另外，作为企业级OS，完成本项目肯定能够让我们学到更多东西。

本项目主要希望学生帮助SylixOS完成一系列额外功能，包括但不限于

- 智能补全（我个人比较感兴趣）
- 管道、过滤和分屏显示机制

主要就是**Shell命令增强**了。



## [proj34-SylixOS-porting-to-XuanTie-C906](https://github.com/oscomp/proj34-SylixOS-porting-to-XuanTie-C906)

**1. 大概内容**

同样是SylixOS公司，从头在玄铁C906上搭建一个简易的SylixOS，比较向**内核赛道**靠近，个人觉得可拓展空间非常大。

需要完成但不限于：

- **完成 SylixOS 最小系统开发**
- **完成 SylixOS 的基础功能（网络、FS）**

**2. 平台**：玄铁C906（64位RISC-V（RV64GCV）1GHz处理器的全志开发板）



## **[proj32-NorFlash-FileSystem-on-SylixOS](https://github.com/oscomp/proj32-NorFlash-FileSystem-on-SylixOS)**

**1. 大概内容**

同样在SylixOS系统上实现一个NorFlash文件系统（个人觉得**有难度**）



## [proj31-Dual-OS-With-OpenAMP](https://github.com/oscomp/proj31-Dual-OS-With-OpenAMP)

**1. 大概内容**

开发 MS-RTOS和SylixOS ，并通过AMP通信。主要为异构操作系统通信

**2. 平台**

STM32MP157 微处理器基于灵活的双 ARM® Cortex®-A7 内核（运行频率为 650 MHz）和 Cortex®-M4（运行频率为 209 MHz）架构。



## [proj9-zcore](https://github.com/oscomp/proj9-zcore)

**1. 大概内容**

从头到尾实现一个Zcore，用Rust实现，有意思。未来OS内核。

**2. 平台**

- 支持 ARM64，并在树莓派4上运行
- 支持 RISC-V 64，并在 K210 或 SIFIVE 开发板上运行



## [proj23-lightweight-hypervisor](https://github.com/oscomp/proj23-lightweight-hypervisor)

**1. 大概内容**

实现一个轻量级的Hypervisor（虚拟机管理器）

有难度，需要深入理解RISC-V架构，包括S态、M态。需要软硬件的融会贯通。



## **[proj19-process-memory-tracker](https://github.com/oscomp/proj19-process-memory-tracker)**

**1. 大概内容**

进程内存泄露追踪，感觉比较简单，但涉及到的知识相对较多。如何监听呢？这涉及到一些底层知识。

> 内存泄漏（Memory Leak）是指程序中已动态分配的堆内存由于某种原因程序未释放或无法释放，造成系统内存的浪费，导致程序运行速度减慢甚至系统崩溃等严重后果。



## [proj15-prolog-in-c](https://github.com/oscomp/proj15-prolog-in-c)

**1. 大概内容**

这个为什么是**高等难度？？** 感觉与OS没啥相关点



## [proj0-experiments-4-os-course](https://github.com/oscomp/proj0-experiments-4-os-course)

**1. 大概内容**

课程设计。完成课程设计，我们需要攥写实验指导书，以及相应的代码，这对于我们自己来说帮助无疑也是非常大的。与此同时，我们还能够帮助下一届的学弟学妹。

[不错的参考（非常不错）](https://github.com/NKU-EmbeddedSystem/riscv64-ucore/blob/master/ucore%20tutorial%E7%BB%84%E6%9C%80%E7%BB%88%E6%8A%A5%E5%91%8A.md)



## [proj5-fuxi-full-system-with-cpu-compiler-os](https://github.com/oscomp/proj5-fuxi-full-system-with-cpu-compiler-os)

**1. 大概内容**

> 工作量有点大，前提只是需要有点多

实现全系统。主要工作

- 移植SoC

  > SoC 全称 “System On Chip‘

- 重写**羽语言**编译器

- 扩展GeeOS

**2. 需要知识**

- 羽语言熟悉
- Chisel3熟悉
- 伏羲CPU架构熟悉



## [proj4-xv6-rust](https://github.com/oscomp/proj4-xv6-rust)

**1. 大概内容**

用RUST重写xv6。主要内容为：

- 参考本项目也用Rust语言复现xv6-riscv的设计与实现

- 实现和扩展智能指针，比如Arc和Condvar

- 实现Buddy System Allocator来管理内存

- 移植`xv6-riscv`的[十几个实验](https://pdos.csail.mit.edu/6.828/2020/labs/)

- 支持在K210硬件上运行



## 总结

基本都与Rust相关，选定题目后，就应该先快速学习Rust了。

主要学习方式可以参考[从0开始基于Rust实现OS指导](https://github.com/rcore-os/rCore/wiki/os-tutorial-summer-of-code)



