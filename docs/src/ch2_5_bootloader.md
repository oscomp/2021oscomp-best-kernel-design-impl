# 启动器与 SBI

我们基于 rustsbi 实现 QEMU 模拟器和 FPGA 上 OS 的启动与 sbi 层。

## rustsbi-qemu

我们基于 `rustsbi/rustsbi-qemu`，放开了对地址的保护，并添加了暂被 rustsbi 组织移除的多核支持以及中断委托。项目位于 [`duskmoon314/rustsbi-qemu:legacy-multicore`](https://github.com/duskmoon314/rustsbi-qemu/tree/legacy-multicore)。

### 地址保护

在 RISC-V 中，有用于保护物理地址的寄存器 `pmpcfg` 和 `pmpaddr`。原本 rustsbi-qemu 只允许 sbi 部分和 OS 所在的地址空间可以使用，而并未允许 PLIC、串口等设备所在的物理地址。出于调试方便，我们放开了所有物理地址。

### 中断委托

在修改后的 rustsbi-qemu 中，用户态的三种中断皆被 sbi 委托给 S 态的 OS，由 OS 决定如何处理。

### 多核启动

最新版的 rustsbi-qemu 正在逐步支持 RISC-V sbi 标准的 hsm 扩展，因而移除了之前的多核启动方案。我们重新支持了多核启动，通过使用 `wfi` 指令，将核 0 以外的核卡在一个循环中，直到一个 M 态软件中断来让其余核跳出循环，进入 sbi 和 os 的设置与运行。

## lrv-rust-bl
