obj-m := task09.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
WARN_FLAGS += -Wall -Wextra

.PHONY: default

default:
	make -C $(KDIR) M=$$PWD modules V=1

install:
	sudo insmod task09.ko

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean 
