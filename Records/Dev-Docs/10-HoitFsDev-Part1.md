# 2021-03-23 ~ 2021-03-30 HoitFs 开发

> 本节主要介绍HoitFS开发设想，未来展望，以及一些实现方面的参考
>
> 参考资料：
>
> 1. [Jffs2源码](https://elixir.bootlin.com/linux/latest/source/fs/jffs2/write.c#L59)

## 项目开发策略概述

HoitFs将首先以Jffs2为原型进行设计，我们采用**Layer Model**进行建模，保证开发的高效进行，如下：

![image-20210328131456435](./images/hoit-fs-layer.png)



## 未来规划

接下来1个月左右的项目规划：

- 3-28 ~ 4.4：以Jffs2为原型，在开发过程中确立基本框架、流程；

- 4.4 ~ 4.11： 读、写、Seek、Mkdir等基本功能实现；

- 4.11 ~ 4.18：GC、写平衡优化；

- 4.18 ~ 4.25：Jffs2 掉电平衡，Build设计； （**里程碑**）

- 4.25 ~ 5.02：阅读UBIFS，阅读更多NorFlash FS设计，获取创新设计灵感；

- 5.02 ~ 5.09：针对寻找的文献，改进Jffs2，诞生V0.2；

- 5.09 ~ 5.16：评估V0.2与V0.1的特性；

- ……

## 导师的建议

1. fio工具，可用于测试；
2. 深挖**nand与nor**的区别；
3. 扣读、写、掉电安全效率；
4. 一些比较老的结构可以做一些文章；



## Jffs2 Code Thru

在正式开工之前，还是需要进行一些准备工作，需要充分借鉴前人的工作，我打算进行I/O管理层的开发，也就是组织**红黑树**来实现File类型的Inode的管理，我们可以看一下Jffs2是如何实现`write`的。

### jffs2_write_dnode

根据注释，我们不难发现，JFFS2在写入Flash之前，会先为其分配一个full_dnode，然后再写入，之后连接到**inode或fragment链表中**。根据[JFFS2文件写入流程](./07-SylixOS&Driver&FS-Part2.md)介绍的那样，写入后，会调用`jffs2_add_full_dnode_to_inode`函数将写入的数据实体添加至**红黑树**中，于是，接下来的工作便是去查看`jffs2_add_full_dnode_to_inode`函数。

