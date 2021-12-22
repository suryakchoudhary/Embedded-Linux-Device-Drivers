obj-m := hello.o

KRN1 = /lib/modules/$(shell uname -r)/build/

PWD := $(shell pwd)

default:
	make -C $(KRN1) M=$(PWD) modules

clean: 
	make -C $(KRN1) M=$(PWD) clean
