
# VFS --------------------------------------------------

obj-$(CONFIG_LCD_TEST_MOD_GLUE_EXAMPLE_VFS) += \
			lcd-test-mod-glue-example-vfs.o

# original code
lcd-test-mod-glue-example-vfs-y      += original/main.o

# if we're isolating ...

# link with liblcd
lcd-test-mod-glue-example-vfs-$(CONFIG_ISOLATE_GLUE_EXAMPLE) += ../$(LIBLCD)

# link with glue
lcd-test-mod-glue-example-vfs-$(CONFIG_ISOLATE_GLUE_EXAMPLE)  += glue/boot.o
lcd-test-mod-glue-example-vfs-$(CONFIG_ISOLATE_GLUE_EXAMPLE)  += glue/vfs_callee.o

# Run linker script through C preprocessor (so we can use macros)
extra-y := glue/vfs_callee.lds

# run link script
ldflags-$(CONFIG_ISOLATE_GLUE_EXAMPLE) += -T $(srctree)/$(src)/glue/vfs_callee.lds
