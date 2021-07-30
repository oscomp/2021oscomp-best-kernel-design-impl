## Kernel Monitor

该部件负责打印内核的部分关键信息，用来方便调试，以及在正常运行中监控内部的资源使用情况。

#### GDB debug pin

该部分用于GDB插桩。其基本原理为：预留专用内存区域，以其内部数据作为内核调试输出开关，这样，通过GDB修改对应的地址值，就可以控制调试目标、输出粒度、是否开启等各类参数。

GDB按字节粒度修改内存数据
> print *(char*)0x807ff000=1

代码内插桩使用方法：
> gdb_print!(PROCESSOR_ENABLE,"[hello]");

这种情况下，若地址“PROCESSOR_ENABLE”的字节数据大于0，就会触发print。

注意QEMU模式下开启
> pub const QEMU:usize = 1

K210模式下要关闭
> pub const QEMU:usize = 0


#### strace

strace即syscall trace，用来监控内核中对于每一个系统调用的接收，以及处理的结果。这样，我们就能够在遇到问题的时候，与linux中的strace进行比较，来判断我们的系统调用的初步正确性。

一个打印示例如下：

> sys_mmap(0x124000,4096,0,0x32,-1,0) = 0x124000