obj-m:=seq.o
KD=/lib/modules/$(shell uname -r)/build/
PWD:=$(shell pwd)

default:
	make -C $(KD) M=$(PWD) modules
	

clean:
	make -C $(KD) M=$(PWD) clean
	

