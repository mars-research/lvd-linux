
ccflags-y += -Ivirt/kvm -Iarch/x86/kvm

obj-$(CONFIG_LCD)	+= lcd.o
#
#obj-m 			+= lcd.o
lcd-y			+= $(addprefix ../../../virt/lcd/, core.o cap.o \
				ipc.o)
lcd-y			+= lcd-vtx.o utils.o 
