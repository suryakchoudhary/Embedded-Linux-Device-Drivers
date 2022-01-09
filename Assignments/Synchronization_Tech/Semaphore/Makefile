obj-m := sema.o

KERN = /lib/modules/$(shell uname -r)/build/

PWD:=$(shell pwd)

default:
	make -C $(KERN) M=$(PWD) modules
	gcc -o 1_writer 1_writer.c
	gcc -o 2_writer 2_writer.c
	gcc -o 1_reader 1_reader.c
	gcc -o 2_reader 2_reader.c

clean:
	make -C $(KERN) M=$(PWD) clean
	rm 1_writer 1_reader 2_writer 2_reader

