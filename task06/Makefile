obj-m += task06.o
KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$$PWD modules

clean:
	make -C $(KDIR) M=$$PWD clean

load:
	sudo insmod task06.ko
	lsmod | grep task06

unload:
	sudo rmmod task06.ko

reload: all unload load
