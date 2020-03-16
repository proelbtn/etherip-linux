obj-m += etherip.o
etherip-m := main.o protocol.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

insmod:
	sudo insmod etherip.ko

rmmod:
	sudo rmmod etherip.ko

refresh: rmmod insmod
