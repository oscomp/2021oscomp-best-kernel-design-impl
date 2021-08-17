platform	:= k210
# platform	:= qemu
# mode 		:= debug
mode		:= release

K := kernel
U := xv6-user
T := target
BUILD_DIR := build

# config toolchain
TOOLPREFIX 	:= riscv64-unknown-elf-
# TOOLPREFIX	:= riscv64-linux-gnu-
CC 		:= $(TOOLPREFIX)gcc
AS		:= $(TOOLPREFIX)gas
LD		:= $(TOOLPREFIX)ld
# LD 		:= $(TOOLPREFIX)gcc
OBJCOPY	:= $(TOOLPREFIX)objcopy
OBJDUMP	:= $(TOOLPREFIX)objdump

QEMU	:= qemu-system-riscv64

# flags
# CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb -g
CFLAGS = -Wall -O -fno-omit-frame-pointer -ggdb -g -march=rv64imafdc
CFLAGS += -MD
CFLAGS += -mcmodel=medany
CFLAGS += -ffreestanding -fno-common -nostdlib -mno-relax
CFLAGS += -Iinclude/
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)

ifeq ($(platform), qemu)
CFLAGS += -D QEMU
endif

ASFLAGS = -Iinclude/

LDFLAGS = -z max-page-size=4096

linker = ./linker/linker64.ld

# SBI
ifeq ($(platform), k210)
	SBI := ./sbi/sbi-k210
else
	SBI	:= ./sbi/sbi-qemu
endif

# QEMU 
CPUS := 2

QEMUOPTS = -machine virt -kernel $T/kernel -m 6M -nographic

# use multi-core 
QEMUOPTS += -smp $(CPUS)

QEMUOPTS += -bios $(SBI)

# import virtual disk image
QEMUOPTS += -drive file=fs.img,if=none,format=raw,id=x0 
QEMUOPTS += -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0

# Open GDB server at localhost:1234
ifeq ($(mode), debug)
	QEMUOPTS += -gdb tcp::1234
endif 

k210-serialport := /dev/ttyUSB0

# entry file 
SRC := $K/entry.S

SRC	+= \
	$K/printf.c \
	$K/sprintf.c \
	$K/console.c \
	$K/exec.c \
	$K/intr.c \
	$K/logo.c \
	$K/main.c \
	$K/timer.c \
	$K/uname.c \
	$K/fs/bio.c \
	$K/fs/blkdev.c \
	$K/fs/fat32/cluster.c \
	$K/fs/fat32/dirent.c \
	$K/fs/fat32/fat.c \
	$K/fs/fat32/fat32.c \
	$K/fs/file.c \
	$K/fs/fs.c \
	$K/fs/mount.c \
	$K/fs/pipe.c \
	$K/fs/poll.c \
	$K/fs/rootfs.c \
	$K/mm/kmalloc.c \
	$K/mm/mmap.c \
	$K/mm/pm.c \
	$K/mm/usrmm.c \
	$K/mm/vm.c \
	$K/sched/proc.c \
	$K/sched/swtch.S \
	$K/sched/signal.c \
	$K/sync/sleeplock.c \
	$K/sync/spinlock.c \
	$K/syscall/syscall.c \
	$K/syscall/sysfile.c \
	$K/syscall/sysmem.c \
	$K/syscall/sysproc.c \
	$K/syscall/syssignal.c \
	$K/syscall/systime.c \
	$K/syscall/sysuname.c \
	$K/trap/kernelvec.S \
	$K/trap/trampoline.S \
	$K/trap/sig_trampoline.S \
	$K/trap/trap.c \
	$K/utils/list.c \
	$K/utils/rbtree.c \
	$K/utils/string.c \
	$K/hal/plic.c \
	$K/hal/disk.c
# $K/trap/fcntxt.S 

ifeq ($(platform), k210) 
SRC += \
	$K/hal/spi.c \
	$K/hal/gpiohs.c \
	$K/hal/fpioa.c \
	$K/hal/sdcard.c \
	$K/hal/dmac.c \
	$K/hal/sysctl.c \
	$K/utils/utils.c
else 
SRC += \
	$K/hal/virtio_disk.c
endif 

ifeq ($(mode), debug) 
CFLAGS += -DDEBUG 
	ifeq ($(module), all)
	CFLAGS += $(addprefix "-D__DEBUG_", $(basename $(notdir $(SRC))))
	else 
	CFLAGS += $(addprefix "-D__DEBUG_",$(module))
	endif 
endif 

# object files 
OBJ := $(basename $(SRC))
OBJ := $(addsuffix .o, $(OBJ))


# Generate binary file to burn onto k210
all: $T/kernel $(SBI) 
ifeq ($(platform), k210) 
	@$(OBJCOPY) $T/kernel --strip-all -O binary $T/kernel.bin
	@$(OBJCOPY) $(SBI) --strip-all -O binary $T/k210.bin
	@dd if=$T/kernel.bin of=$T/k210.bin bs=128k seek=1
	cp $T/k210.bin ./k210.bin
	@sudo chmod 777 $(k210-serialport)
	@python3 ./tools/kflash.py -p $(k210-serialport) -b 1500000 -t ./k210.bin
endif 

# Compile Kernel
$T/kernel: $(OBJ) 
	@if [ ! -d "$T" ]; then mkdir $T; fi
	@$(LD) $(LDFLAGS) -T $(linker) -o $@ $^
	@$(OBJDUMP) -S $@ >$T/kernel.asm

# Compile SBI 
$(SBI): 
	cd ./sbi/psicasbi && cargo build --no-default-features --features=$(platform)
	cp ./sbi/psicasbi/target/riscv64imac-unknown-none-elf/$(mode)/psicasbi $@

# explicitly compile SBI 
sbi: 
	cd ./sbi/psicasbi && cargo build --no-default-features --features=$(platform)
	cp ./sbi/psicasbi/target/riscv64imac-unknown-none-elf/$(mode)/psicasbi $(SBI)

# explicitly clean SBI 
sbi-clean: 
	cd ./sbi/psicasbi && cargo clean
	rm $(SBI)

run: all
ifeq ($(platform), k210) 
	@sudo chmod 777 $(k210-serialport)
	@python3 ./tools/kflash.py -p $(k210-serialport) -b 1500000 -t ./k210.bin
else 
	$(QEMU) $(QEMUOPTS)
endif 


# # try to generate a unique GDB port
# GDBPORT = $(shell expr `id -u` % 5000 + 25000)
# # QEMU's gdb stub command line changed in 0.11
# QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
# 	then echo "-gdb tcp::$(GDBPORT)"; \
# 	else echo "-s -p $(GDBPORT)"; fi)

# .gdbinit: debug/.gdbinit.tmpl-riscv
# 	sed "s/:1234/:$(GDBPORT)/" < $^ > $@

# qemu-gdb: $T/kernel .gdbinit fs.img
# 	@echo "*** Now run 'gdb' in another window." 1>&2
# 	$(QEMU) $(QEMUOPTS) -S $(QEMUGDB)


# Compile user programs
ulinker = ./linker/user.ld

$U/initcode: $U/initcode.S
	$(CC) $(CFLAGS) -march=rv64g -nostdinc -I. -Ikernel -c $U/initcode.S -o $U/initcode.o
	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o $U/initcode.out $U/initcode.o
	$(OBJCOPY) -S -O binary $U/initcode.out $U/initcode
	$(OBJDUMP) -S $U/initcode.o > $U/initcode.asm

ULIB = $U/ulib.o $U/usys.o $U/printf.o $U/umalloc.o $U/crt.o

_%: %.o $(ULIB)
	$(LD) $(LDFLAGS) -T $(ulinker) -e _start -o $@ $^
	$(OBJDUMP) -S $@ > $*.asm

$U/usys.o : $U/usys.pl
	@perl $U/usys.pl > $U/usys.S
	$(CC) $(CFLAGS) -c -o $U/usys.o $U/usys.S


# Prevent deletion of intermediate files, e.g. cat.o, after first build, so
# that disk image changes after first build are persistent until clean.  More
# details:
# http://www.gnu.org/software/make/manual/html_node/Chained-Rules.html
.PRECIOUS: %.o

UPROGS=\
	$U/_init\
	$U/_sh\
	$U/_cat\
	$U/_echo\
	$U/_grep\
	$U/_ls\
	$U/_kill\
	$U/_mkdir\
	$U/_xargs\
	$U/_sleep\
	$U/_find\
	$U/_rm\
	$U/_rmdir\
	$U/_wc\
	$U/_info\
	$U/_usertests\
	$U/_strace\
	$U/_mv\
	$U/_test\
	$U/_grind\
	$U/_forktest\
	$U/_stressfs\
	$U/_cowtest\
	$U/_lazytests\
	$U/_mount\
	$U/_umount\
	$U/_dup3\
	$U/_mmaptests\
	$U/_sync\
	$U/_signal_test

user: $(UPROGS)

ostest: $U/ostest2.c $U/usys.o
	$(CC) $(CFLAGS) -march=rv64g -nostdinc -I. -Ikernel -c $U/ostest2.c -o $U/ostest2.o
	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o $U/ostest2.out $U/ostest2.o $U/usys.o
	$(OBJCOPY) -S -O binary $U/ostest2.out $U/ostest2
	$(OBJDUMP) -S $U/ostest2.o > $U/ostest2.asm

dst=/mnt

# Make fs image
fs:
	@if [ ! -f "fs.img" ]; then \
		echo "making fs image..."; \
		dd if=/dev/zero of=fs.img bs=512k count=512; \
		mkfs.vfat -F 32 -s 4 fs.img; fi
	@sudo mount fs.img $(dst)
	@make sdcard dst=$(dst)
	@sudo umount $(dst)

# Write sdcard mounted at $(dst)
sdcard: user
	@if [ ! -d "$(dst)/bin" ]; then sudo mkdir $(dst)/bin; fi
	@for file in $$( ls $U/_* ); do \
		sudo cp $$file $(dst)/bin/$${file#$U/_}; done
	@sudo cp $U/_init $(dst)/init
	@sudo cp $U/_sh $(dst)/sh
	@sudo cp $U/shrc $(dst)/shrc
	@sudo cp $U/_echo $(dst)/echo
	@sudo cp README $(dst)/README

.PHONY: clean run all fs sdcard user sbi sbi-clean

clean: 
	@rm -rf $(OBJ) $(addsuffix .d, $(basename $(OBJ))) \
		target \
		k210.bin \
		.gdbinit \
		$U/*.d $U/*.o $U/*.asm $U/usys.S $U/_*