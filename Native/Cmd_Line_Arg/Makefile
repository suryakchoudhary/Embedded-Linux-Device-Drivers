obj-m := cmd_line.o

KERN = /lib/modules/$(shell uname -r)/build

PWD = $(shell pwd)

default:
	make -C $(KERN) M=$(PWD) modules

clean: 
	make -C $(KERN) M=$(PWD) clean

