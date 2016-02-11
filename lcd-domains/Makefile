#
# Top-level makefile for building everything LCD-related.
#

LCD_DOMAINS_TOP_SRC_DIR=$(PWD)
export LCD_DOMAINS_TOP_SRC_DIR

# Don't print "Entering directory..." etc.
MAKEFLAGS += --no-print-directory

# Since we aren't hooked into the Linux build, we need
# to define this so we can use it in this file. (The
# kernel build system will have it def'd when it sucks
# in our Kbuild files.)
ARCH=x86

# ------------------------------------------------------------
# Microkernel build setup

MICROKERNEL_BUILD_DIR=$(LCD_DOMAINS_TOP_SRC_DIR)/microkernel_build
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

microkernel: libcap.microkernel $(MICROKERNEL_BUILD_DIR)/Kbuild | \
	$(MICROKERNEL_BUILD_DIRS)
	$(MAKE) -C .. M=$(PWD)/$(MICROKERNEL_BUILD_DIR) -j8

libcap.microkernel: $(MICROKERNEL_BUILD_DIR)/libcap.build/Makefile \
	| $(MICROKERNEL_BUILD_DIRS)
	$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libcap.build && \
	$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libcap.build install

$(MICROKERNEL_BUILD_DIR)/libcap.build/Makefile: libcap/configure
	cd $(MICROKERNEL_BUILD_DIR)/libcap.build && \
	$(LCD_DOMAINS_TOP_SRC_DIR)/libcap/configure && \
	PLATFORM=kernel --disable-test-build \
	--disable-global-cap-types \
	--with-kernel-headers=$(LCD_DOMAINS_TOP_SRC_DIR)/.. \
	--disable-kernel-module \
	--prefix=$(MICROKERNEL_BUILD_DIR)/libcap.install

$(MICROKERNEL_BUILD_DIR)/Kbuild: $(MICROKERNEL_BUILD_DIRS)
	cp scripts/Kbuild.microkernel $(MICROKERNEL_BUILD_DIR)/Kbuild

$(MICROKERNEL_BUILD_DIRS):
	mkdir -p $@

# ------------------------------------------------------------
# Some libcap tidbits

libcap/configure:
	cd libcap && ./autogen.sh

# ------------------------------------------------------------
# clean

clean: 
	$(MAKE) -C .. M=$(PWD)/$(MICROKERNEL_BUILD_DIR) clean
	if test -d $(MICROKERNEL_BUILD_DIR)/libcap.build; then \
		$(MAKE) -C $(MICROKERNEL_BUILD_DIR)/libcap.build clean; \
	fi

distclean:
	rm -rf $(MICROKERNEL_BUILD_DIR)

# ------------------------------------------------------------
# phonies

.PHONY: microkernel libcap.microkernel clean distclean
