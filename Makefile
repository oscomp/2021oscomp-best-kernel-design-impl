all: 
	echo "hello"
	cd codes/user && make build
	cd codes/os && make release