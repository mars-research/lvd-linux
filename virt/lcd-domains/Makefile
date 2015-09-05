#
# LCD arch-independent code
#

ccflags-y               	   += -Werror

# The module
obj-$(CONFIG_LCD_DOMAINS)	   += lcd-domains.o

# microkernel
lcd-domains-y                      += microkernel/main.o microkernel/ipc.o \
                                        microkernel/cap.o
# arch-dependent code
lcd-domains-y                      += ../../arch/$(ARCH)/lcd-domains/main.o

# kliblcd
lcd-domains-y                      += kliblcd/init.o kliblcd/mem.o \
                                      kliblcd/ipc.o kliblcd/enterexit.o \
                                      kliblcd/create.o kliblcd/cap.o \
                                      kliblcd/cptr_cache.o \
				      kliblcd/dstore.o \
				      kliblcd/create_klcd.o
# Unit tests module
obj-$(CONFIG_LCD_DOMAINS_TESTS)    += tests/

# Test modules
obj-$(CONFIG_LCD_TEST_MOD)         += test-mods/
