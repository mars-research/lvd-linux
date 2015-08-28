
LCD_ARCH_PATH=../../../arch/$(ARCH)/lcd-domains/tests

obj-$(CONFIG_LCD_DOMAINS_TESTS)    += lcd-tests.o

lcd-tests-y                        += runner.o cap-tests.o cptr-tests.o \
					ipc-tests.o enterexit-tests.o \
					mem-tests.o dstore-tests.o
# arch-dependent tests
lcd-tests-y                        += $(LCD_ARCH_PATH)/arch-tests.o
