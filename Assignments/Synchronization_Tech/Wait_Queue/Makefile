obj-m := wait.o

KD=/lib/modules/$(shell uname -r)/build/

PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KD) M=$(PWD) modules
	gcc -o writer writer.c
	gcc -o reader reader.c

clean:
	$(MAKE) -C $(KD) M=$(PWD) clean
	rm writer reader
