obj-m := char_driv_all_ops.o

KERN = /lib/modules/$(shell uname -r)/build/

PWD = $(shell pwd)

default:
	make -C $(KERN) M=$(PWD) modules
	gcc -o app app.c

clean:
	make -C $(KERN) M=$(PWD) clean
	rm app
