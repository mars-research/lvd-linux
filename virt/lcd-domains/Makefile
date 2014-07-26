#
# One (and only) build file for LCD system.
#

ccflags-y               	   += -Werror

obj-$(CONFIG_LCD_DOMAINS)	   += lcd-domains.o

obj-$(CONFIG_LCD_MODULE_LOAD_TEST) += lcd-module-load-test.o

CFLAGS_lcd-module-load-test.o      = -O0
