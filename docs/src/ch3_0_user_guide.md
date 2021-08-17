# 使用方法

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

justfile 中默认将 QEMU 的两个串口映射到了 /dev/pts/1 和 /dev/pts/2 ，可以在 Linux 中新打开两个终端，执行 `tty` 指令查看对应的设备路径，替换掉 justfile 中 -serial 后面的参数，并在新终端中执行 [sleep.sh](../../rCore-N/sleep.sh) ，确保键盘输入传入了 QEMU 串口而非 shell 的 stdin/stdout 。
后续可能使用 tmux 重写这一步骤，省去手动配置的过程。

编译、运行：

```sh
git submodule update --init rCore-N
cd rCore-N/os
just run
```

在 `just` 指令前设置 `LOG` 环境变量可以指定 rCore-N 的日志等级，如 `LOG=DEBUG just run` 会将日志等级设为 DEBUG 。

调试 rCore-N 可使用 `just debug` ，打开 QEMU 的调试输出使用 `just debug_qemu` 。
