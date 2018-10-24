KDIR ?= /lib/modules/$(shell uname -r)/build
MODULE_DIR ?= /lib/modules/$(shell uname -r)/

obj-m += task05.o

all:
	make -C $(KDIR) M=$$PWD modules

clean:
	make -C $(KDIR) M=$$PWD clean

install:
	cp task05.ko $(MODULE_DIR)
	depmod -a
