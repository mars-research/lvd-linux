
# Main targets
obj-$(CONFIG_LCD_TEST_MOD_LOAD_BOOT) += lcd-test-mod-load-boot.o

lcd-test-mod-load-boot-y             += boot.o

obj-$(CONFIG_LCD_TEST_MOD_LOAD_LCD)  += lcd-test-mod-load-lcd.o

lcd-test-mod-load-lcd-y              += lcd.o

lcd-test-mod-load-lcd-y              += $(LIBLCD)

# We don't want the loop to be optimized away
CFLAGS_lcd-test-mod-load-lcd         = -O0
