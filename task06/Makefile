obj-m += task06.o
KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$$PWD modules

clean:
	make -C $(KDIR) M=$$PWD clean

load:
	sudo insmod misc_device_driver.ko
	lsmod | grep misc

unload:
	sudo rmmod misc_device_driver.ko

reload: all unload load
