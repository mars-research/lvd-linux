#
# Top level makefile (this isn't touched by the kernel
# since we have a kbuild file)
#

export LCD_DOMAINS_INCLUDE=$(PWD)/include
export LCD_DOMAINS_CFLAGS=-Werror \
			  -I$(LCD_DOMAINS_INCLUDE) \
			  -DLCD_DOMAINS \
			  -DCONFIG_LAZY_THC

# Don't print "Entering directory..." etc.
MAKEFLAGS += --no-print-directory

all: liblcd the_rest

the_rest: liblcd libcap.microkernel
	$(MAKE) -C .. M=$(PWD) -j8

liblcd: libcap.lcd
	$(MAKE) -C .. M=$(PWD)/liblcd -j8

libcap.lcd: libcap.build.lcd/Makefile
	$(MAKE) -C libcap.build.lcd && \
	$(MAKE) -C libcap.build.lcd install

libcap.build.lcd/Makefile: libcap/configure | libcap.build.lcd libcap.out.lcd 
	cd libcap.build.lcd && \
	../libcap/configure PLATFORM=kernel --disable-test-build \
	--with-kernel-headers=$(PWD)/.. \
	--disable-kernel-module \
	--prefix=$(PWD)/libcap.out.lcd

libcap.build.lcd:
	mkdir -p libcap.build.lcd

libcap.out.lcd:
	mkdir -p libcap.out.lcd

libcap.microkernel: libcap.build.microkernel/Makefile
	$(MAKE) -C libcap.build.microkernel && \
	$(MAKE) -C libcap.build.microkernel install

libcap.build.microkernel/Makefile: libcap/configure | libcap.build.microkernel \
							libcap.out.microkernel
	cd libcap.build.microkernel && \
	../libcap/configure PLATFORM=kernel --disable-test-build \
	--disable-global-cap-types \
	--with-kernel-headers=$(PWD)/.. \
	--disable-kernel-module \
	--prefix=$(PWD)/libcap.out.microkernel

libcap.build.microkernel:
	mkdir -p libcap.build.microkernel

libcap.out.microkernel:
	mkdir -p libcap.out.microkernel

libcap/configure:
	cd libcap && ./autogen.sh

clean: 
	$(MAKE) -C .. M=$(PWD) clean
	$(MAKE) -C .. M=$(PWD)/liblcd clean
	if test -d libcap.build.lcd; then $(MAKE) -C libcap.build.lcd clean; fi
	if test -d libcap.build.microkernel; then $(MAKE) -C libcap.build.microkernel clean; fi

distclean: clean
	if test -d libcap.build.lcd; then $(MAKE) -C libcap.build.lcd distclean; fi
	if test -d libcap.build.microkernel; then \
		$(MAKE) -C libcap.build.microkernel distclean; fi
	rm -rf libcap.build.microkernel libcap.out.microkernel
	rm -rf libcap.build.lcd libcap.out.lcd

.PHONY: all the_rest liblcd libcap.microkernel libcap.lcd
