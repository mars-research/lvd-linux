
# Main target
obj-$(CONFIG_LCD_TEST_MOD_LOAD)  += lcd-test-mod-load.o

lcd-test-mod-load-y              += main.o

# We don't want the loop to be optimized away
CFLAGS_lcd-test-mod-load          = -O0
