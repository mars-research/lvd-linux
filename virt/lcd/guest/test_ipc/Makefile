
#ccflags-y += -Ivirt/kvm -Iarch/x86/kvm

obj-m += rcvr.o sender.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
