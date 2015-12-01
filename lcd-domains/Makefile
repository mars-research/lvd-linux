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

the_rest: liblcd
	$(MAKE) -C .. M=$(PWD) -j8

liblcd:
	$(MAKE) -C .. M=$(PWD)/liblcd -j8

clean: 
	$(MAKE) -C .. M=$(PWD) clean
	$(MAKE) -C .. M=$(PWD)/liblcd clean

.PHONY: all the_rest liblcd
