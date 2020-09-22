KDIR ?= /lib/modules/$(shell uname -r)/build

obj-m += task13.o
CFLAGS_task13.o := -DDEBUG

all:
	make -C $(KDIR) M=$$PWD modules

clean:
	make -C $(KDIR) M=$$PWD clean
