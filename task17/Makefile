obj-m += task17.o
KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$$PWD modules

clean:
	make -C $(KDIR) M=$$PWD clean

load:
	sudo insmod task17.ko
	lsmod | grep task17

unload:
	sudo rmmod task17.ko

reload: all unload load
