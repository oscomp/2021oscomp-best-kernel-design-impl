all: 
	echo "hello"
	rustup target add riscv64gc-unknown-none-elf
	cd codes/user && make build
	cd codes/os && make release