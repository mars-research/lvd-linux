obj-$(CONFIG_LCD_TEST_MOD_PMFS_VFS)  += lcd-test-mod-pmfs-vfs.o

# Dispatch loop, etc.
lcd-test-mod-pmfs-vfs-y              += main.o

# Glue
lcd-test-mod-pmfs-vfs-y              += $(addprefix glue/, vfs_callee.o)
