#
# LCD arch-independent code
#

ccflags-y               	   += -Werror

# The module
obj-$(CONFIG_LCD_DOMAINS)	   += lcd-domains.o

lcd-domains-y                      += main.o ipc.o kliblcd.o cap.o

lcd-domains-y                      += ../../arch/$(ARCH)/lcd-domains/main.o

# Test modules

obj-$(CONFIG_LCD_TEST_MOD)         += test-mods/

# LCD Debug
obj-$(CONFIG_LCD_DEBUG)            += debug/
