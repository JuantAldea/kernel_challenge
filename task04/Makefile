KDIR ?= /lib/modules/$(shell uname -r)/build

obj-m += coding_style.o

all:
	make -C $(KDIR) M=$$PWD modules

clean:
	make -C $(KDIR) M=$$PWD clean
