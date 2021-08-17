SHELL := /bin/bash
HOST_CC = gcc
CROSS_PREFIX = riscv64-unknown-elf-
CC = ${CROSS_PREFIX}gcc
AR = ${CROSS_PREFIX}ar
OBJDUMP = ${CROSS_PREFIX}objdump
OBJCOPY = ${CROSS_PREFIX}objcopy
CFLAGS = -O0 -w -nostdlib -T linker/riscv.lds -Wall -mcmodel=medany -Iinclude -Idrivers \
	-Itest -Itiny_libc/include -nostdinc -g -fvar-tracking -ffreestanding \
	-Idrivers/sdcard/include
USER_CFLAGS = -O0 -w -nostdlib -T linker/user_riscv.lds -Wall -mcmodel=medany -Itest -Itiny_libc/include -Iinclude -Idrivers -nostdinc -g -fvar-tracking
# USER_LDFLAGS = -L./ -ltiny_libc

BOOTLOADER_ENTRYPOINT = 0x80000000
START_ENTRYPOINT = 0x80020000

TARGET = K210
KERNEL_ENTRYPOINT = 0xffffffff80200000
ifeq ($(TARGET), qemu)
	KERNEL_ENTRYPOINT = 0xffffffff80600000
	CFLAGS += -Iqemu
	USER_CFLAGS += -Iqemu
endif
K210_SERIALPORT	= /dev/ttyUSB5

START_QEMU_ENTRY = 0x80200000

ARCH = riscv
ARCH_DIR = ./arch/$(ARCH)
ARCH_SD	= ./drivers/sdcard
CFLAGS += -Iarch/$(ARCH)/include
USER_CFLAGS += -Iarch/$(ARCH)/include
USER_LDFLAGS = $(ARCH_DIR)/crt0.o

SRC_HEAD	= $(ARCH_DIR)/kernel/head.S $(ARCH_DIR)/kernel/boot.c payload.c ./libs/string.c
SRC_ARCH	= $(ARCH_DIR)/kernel/trap.S $(ARCH_DIR)/kernel/entry.S $(ARCH_DIR)/kernel/start.S $(ARCH_DIR)/kernel/smp.S \
			$(ARCH_DIR)/sbi/common.c $(ARCH_DIR)/kernel/signal.S
SRC_SCREEN	= ./drivers/screen.c 
SRC_SDCARD	= ${ARCH_SD}/sdcard.c ${ARCH_SD}/fpioa.c ${ARCH_SD}/gpiohs.c ${ARCH_SD}/spi.c ${ARCH_SD}/utils.c \
				${ARCH_SD}/disk.c ${ARCH_SD}/plic.c
SRC_SDCARD  += ${ARCH_SD}/virtio_disk.c
SRC_DRIVER	= ./drivers/screen.c
SRC_INIT 	= ./init/main.c
SRC_INT		= ./kernel/irq/irq.c
SRC_LOCK	= ./kernel/locking/lock.c ./kernel/locking/futex.c
SRC_SCHED	= ./kernel/sched/sched.c ./kernel/sched/init.c ./kernel/sched/exec.c ./kernel/sched/signal.c \
				./kernel/sched/id.c
SRC_MM		= ./kernel/mm/mm.c ./kernel/mm/ioremap.c ./kernel/mm/mman.c ./kernel/mm/pgtable.c ./kernel/mm/mmap.c
SRC_FILE	= ./kernel/user_programs.c
SRC_SYSCALL	= ./kernel/syscall/syscall.c
SRC_ELF 	= ./kernel/elf/elf.c
SRC_FAT32	= ./kernel/fat32/fat32.c ./kernel/fat32/mount.c ./kernel/fat32/read.c ./kernel/fat32/write.c \
				./kernel/fat32/utils.c ./kernel/fat32/file.c ./kernel/fat32/pipe.c ./kernel/fat32/poll.c \
				./kernel/fat32/safe.c ./kernel/fat32/link.c ./kernel/fat32/stat.c ./kernel/fat32/directory.c
SRC_IO		= ./kernel/io/io.c ./kernel/io/select.c ./kernel/io/ring_buffer.c
SRC_SYSTEM	= ./kernel/system/system.c ./kernel/system/uname.c ./kernel/system/resource.c ./kernel/system/vm86.c
SRC_TIME	= ./kernel/time/timer.c ./kernel/time/utils.c ./kernel/time/system_time.c ./kernel/time/sleep.c \
				./kernel/time/itimer.c
SRC_LIBS	= ./libs/string.c ./libs/printk.c

SRC_LIBC	= ./tiny_libc/printf.c ./tiny_libc/string.c ./tiny_libc/mthread.c ./tiny_libc/syscall.c ./tiny_libc/invoke_syscall.S
SRC_LIBC_ASM	= $(filter %.S %.s,$(SRC_LIBC))
SRC_LIBC_C	= $(filter %.c,$(SRC_LIBC))

SRC_USER	= ./test/shell.c 

TEST_ELF	= shell.elf
ifeq ($(TARGET), qemu)
# 	TEST_ELF	+= busybox.elf
	TEST_ELF	+= lmbench_all.elf
endif

SRC_MAIN	= ${SRC_ARCH} ${SRC_INIT} ${SRC_INT} ${SRC_DRIVER} ${SRC_LOCK} ${SRC_SCHED} ${SRC_MM} ${SRC_SYSCALL} ${SRC_LIBS} \
				${SRC_FAT32} ${SRC_ELF} ${SRC_SDCARD} ${SRC_IO} ${SRC_SYSTEM} ${SRC_TIME}
SRC_IMAGE	= ./tools/createimage.c
SRC_GENMAP	= ./tools/generateMapping.c
SRC_ELF2CHAR = ./tools/elf2char.c

SRC_BURNER	= ./tools/kflash.py

SRC_LINKER = ./linker/riscv.lds

.PHONY:all

all: k210

k210: elf payload createimage image asm

qemu: elf payload createimage qemu_head asm

head: ${SRC_HEAD} ${SRC_LINKER}
	${CC} ${CFLAGS} -o head $(SRC_HEAD) -Ttext=${START_ENTRYPOINT}

payload: elf2char main
	./elf2char main > payload.c

elf2char: ${SRC_ELF2CHAR}
	${HOST_CC} ${SRC_ELF2CHAR} -o elf2char -ggdb -Wall

generateMapping: $(SRC_GENMAP)
	${HOST_CC} ${SRC_GENMAP} -o generateMapping -ggdb -Wall

main: $(SRC_MAIN) ${SRC_LINKER} user
	${CC} ${CFLAGS} -o main $(SRC_MAIN) ./kernel/user_programs.c -Ttext=${KERNEL_ENTRYPOINT}

createimage: ${SRC_IMAGE}
	${HOST_CC} ${SRC_IMAGE} -o createimage -ggdb -Wall

image: createimage head main 
	./createimage --extended head head.bin
	./createimage --extended ./bootloader/sbi-k210 ./bootloader/sbi-k210.bin
	cp ./bootloader/sbi-k210.bin k210.bin
	dd if=head.bin of=k210.bin bs=128K seek=1

run:
	sudo python3 ./tools/kflash.py -p ${K210_SERIALPORT} -b 1500000 k210.bin
	sudo python3 -m serial.tools.miniterm --eol LF --dtr 0 --rts 0 --filter direct ${K210_SERIALPORT} 115200

clean:
	rm -rf createimage main elf2char payload.c head head.bin k210.bin head_qemu head_qemu.bin main.bin
	rm -rf generateMapping bootblock
# 	rm -rf test/*.elf

asm:
	cd ./txt
	${OBJDUMP} -d main > txt/kernel.txt
# 	${OBJDUMP} -d head > txt/head.txt
# 	${OBJDUMP} -d head_qemu > txt/head_qemu.txt
# 	for testelf in $(TEST_ELF); do ${OBJDUMP} -d ./test/$$testelf > txt/$${testelf/.elf/.txt}; done

qemu_head: createimage ${SRC_HEAD}
	${CC} ${CFLAGS} -o head_qemu $(SRC_HEAD) -Ttext=${START_QEMU_ENTRY}	
	./createimage --extended head_qemu head_qemu.bin

user: elf2char generateMapping
	echo "" > user_programs.c
	echo "" > user_programs.h
	for prog in $(TEST_ELF); do ./elf2char --header-only ./test/$$prog >> user_programs.h; done
	for prog in $(TEST_ELF); do ./elf2char ./test/$$prog >> user_programs.c; done
	./generateMapping user_programs
	mv user_programs.h include/
	mv user_programs.c kernel/

libtiny_libc.a: $(SRC_LIBC_C) $(SRC_LIBC_ASM) linker/user_riscv.lds
	for libobj in $(SRC_LIBC_C); do ${CC} ${USER_CFLAGS} -c $$libobj -o $${libobj/.c/.o}; done
	for libobj in $(SRC_LIBC_ASM); do ${CC} ${USER_CFLAGS} -c $$libobj -o $${libobj/.S/.o}; done
	${AR} rcs libtiny_libc.a $(patsubst %.c, %.o, $(patsubst %.S, %.o,$(SRC_LIBC)))

$(ARCH_DIR)/crt0.o: $(ARCH_DIR)/crt0.S
	${CC} ${USER_CFLAGS} -c $(ARCH_DIR)/crt0.S -o $(ARCH_DIR)/crt0.o

elf: linker/user_riscv.lds libtiny_libc.a $(ARCH_DIR)/crt0.o
	for user in ${SRC_USER}; do ${CC} ${USER_CFLAGS} $$user ${SRC_LIBC} $(ARCH_DIR)/crt0.S -o $${user/.c/.elf}; done
