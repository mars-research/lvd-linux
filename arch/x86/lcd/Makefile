
ccflags-y += -Ivirt/kvm -Iarch/x86/kvm

obj-$(CONFIG_LCD)	+= lcd-domains.o
#
#obj-m 			+= lcd.o
lcd-domains-y		+= $(addprefix ../../../virt/lcd/, core.o cap.o \
				ipc.o cap-cache.o api.o)
lcd-domains-y		+= lcd-vtx.o utils.o 
