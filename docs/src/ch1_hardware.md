# 硬件与模拟器

> 本节介绍硬件与模拟器中实现的主要特性

## QEMU with extension N

出于稳定性考虑，我们选择以 [QEMU 的 5.0 稳定分支](https://github.com/qemu/qemu/tree/stable-5.0) 为基础，添加用户态中断所需的寄存器和相应的中断处理函数。

- 添加寄存器 `ustatus` `uip` `uie` `sideleg` `sedeleg` `uepc` `utvec` `ucause` `utval` `uscratch`
- 添加用户态中断的触发部分：符合条件时使用上述处理器进入中断状态
- 实现 `uret` 指令

## Labeled RISC-V CPU
