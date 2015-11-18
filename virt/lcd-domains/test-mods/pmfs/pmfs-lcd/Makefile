# Flag to toggle some coditional compilation in the original source code
# for isolation
ccflags-y := -DLCD_ISOLATE

obj-$(CONFIG_LCD_TEST_MOD_PMFS_LCD)  += lcd-test-mod-pmfs-lcd.o

# LCD Main
lcd-test-mod-pmfs-lcd-y              += main.o

# The original code
lcd-test-mod-pmfs-lcd-y              += $(addprefix pmfs/, bbuild.o \
							  balloc.o \
							  dir.o \
							  file.o \
							  inode.o \
							  namei.o \
							  super.o \
							  symlink.o \
							  ioctl.o \
							  journal.o \
							  xip.o \
							  wprotect.o)


# LIBLCD
lcd-test-mod-pmfs-lcd-y              += ../$(LIBLCD)

# Glue
lcd-test-mod-pmfs-lcd-y              += $(addprefix glue/, \
							   vfs_caller.o \
							   bdi_caller.o)
