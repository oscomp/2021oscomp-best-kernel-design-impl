# UltraOS

拥有友好代码和详细文档的Rust编写的基于RISC-V64的多核操作系统UltraOS，支持qemu和k210平台运行。

#### 运行
根目录下Makefile提供了两个命令。

> make all

该命令将生成完整的二进制内核文件，可烧录至k210开发板上。

> make run

该命令将直接在qemu上运行UltraOS。

#### 联系我们

[loancold@qq.com](mailto:loancold@qq.com)

#### 项目人员

来自哈尔滨工业大学（深圳）

李程浩（队长）：主要负责用户程序支持，开发和测试环境搭建，多核支持。

宫浩辰：主要负责文件系统，多核支持。

任翔宇：主要负责内存管理和trap。

#### 项目特征

- Rust语言
- 多核操作系统
- 支持C语言程序和Rust语言程序运行
- FAT32与EXT2-like文件系统
- 详细项目文档
- 双文件块缓存优化机制

#### 文件包阅读说明


- doc：项目文档
- codes：开发代码
  - bootloader：使用开发者洛佳的K210和qemu版本的RustSBI二进制文件（本项目对源码进行了改动，生成的二进制文件不为官方版本）
  - dependency：本地库依赖
  - fat32-fuse：构建fat32文件系统
  - os：kernel代码
  - riscv-syscalls-testing：官方评测程序
  - simple_fat32：fat32文件系统，隶属于kernel
  - user：Rust用户程序代码

