obj-m += task19.o
KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$$PWD modules

clean:
	make -C $(KDIR) M=$$PWD clean

load:
	sudo insmod task19.ko
	lsmod | grep task19

unload:
	sudo rmmod task19.ko

reload: all unload load
