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
#    libcap.microkernel  --  Build libcap for the microkernel
#
#    libfipc.microkernel --  Build libfipc for the non-isolated environment
#
#    libasync.microkernel--  Build libasync for the non-isolated environment;
#                            depends on libfipc.microkernel
#
#    microkernel         --  Build the LCD microkernel and kLIBLCD;
#                            depends on libcap, libfipc, libasync 
#                            non-isolated builds being done
#                            from prior make run (we don't list this as
#                            a target dependency because otherwise it triggers
#                            a full microkernel re-build every time)
#
#    libcap.liblcd       --  Build libcap for isolated liblcd library kernel
#
#    libfipc.liblcd      --  Build libfipc for the isolated environment
#
#    libasync.liblcd     --  Build libasync for the isolated environment;
#                            depends on libfipc.liblcd
#
#    liblcd              --  Build liblcd library kernel;
#                            depends on libcap, libfipc, and libasync builds
#                            being exec'd from prior make run
#
#    test_mods           --  Builds all of the test modules/examples. It 
#                            depends on liblcd target being exec'd from 
#                            prior make run.
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
MAKEFLAGS += --no-print-directory

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
LCD_CSPACE_DEPTH_BITS=2 # depth = 4
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

clean-microkernel:
	if test -e $(MICROKERNEL_BUILD_DIR)/Kbuild; then \
		$(MAKE) -C .. M=$(MICROKERNEL_BUILD_DIR) clean; \
	fi

clean--liblcd:
	if test -e $(LIBLCD_BUILD_DIR)/Kbuild; then \
		$(MAKE) -C .. M=$(LIBLCD_BUILD_DIR) clean; \
	fi

clean-lib%:
	if test -d $(MICROKERNEL_BUILD_DIR)/$(subst clean-,,$@).build; then \
		$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/$(subst clean-,,$@).build clean; \
	fi
	if test -d $(LIBLCD_BUILD_DIR)/$(subst clean-,,$@).build; then \
		$(MAKE) -C $(LIBLCD_BUILD_DIR)/$(subst clean-,,$@).build clean; \
	fi
	rm -rf $(MICROKERNEL_BUILD_DIR)/$(subst clean-,,$@).install
	rm -rf $(LIBLCD_BUILD_DIR)/$(subst clean-,,$@).install

clean-testmods:
	if test -e $(LCD_TEST_MODULES_BUILD_DIR)/Kbuild; then \
		$(MAKE) -C .. M=$(LCD_TEST_MODULES_BUILD_DIR) clean; \
	fi

clean: clean-microkernel clean--liblcd \
	clean-libcap clean-libfipc clean-libasync \
	clean-testmods

distclean:
	rm -rf $(LCD_BUILD_DIR)

# ------------------------------------------------------------
# phonies

.PHONY: \
	microkernel \
	libcap.microkernel \
	libfipc.microkernel \
	libasync.microkernel \
	liblcd \
	$(LIBLCD_BUILD_DIR)/lib.a \
	libcap.liblcd \
	libfipc.liblcd \
	libasync.liblcd \
	test_mods \
	clean \
	clean-microkernel \
	clean--liblcd \
	clean-testmods \
	clean \
	distclean
