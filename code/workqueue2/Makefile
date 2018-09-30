obj-m += workqueue2.o

KERNEL=/lib/modules/`uname -r`/build

all:
	make -C $(KERNEL) M=$(shell pwd) modules

clean:
	make -C $(KERNEL) M=$(shell pwd) clean
