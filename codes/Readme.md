### 文件包阅读说明


- bootloader：使用开发者洛佳的K210和qemu版本的RustSBI二进制文件（本项目对源码进行了动，生成的二进制文件不为官方版本）
- dependency：本地库依赖
- fat32-fuse：构建fat32文件系统
- os：kernel代码（核心）
- riscv-syscalls-testing：官方评测程序
- simple_fat32：fat32文件系统，隶属于kernel
- user：用户程序相关