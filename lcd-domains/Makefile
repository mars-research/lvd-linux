#
# Top-level makefile for building everything LCD-related.
#
# A lot of this script has been split up into parts so it's in
# smaller bite size pieces.
#
# TARGETS (all phony):
#
#    all                 --  Build microkernel, liblcd, and all
#                            test modules/examples
#
#    libcap.X,           --  Build libcap/libfipc/libasync for X, where X is
#    libfipc.X,                   microkernel
#    libasync.X                   liblcd
#                                 libdeker
#
#                            libasync.X depends on libfipc.X being built.
#
#    microkernel         --  Build the LCD microkernel and kLIBLCD; depends
#                            on libcap/libfipc/libasync microkernel builds.
#
#    liblcd              --  Build liblcd library kernel; depends on
#                            on libcap/libfipc/libasync liblcd builds.
#
#    libdeker            --  Build libdeker; depends on
#                            on libcap/libfipc/libasync libdeker builds.
#
#    test_mods           --  Builds all of the test modules/examples. If
#                            DEKER is set, isolated test modules require
#                            libdeker; otherwise, they require liblcd.
#
#    clean               --  Remove built object files and local
#                            install directories. But doesn't delete
#                            config'd build directories.
#
#    distclean           --  Deletes all object files and build directories.
#                            Does NOT delete files created by autoconf. You
#                            need to manually delete those. (These are not
#                            under version control though, FYI.)
#

# Common top-level vars used throughout
LCD_DOMAINS_TOP_SRC_DIR=$(PWD)
export LCD_DOMAINS_TOP_SRC_DIR
export LCD_DOMAINS_CFLAGS=\
	-Werror \
	-Wall \
	-DLCD_DOMAINS \
	-DLINUX_KERNEL \
	-DLCD_TEST_MODS_PATH=\"$(PWD)/build/test_mods\"

# Don't print "Entering directory..." etc.
MAKEFLAGS +=  --no-print-directory

# Since we aren't hooked into the Linux build, we need
# to define this so we can use it in this file. (The
# kernel build system will have it def'd when it sucks
# in our Kbuild files.)
ARCH=x86

LCD_BUILD_DIR_REL=build
LCD_BUILD_DIR=$(LCD_DOMAINS_TOP_SRC_DIR)/$(LCD_BUILD_DIR_REL)

# ------------------------------------------------------------
# CSPACE configuration

# Since non-isolated code sets up the boot cptr cache for isolated code
# you need to use the same config for both environments.
LCD_CSPACE_DEPTH_BITS=3 # depth = 8
LCD_CSPACE_CNODE_TABLE_BITS=3 # cnode tables have 8 slots

# ------------------------------------------------------------
# all targets

# build_all.sh does the right target sequencing
all:
	scripts/build_all.sh $(MAKEFLAGS)

# ------------------------------------------------------------
# Microkernel build setup

include scripts/Makefile.microkernel

# ------------------------------------------------------------
# LIBLCD build setup

include scripts/Makefile.liblcd

# ------------------------------------------------------------
# LIBDEKER build setup

ifneq ($(DEKER),)
include scripts/Makefile.libdeker
else # ! DEKER
include scripts/Makefile.libdeker.dummy
endif

# ------------------------------------------------------------
# Test modules/examples

include scripts/Makefile.test_mods

# ------------------------------------------------------------
# Some lib tidbits

libcap/configure: libcap/configure.ac
	cd libcap && ./autogen.sh

libasync/fast-ipc-module/configure: libasync/fast-ipc-module/configure.ac
	cd libasync/fast-ipc-module && ./autogen.sh

libasync/configure: libasync/configure.ac
	cd libasync && ./autogen.sh

# ------------------------------------------------------------
# clean

# (clean is set in included makefiles above)

distclean: clean
	rm -rf $(LCD_BUILD_DIR)

# ------------------------------------------------------------
# phonies

.PHONY:  \
	clean \
	distclean

