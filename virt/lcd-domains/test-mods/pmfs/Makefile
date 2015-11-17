
#---------- PMFS LCD ----------

# Flag to toggle some coditional compilation in the original source code
# for isolation
ccflags-y := -DLCD_ISOLATE

# The original code
obj-$(CONFIG_LCD_TEST_MOD_PMFS_LCD)  += lcd-test-mod-pmfs-lcd.o

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

# LCD Main
lcd-test-mod-pmfs-lcd-y              += $(addprefix lcd/, main.o)

# LIBLCD
lcd-test-mod-pmfs-lcd-y              += $(LIBLCD)

# Glue
lcd-test-mod-pmfs-lcd-y              += $(addprefix glue/, vfs_caller.o \
							   bdi_caller.o)

#---------- BOOT ----------

obj-$(CONFIG_LCD_TEST_MOD_PMFS_BOOT) += lcd-test-mod-pmfs-boot.o

lcd-test-mod-pmfs-boot-y             += $(addprefix boot/, main.o)
