#!/bin/bash
qemu-system-riscv64 \
	-machine virt \
	-nographic -bios rustsbi-qemu.bin \
	-device loader,file=head_qemu.bin,addr=0x80200000 \
	-drive file=disk.img,if=none,format=raw,id=x0 \
	-device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0 \
	-S -s