obj-m += task18.o
KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$$PWD modules

clean:
	make -C $(KDIR) M=$$PWD clean

load:
	sudo insmod task18.ko
	lsmod | grep task18

unload:
	sudo rmmod task18.ko

reload: all unload load
