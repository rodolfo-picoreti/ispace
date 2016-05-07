#!/bin/bash

# get su privileges
if [[ $EUID != 0 ]]; then
    sudo "$0" "$@"
    exit $?
fi

# librabbitmq installation 
pkg-config --exists librabbitmq # exit code ($?) = 0 if successful  

if [[ $? != 0 ]]; then 
	echo 'librabbitmq not found! installing...'

	git clone https://github.com/alanxz/rabbitmq-c
	cd rabbitmq-c
	mkdir build
	cd build
	cmake ..
	cmake --build [--config Release] .

	make -j4
	sudo make install
	
	LD_LIBRARY_PATH=/usr/local/lib/x86_64-linux-gnu
	export LD_LIBRARY_PATH

	cd ../..
	rm -rf rabbitmq-c/
fi

# libSimpleAmqpClient installation 
pkg-config --exists libSimpleAmqpClient # exit code ($?) = 0 if successful  

if [[ $? != 0 ]]; then 
	echo 'libSimpleAmqpClient not found! installing...'

	git clone https://github.com/alanxz/SimpleAmqpClient
	cd SimpleAmqpClient
	mkdir build
	cd build
	cmake ..

	make -j4
	sudo make install

	cd ../..
	rm -rf SimpleAmqpClient/
fi

# msgpack installation 
pkg-config --exists msgpack # exit code ($?) = 0 if successful  

if [[ $? != 0 ]]; then 
	echo 'msgpack not found! installing...'

	git clone https://github.com/msgpack/msgpack-c.git
	cd msgpack-c
	mkdir build
	cd build
	cmake -DMSGPACK_CXX11=ON ..

	make -j4
	sudo make install

	cd ../..
	rm -rf msgpack-c/
fi

