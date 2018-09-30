

#CFILES = main.c 

#obj-m += sample.o
#sample-objs := $(CFILES:.c=.o)
obj-m += tasklet1.o

all:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

