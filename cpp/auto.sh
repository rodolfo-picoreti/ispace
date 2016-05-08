#!/bin/bash

# shell will exit immediately if a command exits with a nonzero exit value
set -e

exec 0< /dev/null
exec 1> log.txt
exec 2> /dev/null
exec 3> /dev/tty 

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
	printf 'Unsupported OS: '$OS  >&3 
	exit 1
fi

# Get super user privileges
if [[ $EUID != 0 ]]; then
    sudo "$0" "$@"
    exit $?
fi

sudo apt-get install -y build-essential
sudo apt-get install -y cmake

printf '%-50s' ' [x] Searching for boost...'  >&3 
BOOST_VERSION=`ldconfig -p | grep -Eo 'libboost_[a-z]+.so.1.[0-9]+' | head -n 1 | cut -d . -f 4`

# boost installation 
if (("$BOOST_VERSION" < 58)); then
	printf 'not found! Installing...\n'  >&3 

	wget https://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz/download
	tar -xf download
	rm download
	cd boost_1_60_0/
	./bootstrap.sh
	sudo ./b2 install

	cd ..
	sudo chmod -R 755 boost_1_60_0/
	sudo chown -R $__USER boost_1_60_0/
	sudo ldconfig
else
	printf 'found!\n'  >&3 
fi

# librabbitmq installation 
printf '%-50s' ' [x] Searching for librabbitmq...'  >&3 
pkg-config --exists librabbitmq # exit code ($?) = 0 if successful  

if [[ $? != 0 ]]; then 
	printf 'not found! Installing...\n'  >&3 

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
else
	printf 'found!\n'  >&3 
fi

# libSimpleAmqpClient installation 
printf '%-50s' ' [x] Searching for libSimpleAmqpClient...'  >&3 
pkg-config --exists libSimpleAmqpClient # exit code ($?) = 0 if successful  

if [[ $? != 0 ]]; then 
	printf 'not found! Installing...\n'  >&3 

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
else
	printf 'found!\n'  >&3 
fi

# msgpack installation 
printf '%-50s' ' [x] Searching for msgpack...'  >&3 
pkg-config --exists msgpack # exit code ($?) = 0 if successful  
if [[ $? != 0 ]]; then 
	printf 'not found! Installing...\n'  >&3 

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
else
	printf 'found!\n'  >&3 
fi

rm log.txt