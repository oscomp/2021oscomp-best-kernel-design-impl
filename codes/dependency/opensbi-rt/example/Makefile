arch ?= riscv64
target := $(arch)imac-unknown-none-elf
mode := debug
kernel := target/$(target)/$(mode)/opensbi-rt-example
bin := target/$(target)/$(mode)/kernel.bin

sysroot := $(shell rustc --print sysroot)
objdump := $(shell find $(sysroot) -name llvm-objdump) --arch-name=$(arch)
objcopy := $(shell find $(sysroot) -name llvm-objcopy)

ifeq ($(mode), release)
	BUILD_ARGS += --release
endif

ifeq ($(arch), riscv32)
	START_ADDR := 0x80400000
else ifeq ($(arch), riscv64)
	START_ADDR := 0x80200000
endif

.PHONY: kernel build clean qemu run env

build: $(bin)

env:
	rustup component add llvm-tools-preview rustfmt
	rustup target add $(target)

kernel:
	cargo build $(BUILD_ARGS)

$(bin): kernel
	$(objcopy) $(kernel) --strip-all -O binary $@

asm:
	$(objdump) -d $(kernel) | less

sym:
	$(objdump) -t $(kernel) | less

header:
	$(objdump) -x $(kernel) | less

clean:
	cargo clean

qemu: $(bin)
	qemu-system-$(arch) \
		-machine virt \
		-nographic \
		-bios default \
		-device loader,file=$(bin),addr=$(START_ADDR)

run: build qemu
