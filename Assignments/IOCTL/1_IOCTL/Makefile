obj-m := ioctl.o

KD=/lib/modules/$(shell uname -r)/build/

PWD:=$(shell pwd)

default:
	make -C $(KD) M=$(PWD) modules
	gcc -o app app.c

clean:
	make -C $(KD) M=$(PWD) clean
	rm app
