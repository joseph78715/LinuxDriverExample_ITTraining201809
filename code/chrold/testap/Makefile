
#CROSS_COMPILE=/usr/local/arm/4.3.1-eabi-armv6/usr/bin/arm-linux-
CC=$(CROSS_COMPILE)gcc

EXEC=testap

all:	$(EXEC)

$(EXEC):	$(EXEC).c
	$(CC) $< -o $@

clean:
	rm -f $(EXEC) *.o
