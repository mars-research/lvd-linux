
# MINIX --------------------------------------------------

obj-$(CONFIG_LCD_TEST_MOD_GLUE_EXAMPLE_MINIX) += \
			lcd-test-mod-glue-example-minix.o

# original code
lcd-test-mod-glue-example-minix-y    += original/main.o

# if we're isolating ...

# link with liblcd
lcd-test-mod-glue-example-minix-$(CONFIG_ISOLATE_GLUE_EXAMPLE) += ../$(LIBLCD)

# link with glue
lcd-test-mod-glue-example-minix-$(CONFIG_ISOLATE_GLUE_EXAMPLE) += glue/boot.o
lcd-test-mod-glue-example-minix-$(CONFIG_ISOLATE_GLUE_EXAMPLE) += glue/vfs_caller.o

