#
# Top-level makefile for building everything LCD-related.
#
# TARGETS:
#
#    libcap.microkernel  --  Build libcap for the microkernel
#
#    microkernel         --  Build the LCD microkernel and kLIBLCD
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
			  -DLCD_DOMAINS

# Don't print "Entering directory..." etc.
MAKEFLAGS += --no-print-directory

# Since we aren't hooked into the Linux build, we need
# to define this so we can use it in this file. (The
# kernel build system will have it def'd when it sucks
# in our Kbuild files.)
ARCH=x86

LCD_BUILD_DIR=$(LCD_DOMAINS_TOP_SRC_DIR)/build

# ------------------------------------------------------------
# Microkernel build setup

MICROKERNEL_BUILD_DIR=$(LCD_BUILD_DIR)/microkernel_build
export MICROKERNEL_BUILD_DIR

MICROKERNEL_DIRS= \
	microkernel \
	kliblcd \
	libcap.build \
	libcap.install \
	common \
	arch/$(ARCH)/microkernel
MICROKERNEL_BUILD_DIRS=$(foreach dir,$(MICROKERNEL_DIRS),\
	$(MICROKERNEL_BUILD_DIR)/$(dir))

MICROKERNEL_LIBCAP=$(MICROKERNEL_BUILD_DIR)/libcap.install/lib/libcap.a
MICROKERNEL_LIBCAP_CFLAGS= \
	"-DLCD_DOMAINS -I$(LCD_DOMAINS_TOP_SRC_DIR)/include -I$(LCD_DOMAINS_TOP_SRC_DIR)/config/non_isolated"

microkernel: $(MICROKERNEL_BUILD_DIR)/Kbuild $(MICROKERNEL_LIBCAP) | \
	$(MICROKERNEL_BUILD_DIRS)
	$(MAKE) -C .. M=$(MICROKERNEL_BUILD_DIR) -j8

# (We don't combine this with the LCD microkernel build, because install
# triggers every time, and as a result, triggers a full microkernel
# rebuild.)
libcap.microkernel: $(MICROKERNEL_BUILD_DIR)/libcap.build/Makefile
	$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libcap.build \
		CFLAGS=$(MICROKERNEL_LIBCAP_CFLAGS) && \
	$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libcap.build install

$(MICROKERNEL_BUILD_DIR)/libcap.build/Makefile: libcap/configure | \
	$(MICROKERNEL_BUILD_DIRS)
	cd $(MICROKERNEL_BUILD_DIR)/libcap.build && \
	$(LCD_DOMAINS_TOP_SRC_DIR)/libcap/configure \
	PLATFORM=kernel --disable-test-build \
	--disable-global-cap-types \
	--with-kernel-headers=$(LCD_DOMAINS_TOP_SRC_DIR)/.. \
	--disable-kernel-module \
	--prefix=$(MICROKERNEL_BUILD_DIR)/libcap.install

$(MICROKERNEL_BUILD_DIR)/Kbuild: scripts/Kbuild.microkernel | \
	$(MICROKERNEL_BUILD_DIRS)
	cp scripts/Kbuild.microkernel $(MICROKERNEL_BUILD_DIR)/Kbuild

$(MICROKERNEL_BUILD_DIRS):
	mkdir -p $@

# ------------------------------------------------------------
# LIBLCD build setup

LIBLCD_BUILD_DIR=$(LCD_BUILD_DIR)/liblcd_build
export LIBLCD_BUILD_DIR

LIBLCD_DIRS= \
	liblcd/lcd-domains \
	liblcd/lcd-domains/async \
	liblcd/mm \
	liblcd/lib \
	libcap.build \
	libcap.install \
	common
LIBLCD_BUILD_DIRS=$(foreach dir,$(LIBLCD_DIRS),\
	$(LIBLCD_BUILD_DIR)/$(dir))

LIBLCD_LIBCAP=$(LIBLCD_BUILD_DIR)/libcap.install/lib/libcap.a

LIBLCD_LIBCAP_CFLAGS= \
	"-DLCD_DOMAINS -I$(LCD_DOMAINS_TOP_SRC_DIR)/include -I$(LCD_DOMAINS_TOP_SRC_DIR)/config/isolated"

# There isn't a nice easy way to multiline this, and I want access to the
# LIBLCD_BUILD_DIR variable (so I don't want to create a separate file); so 
# I mash this in one line for now.
LIBLCD_AR_SCRIPT="CREATE $(LIBLCD_BUILD_DIR)/liblcd.a\nADDLIB $(LIBLCD_BUILD_DIR)/lib.a\nADDLIB $(LIBLCD_BUILD_DIR)/libcap.install/lib/libcap.a\nSAVE\nEND\n"

liblcd: $(LIBLCD_BUILD_DIR)/liblcd.a

$(LIBLCD_BUILD_DIR)/liblcd.a: $(LIBLCD_BUILD_DIR)/lib.a $(LIBLCD_LIBCAP)
	@echo "  AR      $@"
	@echo $(LIBLCD_AR_SCRIPT) | ar -M

$(LIBLCD_BUILD_DIR)/lib.a: $(LIBLCD_BUILD_DIR)/Kbuild | \
	$(LIBLCD_BUILD_DIRS)
	$(MAKE) -C .. M=$(LIBLCD_BUILD_DIR) -j8

libcap.liblcd: $(LIBLCD_BUILD_DIR)/libcap.build/Makefile
	$(MAKE) -C $(LIBLCD_BUILD_DIR)/libcap.build \
	CFLAGS=$(LIBLCD_LIBCAP_CFLAGS) && \
	$(MAKE) -C $(LIBLCD_BUILD_DIR)/libcap.build install

$(LIBLCD_BUILD_DIR)/libcap.build/Makefile: libcap/configure | \
	$(LIBLCD_BUILD_DIRS)
	cd $(LIBLCD_BUILD_DIR)/libcap.build && \
	$(LCD_DOMAINS_TOP_SRC_DIR)/libcap/configure \
	PLATFORM=kernel --disable-test-build \
	--disable-kernel-module \
	--with-kernel-headers=$(LCD_DOMAINS_TOP_SRC_DIR)/.. \
	--prefix=$(LIBLCD_BUILD_DIR)/libcap.install

$(LIBLCD_BUILD_DIR)/Kbuild: scripts/Kbuild.liblcd | \
	$(LIBLCD_BUILD_DIRS)
	cp scripts/Kbuild.liblcd $(LIBLCD_BUILD_DIR)/Kbuild

$(LIBLCD_BUILD_DIRS):
	mkdir -p $@

# ------------------------------------------------------------
# Some libcap tidbits

libcap/configure: libcap/configure.ac
	cd libcap && ./autogen.sh

# ------------------------------------------------------------
# clean

clean: 
	if test -e $(MICROKERNEL_BUILD_DIR)/Makefile; then \
		$(MAKE) -C .. M=$(MICROKERNEL_BUILD_DIR) clean; \
	fi
	if test -e $(LIBLCD_BUILD_DIR)/Makefile; then \
		$(MAKE) -C .. M=$(LIBLCD_BUILD_DIR) clean; \
	fi
	if test -d $(MICROKERNEL_BUILD_DIR)/libcap.build; then \
		$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libcap.build clean; \
	fi
	if test -d $(LIBLCD_BUILD_DIR)/libcap.build; then \
		$(MAKE) -C $(LIBLCD_BUILD_DIR)/libcap.build clean; \
	fi
	rm -rf $(MICROKERNEL_BUILD_DIR)/libcap.install
	rm -rf $(LIBLCD_BUILD_DIR)/libcap.install

distclean:
	rm -rf $(LCD_BUILD_DIR)

# ------------------------------------------------------------
# phonies

.PHONY: \
	microkernel \
	libcap.microkernel \
	liblcd \
	libcap.liblcd \
	clean \
	distclean
