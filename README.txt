编译方法的区别：
1. include/qemu.h中，编译QEMU要把define K210注释
2. K210直接make，QEMU运行./qemu.sh脚本

运行的区别：
K210输入make run
QEMU直接运行run_qemu.sh脚本

编译K210和QEMU的实际区别：
1. Makefile中KERNEL_ENTRY_POINT：
    K210：0xffffffff80300000
    QEMU: 0xffffffff80500000
2. include/qemu.h中，编译QEMU要把define K210注释
3. Makefile的TEST_ELF，编译K210不能有busybox.elf，否则太大装不下