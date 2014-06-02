#
# One (and only) build file for LCD system.
#

ccflags-y               += -Werror

obj-$(CONFIG_LCD)	+= core.o

$(info $(lcd-y))
lcd-y                   += arch/$(ARCH)/lcd/lcd-vmx.o
$(info $(lcd-y))
