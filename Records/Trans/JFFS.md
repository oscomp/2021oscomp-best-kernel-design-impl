# JFFS: The Journaling Flash File System

> JFFS：Flash文件系统的旅程
>
> [翻译自](https://sourceware.org/jffs2/jffs2.pdf)

[TOC]



## Abstract

Until recently, the common approach to using Flash memory technology in embedded devices has been to use a pseudo-filesystem on the flash chips to emulate a standard block device and provide wear levelling, and to use a normal file system on top of that emulated block device. JFFS is a log-structured file system designed by Axis Communications AB in Sweden specifically for use on flash devices in embedded systems, which is aware of the restrictions imposed by flash technology and which operates directly on the flash chips, thereby avoiding the inefficiency of having two journalling file systems on top of each other. This paper will give an overview of the restrictions imposed by flash technology and hence the design aims of JFFS, and the implementation of both JFFS and the improvements made in version 2, including compression and more efficient garbage collection.

> 截止至今，将Flash Memory技术用于**嵌入式** 设备的最常用的方法是在Flash芯片上使用一个伪文件系统来模拟标准块设备、提供损耗平衡（Wearing Level）以及在该模拟块设备之上使用普通文件系统。JFFS是一个日志结构的文件系统，最初由Sweden的Axis Communications AB设计，主要用于嵌入式系统的闪存（Flash）设备中。JFFS关注Flash技术带来的限制，并且可以直接在Flash芯片上运行，从而避免了两个日志文件系统相互重叠的低效率。本文将给出一个概述：Flash技术的局限性以及JFFS设计目的，然后，我们还会给出JFFS的实现方法以及JFFS2的进步之处，包括但不限于压缩存储以及更高效的垃圾回收机制。



## 1. Introduction

### 1.1 Flash

Flash memory is an increasingly common storage medium in embedded devices, because it provides solid state storage with high reliability and high density, at a relatively low cost.

> 闪存被越来越广泛地应用于嵌入式设备中，因为它提供固态存储技术，在价格低的同时具有较高的可靠性、高密度。

Flash is a form of Electrically Erasable Read Only Memory (EEPROM), available in two major types — the traditional NOR flash which is directly accessible, and the newer, cheaper NAND flash which is addressable only through a single 8-bit bus used for both data and addresses, with separate control lines. These types of flash share their most important characteristics — each bit in a clean flash chip will be set to a logical one, and can be set to zero by a write operation.

> Flash是EEPROM（电可擦写ROM）的一种形式，主要有两种类型：传统的NOR flash， 它是直接存取，以及更新、更便宜的NAND flash，它只通过既是数据总线亦是地址总线的8位总线外加单独的控制线寻址。这些来性的Flash有一个非常重要的共性：在干净（Clean，大概就是什么也没有装）的Flash芯片中所有比特位都被置为1，写操作可以将这些比特位置为0。

Flash chips are arranged into blocks which are typically 128KiB on NOR flash and 8KiB on NAND flash. Resetting bits from zero to one cannot be done individually, but only by resetting (or “erasing”) a complete block. The lifetime of a flash chip is measured in such erase cycles, with the typical lifetime being 100,000 erases per block. To ensure that no one erase block reaches this limit before the rest of the chip, most users of flash chips attempt to ensure that erase cycles are evenly distributed around the flash; a process known as “wear levelling”.

> Flash芯片被布局为许多块（Block），在NOR flash上，这些块大小通常为128KB，而在NAND flash上，它们通常为8KB。我们不能单独将比特位从0置为1，只能通过擦除整个块的方式才行，这是Flash的特点。Flash芯片的生命周期正是由擦除次数所决定，通常来讲，每个块都有100,000次擦除机会。为了确保在芯片退休前，块的擦除次数不会达到极限，许多Flash芯片用户尝试保证擦除均匀发生在Flash芯片中，这就是所谓的“损耗平衡（Wear Levelling）”

Aside from the difference in erase block sizes, NAND flash chips also have other differences from NOR chips. They are further divided into “pages” which are typically 512 bytes in size, each of which has an extra 16 bytes of “out of band” storage space, intended to be used for metadata or error correction codes. NAND flash is written by loading the required data into an internal buffer one byte at a time, then issuing a write command. While NOR flash allows bits to be cleared individually until there are none left to be cleared, NAND flash allows only ten such write cycles to each page before leakage causes the contents to become undefined until the next erase of the block in which the page resides.

> 除了擦除块大小不同外，NAND flash和NOR flash芯片还有其他不同之处。Nand Flash芯片被进一步地被划分为512B的页面，每个页面还有额外的16B作为“Out Of Band（OOB）”存储空间，目的用于存储元数据或者纠错码（ECC）。NAND flash的写操作是通过将所需的数据一次一个字节送入内部缓冲区，然后调用写命令完成。而NOR flash允许清理单个的比特，直到没有比特可被清理为止。NAND flash允许每个页面在漏电（leakage）导致内容丢失之前有十个这样的写周期。

### 1.2 Flash Translation Layers

> Personal Computer Memory Card International Association, 

