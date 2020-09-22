KDIR ?= /lib/modules/$(shell uname -r)/build

obj-m += task12.o
CFLAGS_task12.o := -DDEBUG

all:
	make -C $(KDIR) M=$$PWD modules

clean:
	make -C $(KDIR) M=$$PWD clean
