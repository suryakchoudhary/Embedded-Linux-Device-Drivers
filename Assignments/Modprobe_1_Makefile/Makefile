obj-m := Add_Use/add.o Fun_Def/exp_fun.o

KERN = /lib/modules/$(shell uname -r)/build/

PWD = $(shell pwd)

default:
	make -C $(KERN) M=$(PWD) modules

install:
	make -C $(KERN) M=$(PWD) modules_install
	depmod -a

clean:
	make -C $(KERN) M=$(PWD) clean
	rm -rf /lib/modules/$(shell uname -r)/extra/*
	depmod -a
