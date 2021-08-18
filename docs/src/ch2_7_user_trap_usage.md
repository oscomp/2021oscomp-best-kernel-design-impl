# 用户态中断扩展的使用

## 系统调用

我们在 rCore-N 中添加了五个新的系统调用，供用户进程使用用户态中断机制。

### init_user_trap()

该系统调用用于通知内核为进程分配中断缓冲区内存页，并将 `sstatus.UIE` 置位。注意该调用不会设置 `xie.UXIE` ，用户进程应自行设置相应位以启用中断。若无法分配内存，或进程已经调用过该函数，则本次调用会失败并返回 -1 。

### send_msg(pid, msg)

该系统调用将向 pid 所指定的进程发送一条消息，若发送失败则返回 -1 。失败的可能原因有：目标进程未初始化中断缓冲区、缓冲区已满、目标进程全局关闭中断或相应进程不存在。
这一系统调用的作用类似于 Linux 中的 `signal()` ，但目前内核尚未对消息内容进行规范，如何解析和处理消息完全由源进程和目标进程决定。后续可能会加入对 Linux 信号编号的兼容支持，以及在内核中为一些信号提供额外的处理机制，如 SIGKILL 。

### set_timer(time_us)

为进程设置一个 time_us 后到期的定时器，到期后产生一个用户态时钟中断，或在相应进程的中断缓冲区中加入该信息。该调用类似于 `alarm()` ，其效果是一次性的。

### claim_ext_int(device_id)

将 device_id 对应的外设中断分配给调用进程，并将相应的 PLIC 领取/完成寄存器和外设地址映射到用户地址空间中（目前采用恒等映射）。调用成功时，返回相应外设的基址。
该调用可能的失败原因较多，包括当前进程未启用中断（-1），外设地址映射失败（-2），外设编号不合法（-4），中断缓冲区未初始化（-5），PLIC 领取/完成寄存器地址映射失败（-6）。

该调用不会在用户上下文中使能相应的外设，使能需要通过下一个系统调用来实现。将这两部分分开一方面是为了允许用户程序更精细地控制每个外设单独的使能情况，也是为了避免在用户完成外设初始化之前外设触发中断。

### set_ext_int_enable(device_id, enable)

在 PLIC 中启用或禁用对应外设在当前硬件线程的 U 态上下文的中断。enable > 0 表示启用，否则为禁用。若启用，会同时在所有硬件线程的 S 态的上下文中禁用该中断。调用成功时返回 0 。若该外设被分配给其他进程则返回 -1 ，外设由内核管理时返回 -2 ，中断缓冲区未初始化时返回 -5 。

## 用户态中断处理函数

用户进程可以读写 `utvec` 寄存器，令其指向自定义的中断处理函数入口。为了方便起见，我们在 rCore-N 的用户运行库中提供了一些缺省的实现，包括跳板代码、全局处理函数和三类中断各自的处理函数。

跳板代码使用汇编编写，将所有通用寄存器和一部分中断 CSR 的值保存在用户栈上，将上下文的地址写入 `a0` 寄存器，并跳转到全局处理函数。全局处理函数会根据 `ucause` 判断陷入类型，若为时钟中断，则直接调用相应的处理函数；若为外部中断，则从 PLIC 领取中断编号，传给外部中断处理函数，返回后向 PLIC 提交完成信息；若为软件中断，则从中断缓冲区中读取所有中断记录，并根据中断记录中的 `cause` 的值调用相应的处理函数。

全局处理函数和三类中断处理函数的签名如下：

```rust
#[linkage = "weak"]
#[no_mangle]
pub fn user_trap_handler(cx: &mut UserTrapContext) -> &mut UserTrapContext {...}

#[linkage = "weak"]
#[no_mangle]
pub fn ext_intr_handler(irq: u16, is_from_kernel: bool) {...}

#[linkage = "weak"]
#[no_mangle]
pub fn soft_intr_handler(pid: usize, msg: usize) {...}

#[linkage = "weak"]
#[no_mangle]
pub fn timer_intr_handler(time_us: usize) {...}
```

这些函数均使用弱链接标记，用户程序可以直接定义相同签名的函数，这样编译出的可执行文件中就会链接到用户的函数，而非缺省实现。注意使用 `#[no_mangle]` 标记以避免编译器对函数重命名。

## 用户程序基本框架

```rust
#[no_mangle]
pub fn main() -> i32 {
    init_user_trap();
    claim_ext_int(device_id);
    set_ext_int_enable(device_id, 1);

    unsafe {
        uie::set_usoft();
        uie::set_utimer();
        uie::set_uext();
    }

    ...

    set_timer(time_us);
    send_msg(pid, msg);

    ...

    0
}

#[no_mangle]
pub fn ext_intr_handler(irq: u16, is_from_kernel: bool) {...}

#[no_mangle]
pub fn soft_intr_handler(pid: usize, msg: usize) {...}

#[no_mangle]
pub fn timer_intr_handler(time_us: usize) {...}
```
