module add raspberry
KERNEL=kernel7
LINUX_SOURCE=./../linux
make -C $LINUX_SOURCE ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- SUBDIRS=$PWD modules
