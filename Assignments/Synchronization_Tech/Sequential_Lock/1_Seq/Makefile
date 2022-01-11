obj-m:=seq.o

KD=/lib/modules/$(shell uname -r)/build/

PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KD) M=$(PWD) modules
	gcc -o 1_writer 1_writer.c
	gcc -o 1_reader 1_reader.c
	gcc -o 2_reader 2_reader.c

clean:
	$(MAKE) -C $(KD) M=$(PWD) clean
	rm 1_writer 1_reader 2_reader
