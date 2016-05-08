#!/bin/bash

__USER=$USER

if [ -f /etc/lsb-release ]; then
    . /etc/lsb-release
    OS=$DISTRIB_ID
    VER=$DISTRIB_RELEASE
elif [ -f /etc/debian_version ]; then
    OS=Debian  # XXX or Ubuntu??
    VER=$(cat /etc/debian_version)
elif [ -f /etc/redhat-release ]; then
    # TODO: add code for Red Hat and CentOS here
    ...
else
    OS=$(uname -s)
    VER=$(uname -r)
fi

if [[ $OS != "Ubuntu" ]]; then
	echo 'Unsupported OS: '$OS
	exit 1
fi

# Get super user privileges
if [[ $EUID != 0 ]]; then
    sudo "$0" "$@"
    exit $?
fi

sudo apt-get install -y build-essential
sudo apt-get install -y cmake

BOOST_VERSION=`ldconfig -p | grep -Eo 'libboost_[a-z]+.so.1.[0-9]+' | head -n 1 | cut -d . -f 4`

# boost installation 
if (("$BOOST_VERSION" < 58)); then
	echo 'libboost not found! installing...'

	wget https://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz/download
	tar -xf download
	cd boost_1_60_0/
	./bootstrap.sh
	sudo ./b2 install

	cd ..
	sudo chmod -R 755 boost_1_60_0/
	sudo chown -R $__USER boost_1_60_0/
	sudo ldconfig
fi

# librabbitmq installation 
pkg-config --exists librabbitmq # exit code ($?) = 0 if successful  

if [[ $? != 0 ]]; then 
	echo 'librabbitmq not found! installing...'

	sudo apt-get install libssl-dev -y
	
	git clone https://github.com/alanxz/rabbitmq-c
	cd rabbitmq-c
	mkdir build
	cd build
	cmake ..

	make -j4
	sudo make install
	
	LD_LIBRARY_PATH=/usr/local/lib/x86_64-linux-gnu
	export LD_LIBRARY_PATH

	cd ../..
	sudo chmod -R 755 rabbitmq-c/
	sudo chown -R $__USER rabbitmq-c/
	sudo ldconfig
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
	sudo chmod -R 755 SimpleAmqpClient/
	sudo chown -R $__USER SimpleAmqpClient/
	sudo ldconfig
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
	sudo chmod -R 755 msgpack-c/
	sudo chown -R $__USER msgpack-c/
	sudo ldconfig
fi

