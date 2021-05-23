all: 
	# rustup target add riscv64gc-unknown-none-elf
	cd codes/user && make elf
	cd codes/os && make release BOARD=k210

run:
	cd codes/user && make elf
	cd codes/os && make fat32
	cd codes/os && make run