#
# Top-level makefile for building everything LCD-related.
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
export LCD_DOMAINS_CFLAGS=-Werror \
			  -DLCD_DOMAINS \
			  -DLCD_TEST_MODS_PATH="\"$(PWD)/build/test_mods\""

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

MICROKERNEL_BUILD_DIR=$(LCD_BUILD_DIR)/microkernel_build
export MICROKERNEL_BUILD_DIR

MICROKERNEL_DIRS= \
	microkernel \
	kliblcd \
	libcap.build \
	libcap.install \
	libfipc.build \
	libfipc.install \
	libasync.build \
	libasync.install \
	common \
	arch/$(ARCH)/microkernel
MICROKERNEL_BUILD_DIRS=$(foreach dir,$(MICROKERNEL_DIRS),\
	$(MICROKERNEL_BUILD_DIR)/$(dir))

MICROKERNEL_LIB_CFLAGS= \
	"-Wall -DLCD_DOMAINS -I$(LCD_DOMAINS_TOP_SRC_DIR)/include -I$(LCD_DOMAINS_TOP_SRC_DIR)/arch/$(ARCH)/include -I$(LCD_DOMAINS_TOP_SRC_DIR)/config/non_isolated"

MICROKERNEL_LIBCAP=$(MICROKERNEL_BUILD_DIR)/libcap.install/lib/libcap.a
MICROKERNEL_LIBFIPC=$(MICROKERNEL_BUILD_DIR)/libfipc.install/lib/libfipc.a
MICROKERNEL_LIBASYNC=$(MICROKERNEL_BUILD_DIR)/libcap.install/lib/libasync.a

microkernel: $(MICROKERNEL_BUILD_DIR)/Kbuild | \
	$(MICROKERNEL_LIBCAP) $(MICROKERNEL_LIBFIPC) $(MICROKERNEL_LIBASYNC) \
	$(MICROKERNEL_BUILD_DIRS)
	$(MAKE) -C .. M=$(MICROKERNEL_BUILD_DIR) $(MAKEFLAGS)

libcap.microkernel: $(MICROKERNEL_BUILD_DIR)/libcap.build/Makefile
	$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libcap.build \
		CFLAGS=$(MICROKERNEL_LIB_CFLAGS) $(MAKEFLAGS) && \
	$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libcap.build install $(MAKEFLAGS)

$(MICROKERNEL_BUILD_DIR)/libcap.build/Makefile: libcap/configure | \
	$(MICROKERNEL_BUILD_DIRS)
	cd $(MICROKERNEL_BUILD_DIR)/libcap.build && \
	$(LCD_DOMAINS_TOP_SRC_DIR)/libcap/configure \
	PLATFORM=kernel --disable-test-build \
	--disable-global-cap-types \
	--with-kernel-headers=$(LCD_DOMAINS_TOP_SRC_DIR)/.. \
	--disable-kernel-module \
	--prefix=$(MICROKERNEL_BUILD_DIR)/libcap.install \
	CSPACE_DEPTH_BITS=$(LCD_CSPACE_DEPTH_BITS) \
	CSPACE_CNODE_TABLE_BITS=$(LCD_CSPACE_CNODE_TABLE_BITS)

libfipc.microkernel: $(MICROKERNEL_BUILD_DIR)/libfipc.build/Makefile
	$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libfipc.build \
		CFLAGS=$(MICROKERNEL_LIB_CFLAGS) $(MAKEFLAGS) && \
	$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libfipc.build install $(MAKEFLAGS)

$(MICROKERNEL_BUILD_DIR)/libfipc.build/Makefile: \
	libasync/fast-ipc-module/configure | \
	$(MICROKERNEL_BUILD_DIRS)
	cd $(MICROKERNEL_BUILD_DIR)/libfipc.build && \
	$(LCD_DOMAINS_TOP_SRC_DIR)/libfipc/configure \
	--with-kernel-headers=$(LCD_DOMAINS_TOP_SRC_DIR)/.. \
	--prefix=$(MICROKERNEL_BUILD_DIR)/libfipc.install

libasync.microkernel: $(MICROKERNEL_BUILD_DIR)/libasync.build/Makefile
	$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libasync.build \
		CFLAGS=$(MICROKERNEL_LIB_CFLAGS) $(MAKEFLAGS) && \
	$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libasync.build install $(MAKEFLAGS)

$(MICROKERNEL_BUILD_DIR)/libasync.build/Makefile: libasync/configure | \
	$(MICROKERNEL_BUILD_DIRS)
	cd $(MICROKERNEL_BUILD_DIR)/libasync.build && \
	$(LCD_DOMAINS_TOP_SRC_DIR)/libasync/configure \
	--with-kernel-headers=$(LCD_DOMAINS_TOP_SRC_DIR)/.. \
	--with-libfipc=$(MICROKERNEL_BUILD_DIR)/libfipc.install \
	--prefix=$(MICROKERNEL_BUILD_DIR)/libasync.install

$(MICROKERNEL_BUILD_DIR)/Kbuild: scripts/Kbuild.microkernel | \
	$(MICROKERNEL_BUILD_DIRS)
	cp scripts/Kbuild.microkernel $(MICROKERNEL_BUILD_DIR)/Kbuild

$(MICROKERNEL_BUILD_DIRS):
	@echo "  MKDIR      $@"
	@mkdir -p $@

# ------------------------------------------------------------
# LIBLCD build setup

LIBLCD_BUILD_DIR=$(LCD_BUILD_DIR)/liblcd_build
export LIBLCD_BUILD_DIR

LIBLCD_DIRS=$(shell cd liblcd && find . -mindepth 1 -type d)
LIBLCD_DIRS:=$(foreach d,$(LIBLCD_DIRS),liblcd/$(d))
LIBLCD_DIRS:= \
	$(LIBLCD_DIRS)
	libcap.build \
	libcap.install \
	libfipc.build \
	libfipc.install \
	libasync.build \
	libasync.install \
	common
LIBLCD_BUILD_DIRS=$(foreach dir,$(LIBLCD_DIRS),\
	$(LIBLCD_BUILD_DIR)/$(dir))

LIBLCD_LIBCAP=$(LIBLCD_BUILD_DIR)/libcap.install/lib/libcap.a
LIBLCD_LIBFIPC=$(LIBLCD_BUILD_DIR)/libfipc.install/lib/libfipc.a
LIBLCD_LIBFIPC=$(LIBLCD_BUILD_DIR)/libasync.install/lib/libasync.a

LIBLCD_LIB_CFLAGS= \
	"-Wall -DLCD_DOMAINS -DLCD_ISOLATE -DCPTR_CACHE_LOCK_PADDING_SIZE=8 -I$(LCD_DOMAINS_TOP_SRC_DIR)/include -I$(LCD_DOMAINS_TOP_SRC_DIR)/arch/$(ARCH)/include -I$(LCD_DOMAINS_TOP_SRC_DIR)/config/isolated"

# There isn't a nice easy way to multiline this, and I want access to the
# LIBLCD_BUILD_DIR variable (so I don't want to create a separate file); so 
# I mash this in one line for now.
LIBLCD_AR_SCRIPT="CREATE $(LIBLCD_BUILD_DIR)/liblcd.a\nADDLIB $(LIBLCD_BUILD_DIR)/lib.a\nADDLIB $(LIBLCD_BUILD_DIR)/libcap.install/lib/libcap.a\nADDLIB $(LIBLCD_BUILD_DIR)/libfipc.install/lib/libfipc.a\nADDLIB $(LIBLCD_BUILD_DIR)/libasync.install/lib/libasync.a\nSAVE\nEND\n"

liblcd: $(LIBLCD_BUILD_DIR)/lib.a | $(LIBLCD_LIBCAP) $(LIBLCD_LIBFIPC) \
	$(LIBLCD_LIBASYNC)
	@echo "  AR      $@"
	@echo $(LIBLCD_AR_SCRIPT) | ar -M

$(LIBLCD_BUILD_DIR)/lib.a: $(LIBLCD_BUILD_DIR)/Kbuild | \
	$(LIBLCD_BUILD_DIRS)
	$(MAKE) -C .. M=$(LIBLCD_BUILD_DIR) $(MAKEFLAGS)

libcap.liblcd: $(LIBLCD_BUILD_DIR)/libcap.build/Makefile
	$(MAKE) -C $(LIBLCD_BUILD_DIR)/libcap.build \
	CFLAGS=$(LIBLCD_LIB_CFLAGS) $(MAKEFLAGS) && \
	$(MAKE) -C $(LIBLCD_BUILD_DIR)/libcap.build install $(MAKEFLAGS)

$(LIBLCD_BUILD_DIR)/libcap.build/Makefile: libcap/configure | \
	$(LIBLCD_BUILD_DIRS)
	cd $(LIBLCD_BUILD_DIR)/libcap.build && \
	$(LCD_DOMAINS_TOP_SRC_DIR)/libcap/configure \
	PLATFORM=kernel --disable-test-build \
	--disable-kernel-module \
	--with-kernel-headers=$(LCD_DOMAINS_TOP_SRC_DIR)/.. \
	--prefix=$(LIBLCD_BUILD_DIR)/libcap.install \
	CSPACE_DEPTH_BITS=$(LCD_CSPACE_DEPTH_BITS) \
	CSPACE_CNODE_TABLE_BITS=$(LCD_CSPACE_CNODE_TABLE_BITS)

libfipc.liblcd: $(LIBLCD_BUILD_DIR)/libfipc.build/Makefile
	$(MAKE) -C $(LIBLCD_BUILD_DIR)/libfipc.build \
	CFLAGS=$(LIBLCD_LIB_CFLAGS) $(MAKEFLAGS) && \
	$(MAKE) -C $(LIBLCD_BUILD_DIR)/libfipc.build install $(MAKEFLAGS)

$(LIBLCD_BUILD_DIR)/libfipc.build/Makefile: \
	libasync/fast-ipc-module/configure | \
	$(LIBLCD_BUILD_DIRS)
	cd $(LIBLCD_BUILD_DIR)/libfipc.build && \
	$(LCD_DOMAINS_TOP_SRC_DIR)/libfipc/configure \
	--with-kernel-headers=$(LCD_DOMAINS_TOP_SRC_DIR)/.. \
	--prefix=$(LIBLCD_BUILD_DIR)/libfipc.install

libasync.liblcd: $(LIBLCD_BUILD_DIR)/libasync.build/Makefile
	$(MAKE) -C $(LIBLCD_BUILD_DIR)/libasync.build \
	CFLAGS=$(LIBLCD_LIB_CFLAGS) $(MAKEFLAGS) && \
	$(MAKE) -C $(LIBLCD_BUILD_DIR)/libasync.build install $(MAKEFLAGS)

$(LIBLCD_BUILD_DIR)/libasync.build/Makefile: libasync/configure | \
	$(LIBLCD_BUILD_DIRS)
	cd $(LIBLCD_BUILD_DIR)/libasync.build && \
	$(LCD_DOMAINS_TOP_SRC_DIR)/libasync/configure \
	--with-kernel-headers=$(LCD_DOMAINS_TOP_SRC_DIR)/.. \
	--with-libfipc=$(LIBLCD_BUILD_DIR)/libfipc.install \
	--prefix=$(LIBLCD_BUILD_DIR)/libasync.install \

$(LIBLCD_BUILD_DIR)/Kbuild: scripts/Kbuild.liblcd | \
	$(LIBLCD_BUILD_DIRS)
	cp scripts/Kbuild.liblcd $(LIBLCD_BUILD_DIR)/Kbuild

$(LIBLCD_BUILD_DIRS):
	@echo "  MKDIR      $@"
	@mkdir -p $@

# ------------------------------------------------------------
# Test modules/examples

LCD_TEST_MODULES_DIR=$(PWD)/test_mods
export LCD_TEST_MODULES_DIR

LCD_TEST_MODULES_BUILD_DIR=$(LCD_BUILD_DIR)/test_mods
export LCD_TEST_MODULES_BUILD_DIR

# Collect all of the example directories and kbuilds
TEST_MODULES_DIRS=$(shell cd $(LCD_TEST_MODULES_DIR) && \
	find . -mindepth 1 -type d)
TEST_MODULES_KBUILDS=$(shell cd $(LCD_TEST_MODULES_DIR) && \
	find . -name "Kbuild")

TEST_MODULES_BUILD_DIRS=$(foreach dir,$(TEST_MODULES_DIRS), \
	$(LCD_TEST_MODULES_BUILD_DIR)/$(dir))
TEST_MODULES_BUILD_KBUILDS=$(foreach kb,$(TEST_MODULES_KBUILDS), \
	$(LCD_TEST_MODULES_BUILD_DIR)/$(kb))

KLIBLCD_SYMBOLS=$(MICROKERNEL_BUILD_DIR)/Module.symvers

test_mods: $(TEST_MODULES_BUILD_KBUILDS) $(TEST_MODULES_BUILD_DIRS) | \
	$(LIBLCD_BUILD_DIR)/liblcd.a
	$(MAKE) -C .. M=$(LCD_TEST_MODULES_BUILD_DIR) \
	KBUILD_EXTRA_SYMBOLS=$(KLIBLCD_SYMBOLS) $(MAKEFLAGS)

# This rule does the external source hack (define "src" in Kbuild file)
$(LCD_TEST_MODULES_BUILD_DIR)/%/Kbuild: $(LCD_TEST_MODULES_DIR)/%/Kbuild | \
	$(TEST_MODULES_BUILD_DIRS)
	@echo "  KBUILD     $^"
	@printf "#\n# BEGIN: Auto-generated part.\n#\n\n" > $@ && \
	printf "LIBLCD = $(shell scripts/relpath.py $(LIBLCD_BUILD_DIR)/liblcd.a $(shell dirname $(shell readlink -f $@)))\n\n" >> $@ && \
	printf "src = $(shell dirname `readlink -f $^`)\n\n" >> $@ && \
	printf "#\n# END: Auto-generated part.\n#\n\n" >> $@ && \
	cat $^ >> $@

$(TEST_MODULES_BUILD_DIRS):
	@echo "  MKDIR      $@"
	@mkdir -p $@

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

clean-liblcd:
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

clean: clean-microkernel clean-liblcd \
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
	clean-liblcd \
	clean-testmods \
	clean-libcap \
	clean-libfipc \
	clean-libasync \
	clean \
	distclean
