
obj-m	+= hello.o

all drivers:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	#make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C  /lib/modules/$(shell uname -r)/build M=$(PWD) clean
