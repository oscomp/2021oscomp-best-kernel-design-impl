# N 扩展 <!-- omit in toc -->

> 目前此扩展的讨论基于 M/S/U 的三特权设计

- [用户态中断与异常的执行流程](#用户态中断与异常的执行流程)
  - [中断的产生](#中断的产生)
  - [异常的产生](#异常的产生)
  - [中断的处理](#中断的处理)
- [N 扩展的 CSR](#n-扩展的-csr)
  - [ustatus](#ustatus)
  - [uip uie](#uip-uie)
  - [sedeleg sideleg](#sedeleg-sideleg)
  - [uscratch](#uscratch)
  - [uepc](#uepc)
  - [ucause](#ucause)
  - [utvec](#utvec)
  - [utval](#utval)
- [N 扩展的指令](#n-扩展的指令)
  - [URET](#uret)

## 用户态中断与异常的执行流程

![用户态中断与异常的执行流程](assets/trap_flow.drawio.svg)

### 中断的产生

与 M 态和 S 态类似，中断分为软件中断 (Software Interrupt)、时钟中断 (Timer Interrupt) 和外部中断 (External Interrupt)。

> 之后为了方便，三类中断会简称为 xSI、xTI、xEI，其中 x 为特权级。

当中断发生时，通过硬件机制设置 `uip` 寄存器中的 `UXIP` 位 (X 表示中断种类) 为 1。硬件检测发现 `uip` 非零，于是进入中断的判断流程。

首先，检查该中断是否被允许发生，即检查 `uie.UXIE` 是否为 1。如果为真，再检查该中断是否被委托给用户态处理，即检查 `sideleg` 寄存器中对应的位。如果仍为真，检查全局中断使能是否为真，即检查 `ustatus.UIE` 是否为 1。如果还为真，则触发中断处理的流程。

> **注意**
>
> 上述寄存器中，`uie` `uip` 为 `mie` `mip` 的子集，即读写会同时作用于所有的 `xip` `xie` 或它们本就是同一寄存器。而 `ustatus` 至少 `UIE` 和 `UPIE` 与 `mstatus` 中的相同位相同。
>
> 虽然上述中断产生的流程中有判断次序，实际实现中一般使用组合逻辑，将寄存器值进行位与来判断，可以认为是同时判断的。

### 异常的产生

当异常发生时，硬件只检查 `sedeleg` 寄存器中对应的位是否为 1，若为真则触发异常的处理流程。

在 RISC-V 中，中断和异常的处理流程是统一的，下面出于描述简单考虑，多数情况下围绕中断进行描述。

### 中断的处理

在上述的产生流程后，处理器开始进行一些预处理：

- 设置 `ustatus.UPIE` 为 `ustatus.UIE` 的值，并置 `ustatus.UIE` 为 0
- 根据中断类型设置 `ucause`
- 设置 `uepc` 为发生中断或异常时的 `pc`
- (可选) 设置 `utval`
- 根据 `utvec` 的最低二位和高位的基地址，跳转到设置好的中断处理程序

中断处理程序需要完成以下工作：

- 保存现场
- 如果 `utvec` 为 direct 模式，根据 `ucause` 执行相应的处理程序
- 如果 `utvec` 为 vector 模式，执行处理程序
- 恢复现场

需要保存和恢复的现场有

- x1-x31 寄存器 (如果确定中断处理程序中不会使用到某些寄存器，可以省去保存和恢复)
- `ustatus` (可能需要通过修改 `ustatus` 来改变 CPU 状态)
- `uepc` (可能需要通过修改进一步触发 S 态的中断/异常)
- **我们尚未注意到的但需要保存和恢复的寄存器**

![中断与异常的硬件处理流程](assets/trap.drawio.svg)

## N 扩展的 CSR

> WPRI: 该段数值保留为以后使用，欲读取该位时得到合法的值，即 0

### ustatus

```
UXLEN-1 5   4    3  1    0
┌────────┬──────┬──────┬─────┐
│  WPRI  │ UPIE │ WPRI │ UIE │
└────────┴──────┴──────┴─────┘
  UXLEN-5   1       3     1
```

`ustatus` 是一个 UXLEN 位长的读写寄存器，记录和控制硬件的工作状态。

用户态中断使能位 UIE 为零时阻止用户态中断的发生。为了向用户态陷入处理程序提供原子性， UIE 中的值在用户态中断被处理时复制到 UPIE 并被置为零。

UIE 和 UPIE 是 `mstatus` 和 `sstatus` 中对应位的镜像。在硬件实现中三者可以是同一寄存器。

指令 URET 用于从陷入状态返回用户态。URET 将 UPIE 复制回 UIE，然后将 UPIE 置位，最后将 `uepc` 拷贝至 `pc`。

用户态中断只能在用户态触发，所以不需要 UPP 位。

### uip uie

`uip` 和 `uie` 均为 UXLEN 位的读写寄存器，其中 `uip` 存储等待处理的中断信息， `uie` 存储相应的中断使能位。

```
| WPRI | UEIP | WPRI | UTIP | WPRI | USIP |

| WPRI | UEIE | WPRI | UTIE | WPRI | USIE |
```

定义三种中断：软件中断、时钟中断、外部中断。用户态软件中断通过置位当前的 hart 的 `uip` 的软件中断等待 (USIP) 来触发。清零该位可以取消等待中的软件中断。当 `uie` 中的 USIE 为零时，用户态软件中断被禁止。

ABI 应当提供一种向其他 hart 发送处理器间中断的机制，这最终将置位接收 hart 的 `uip` 寄存器的 USIP 位。

除了 USIP， `uip` 中的其他位在用户态是可读的。

如果 `uip` 寄存器中的 UTIP 位被置位，一个用户态定时器中断将进入待处理状态。当 `uie` 寄存器中的 UTIE 位被置零时，用户态定时器中断被禁用。ABI 应该提供一个机制来清除一个待定的定时器中断。

如果 `uip` 寄存器中的 UEIP 位被置位，一个用户态外部中断将进入待处理状态。当 `uie` 寄存器中的 UEIE 位被置位时，用户态外部中断被禁用。ABI 应该提供屏蔽、解除屏蔽和查询外部中断原因的机制。

`uip` 和 `uie` 寄存器是 `mip` 和 `mie` 寄存器的子集。读取 `uip`/`uie` 的任何字段，或者写入任何可写字段，都会对 `mip`/`mie` 的相应字段进行读取或写入。如果系统实现了 S 模式，`uip` 和 `uie` 寄存器也是 `sip` 和 `sie` 寄存器的子集。

### sedeleg sideleg

为提升中断和异常的处理性能，可以实现独立的读写寄存器 `sedeleg` 和 `sideleg`，设置其中的位将特定的中断和异常交由用户态陷入处理程序处理。

当一个陷入被委托给一个权限较低的模式 u 时，`ucause` 寄存器被写入陷阱的原因；`uepc` 寄存器被写入发生陷阱的指令的虚拟地址；`utval` 寄存器被写入一个特定的异常数据；`mstatus` 的 UPIE 字段被写入陷阱发生时 UIE 字段的值；`mstatus` 的 UIE 字段被清零。`mcause`/`scause` 和 `mepc`/`sepc` 寄存器以及 `mstatus` 的 MPP 和 MPIE 字段不被写入。

一个实现不应硬性规定任何委托位为一，也就是说，任何可以被委托的陷阱都必须支持不被委托。一个实现方案是选择可委托的陷入的子集。支持的可委托位可通过向每个比特位置写 1，然后读回 `medeleg`/`sedeleg` 或 `mideleg`/`sideleg` 中的值，看看哪些位上有 1。

> 目前，不支持触发低权限级的陷入

不会在用户态发生的应硬件恒零，如 ECall from S/H/M-mode

### uscratch

`uscratch` 寄存器是一个 UXLEN 位读/写寄存器。

### uepc

`uepc` 是 UXLEN 位读写寄存器。最低位（`uepc[0]`）恒零。次低位 `uepc[1]` 视实现的对齐需求而定。

`uepc` 是 WARL 寄存器，应能存储所有的合法物理/虚拟地址，但不需要能挣钱存储非法地址。实现可以先将非法地址转为其他非法地址再写入 `uepc`。

但陷入在用户态处理时，`uepc` 被写入中断或触发异常的指令的虚拟地址。此外，除了软件显式地写，否则 `uepc` 应永不被写。

### ucause

```
| Interrupt | Exception Code WLRL |
```

`ucause` 是 UXLEN 位长读写寄存器。

### utvec

```
| BASE[UXLEN-1 : 2] | MODE |
```

`utvec` 是 UXLEN 位长读写寄存器，存储陷入向量，包括向量基地址和向量模式。

BASE 是 WARL，可以存储任意合法的虚拟地址或物理地址，需要 4 字节对齐。特殊的模式可以有其他对齐标准。

| value | name     | description       |
| ----- | -------- | ----------------- |
| 0     | direct   | base              |
| 1     | vectored | base + 4 \* cause |
|       |          | reserved          |

### utval

存储内容仍在讨论中

## N 扩展的指令

### URET

`uret` 将 `pc` 设置为 `uepc` ，将 `ustatus.UIE` 设置为 `ustatus.UPIE` ，从而恢复中断前的状态。
