KDIR ?= /lib/modules/$(shell uname -r)/build

obj-m += task01.o

all:
	make -C $(KDIR) M=$$PWD modules V=1

clean:
	make -C $(KDIR) M=$$PWD clean
