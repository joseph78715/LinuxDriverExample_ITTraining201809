
obj-m	+= misc.o

all drivers:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

app:
	gcc -o testap testap.c

clean:
	make -C  /lib/modules/$(shell uname -r)/build M=$(PWD) clean
