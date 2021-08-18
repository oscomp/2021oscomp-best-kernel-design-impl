# 使用教程

克隆仓库：

```sh
git clone https://github.com/Gallium70/rv-n-ext-impl.git
cd rv-n-ext-impl
```

本仓库中包含多个子模组，不同的子模组之间并不一定能够组合出合理的搭配，故**不建议**直接使用 `git clone --recurse-submodules` 或是 `git clone --recurse-submodules` 一次性初始化所有的子模组，而是分别使用 `git submodule update --init xxx` 进行初始化。

## 环境配置

主要开发环境为 Ubuntu 20.04 (WSL) ，其中一些使用 Rust 编写的项目在 Windows 下也可编译，Rust 环境配置可参考 [rCore-Tutorial](https://rcore-os.github.io/rCore-Tutorial-Book-v3/chapter0/5setup-devel-env.html#rust)。
QEMU 的环境配置见 [Building QEMU for Linux](https://wiki.qemu.org/Hosts/Linux#Building_QEMU_for_Linux)（我们推荐的编译路径和选项与此文中有所不同）；此外 QEMU 还提供了 [Windows 环境下的编译指南](https://wiki.qemu.org/Hosts/W32#Building_QEMU_for_W64) ，但我们没有测试过。

## 在 QEMU 上运行 rCore-N

### QEMU

```sh
git submodule update --init qemu
cd qemu-build
../qemu/configure --target-list="riscv64-softmmu"
make -j
```

我们选择在 qemu 文件夹之外（而非在其中创建子文件夹）进行编译是为了避免编译产生的大量中间文件被 git 追踪。
编译产生的可执行文件为 qemu-build/riscv64-softmmu/qemu-system-riscv64 。

调试 PLIC 可修改 [sifive_plic.c](../../qemu/hw/riscv/sifive_plic.c) 中 `RISCV_DEBUG_PLIC` 宏的值为 1，然后在 qemu-build 目录中重新使用 `make -j` 编译。

### rCore-N

justfile 中默认将 QEMU 的三个串口映射到了 /dev/pts/24、/dev/pts/25 和 /dev/pts/31，可以在 Linux 中新打开两个终端，执行 `tty` 指令查看对应的设备路径，替换掉 justfile 中 -serial 后面的参数，并在新终端中执行 [sleep.sh](../../rCore-N/sleep.sh) ，确保键盘输入传入了 QEMU 串口而非 shell 的 stdin/stdout 。

编译、运行：

```sh
git submodule update --init rCore-N
cd rCore-N/os
just run
```

在 `just` 指令前设置 `LOG` 环境变量可以指定 rCore-N 的日志等级，如 `LOG=DEBUG just run` 会将日志等级设为 DEBUG 。

调试 rCore-N 可使用 `just debug` ，打开 QEMU 的调试输出使用 `just debug_qemu` 。

## 在 FPGA 上运行 rCore-N

### FPGA 环境配置

```sh
git submodule update --init labeled-RISC-V-N
cd labeled-RISC-V-N
```

后续步骤请参考[子模组仓库中相应的教程](../../labeled-RISC-V-N/fpga/README.md)和[标签化 RISC-V 架构复现指南](https://github.com/Gallium70/labeled-RISC-V-reproduction-note) 。

注意：该仓库初始化时会克隆 chisel 和 firrtl 的源代码并编译，编译时会克隆 riscv-linux（尽管本项目没有用到）和 verilator 源代码，FPGA 的综合和实现会消耗大量的 CPU 和内存资源，请保持耐心，并在开始前保存电脑上的所有工作，以防万一。

### lrv-rust-bl

```sh
git submodule update --init lrv-rust-bl
cd lrv-rust-bl
just build
```

编译好的二进制文件位于 lrv-rust-bl/target/riscv64imac-unknown-none-elf/debug/lrv-rust-bl.bin

### rCore-N

修改 [user 文件夹中的 Makefile](../../rCore-N/user/Makefile)，将第 14 行的 board_qemu 改为 board_lrv ，然后在 os 文件夹中执行 `just build_lrv` （或按需设置日志等级），编译好的二进制文件为 rCore-N/os/target/riscv64imac-unknown-none-elf/debug/os.bin 和 rcore-n.bin，二者内容完全一致。

### 在 FPGA 上运行

将 lrv-rust-bl.bin 和 rcore-n.bin 复制到 FPGA 的 Arm Linux 中，使用[标签 RISC-V 启动脚本](https://github.com/Gallium70/labeled-RISC-V-boot)启动 RISC-V 部分。
