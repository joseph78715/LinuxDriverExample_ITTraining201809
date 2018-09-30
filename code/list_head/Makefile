obj-m += list_head.o

KERNEL=/lib/modules/`uname -r`/build

all:
	make -C $(KERNEL) M=$(shell pwd) modules 
clean:
	make -C $(KERNEL) M=$(shell pwd) clean
