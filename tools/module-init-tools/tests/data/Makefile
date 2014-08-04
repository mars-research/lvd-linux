# Makefile to rebuild the test modules for a given bits-endianess combination
USAGE := make BITS=32|64 END=be|le [CC=..] [CFLAGS=...]

CC := gcc
CFLAGS := -Wall -DBITS_PER_LONG=$(BITS)

targets :=                                                   \
	alias/alias.ko                                       \
	complex/complex_a.ko                                 \
	complex/complex_b.ko                                 \
	complex/complex_c.ko                                 \
	complex/complex_d.ko                                 \
	complex/complex_e.ko                                 \
	loop/loop1.ko                                        \
	loop/loop2.ko                                        \
	loop/loop_dep.ko                                     \
	map/ccw_map.ko                                       \
	map/ieee1394_map.ko                                  \
	map/input_map.ko                                     \
	map/input_map_new.ko                                 \
	map/of_map.ko                                        \
	map/pci_map.ko                                       \
	map/pnp_map.ko                                       \
	map/usb_map.ko                                       \
	modinfo/modinfo.ko                                   \
	normal/export_dep.ko                                 \
	normal/export_nodep.ko                               \
	normal/noexport_dep.ko                               \
	normal/noexport_doubledep.ko                         \
	normal/noexport_nodep.ko                             \
	rename/rename-new.ko                                 \
	rename/rename-old.ko                                 \
	rename/rename-version.ko                             \
	unknown/unknown.ko

all: check_vars build

check_vars:
	@case "$(BITS)-$(END)" in                            \
	32-le | 32-be | 64-le | 64-be) ;;                    \
	*)                                                   \
		echo "Usage: $(USAGE)" >&2;                  \
		exit 1;;                                     \
	esac

build: $(patsubst %.ko, $(BITS)-$(END)/%-$(BITS).ko, $(targets))

%.ko: %.o
	mv $^ $@

# FIXME - ugly
$(BITS)-$(END)/map/%-$(BITS).o                               \
$(BITS)-$(END)/modinfo/%-$(BITS).o                           \
$(BITS)-$(END)/rename/%-$(BITS).o                            \
$(BITS)-$(END)/normal/%-$(BITS).o                            \
$(BITS)-$(END)/unknown/%-$(BITS).o                           \
$(BITS)-$(END)/complex/%-$(BITS).o                           \
$(BITS)-$(END)/alias/%-$(BITS).o                             \
$(BITS)-$(END)/loop/%-$(BITS).o                              \
$(BITS)-$(END)/normal/%-$(BITS).o                            \
: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $^
	@case "$(END)" in                                    \
	le)     end2=LSB;;                                   \
	be)	end2=MSB;;                                   \
	esac;                                                \
	desc=`file -b $@`;                                   \
	case "$$desc" in                                     \
	*\ $(BITS)-bit\ $$end2\ *) ;;                        \
	*) echo "$@ does not look like a $(BITS)-$(END) object:" >&2; \
	   echo "$$desc" >&2 ;                               \
	   exit 1;;                                          \
	esac

