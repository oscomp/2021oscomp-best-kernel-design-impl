#!/bin/bash
# dd if=/dev/zero of=disk.img bs=3M count=2
# mkfs.vfat -F 32 -s2 disk.img
qemu-system-riscv64 \
	-machine virt \
	-nographic -bios rustsbi-qemu.bin \
	-device loader,file=head_qemu.bin,addr=0x80200000 \
	-drive file=mkfs/fs.img,if=none,format=raw,id=x0 \
	-device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0 \
	-S -s