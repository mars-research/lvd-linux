#! /bin/sh
# Test of generation of 32-bit and 64-bit maps, gzipped modules.

[ -n "$CONFIG_HAVE_ZLIB" ] || exit 0

for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

mkdir tests/tmp/data
for f in tests/data/$BITNESS$ENDIAN/map/*.ko; do
    gzip < $f > tests/tmp/data/`basename $f .ko`.ko.gz
done


rm -rf tests/tmp/*

# Copy modules instead of linking, so we can compress them
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
cp tests/data/$BITNESS$ENDIAN/map/ccw_map-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/map/pci_map-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/map/usb_map-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/map/ieee1394_map-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/map/pnp_map-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/map/input_map-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/map/of_map-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/map/input_map_new-$BITNESS.ko \
   $MODULE_DIR
gzip `find $MODULE_DIR -name '*.ko'`

touch tests/tmp/empty

# Expect no output.
[ "`depmod`" = "" ]

# Check PCI: expect 2 lines
[ `grep -vc '^#' < $MODULE_DIR/modules.pcimap` = 2 ]

[ "`grep pci_map $MODULE_DIR/modules.pcimap`" = "pci_map-$BITNESS           0x00000001 0x00000002 0x00000003 0x00000004 0x00000005 0x00000006 0x0
pci_map-$BITNESS           0x0000000b 0x0000000c 0x0000000d 0x0000000e 0x0000000f 0x00000010 0x0" ]

# Check USB: expect 2 lines
[ `grep -vc '^#' < $MODULE_DIR/modules.usbmap` = 2 ]

[ "`grep usb_map $MODULE_DIR/modules.usbmap`" = "usb_map-$BITNESS           0x0001      0x0002   0x0003    0x0004       0x0005       0x06         0x07            0x08            0x09            0x0a               0x0b               0x0
usb_map-$BITNESS           0x000b      0x000c   0x000d    0x000e       0x000f       0x10         0x11            0x12            0x13            0x14               0x15               0x0" ]

# Check CCW: expect 2 lines
[ `grep -vc '^#' < $MODULE_DIR/modules.ccwmap` = 2 ]

[ "`grep ccw_map $MODULE_DIR/modules.ccwmap`" = "ccw_map-$BITNESS           0x000f      0x0001  0x03      0x0002  0x04
ccw_map-$BITNESS           0x000f      0x000b  0x0d      0x000c  0x0e" ]

# Check ieee1394: expect 2 lines
[ `grep -vc '^#' < $MODULE_DIR/modules.ieee1394map` = 2 ]

#%-20s 0x%08x  0x%06x  0x%06x 0x%06x     0x%06x\n",
#+		name, fw->match_flags, fw->vendor_id, fw->model_id,
#+		fw->specifier_id, fw->version);
[ "`grep ieee1394_map $MODULE_DIR/modules.ieee1394map`" = "ieee1394_map-$BITNESS      0x0000000c  0x000000  0x000000 0x00a02d     0x010001
ieee1394_map-$BITNESS      0x0000000c  0x000000  0x000000 0x00a02d     0x000100" ]

# Check input: expect 4 lines
[ `grep -vc '^#' < $MODULE_DIR/modules.inputmap` = 4 ]

# module         matchBits bustype vendor product version evBits keyBits relBits absBits mscBits ledBits sndBits ffBits [swBits] driver_info
[ "`grep input_map $MODULE_DIR/modules.inputmap`" = "input_map_new-$BITNESS    0x1010  0x0  0x0  0x0  0x0  2  0  0  0  0  0  0  0  1  0x0
input_map_new-$BITNESS    0x10  0x0  0x0  0x0  0x0  40000  0  0  0  0  0  0  0  0  0x0
input_map-$BITNESS        0x10  0x0  0x0  0x0  0x0  2  0  0  0  0  0  0  0  0x0
input_map-$BITNESS        0x10  0x0  0x0  0x0  0x0  40000  0  0  0  0  0  0  0  0x0" ] || [ "`grep input_map $MODULE_DIR/modules.inputmap`" = "input_map-$BITNESS        0x10  0x0  0x0  0x0  0x0  2  0  0  0  0  0  0  0  0x0
input_map-$BITNESS        0x10  0x0  0x0  0x0  0x0  40000  0  0  0  0  0  0  0  0x0
input_map_new-$BITNESS    0x1010  0x0  0x0  0x0  0x0  2  0  0  0  0  0  0  0  1  0x0
input_map_new-$BITNESS    0x10  0x0  0x0  0x0  0x0  40000  0  0  0  0  0  0  0  0  0x0" ]

# Check of: expect 7 lines
[ `grep -vc '^#' < $MODULE_DIR/modules.ofmap` = 7 ]

# of module          name                 type                 compatible
[ "`grep of_map $MODULE_DIR/modules.ofmap`" = "of_map-$BITNESS            test_name_1          *                    *
of_map-$BITNESS            *                    test_type_1          *
of_map-$BITNESS            *                    *                    test_compat_1
of_map-$BITNESS            test_name_2          test_type_2          *
of_map-$BITNESS            test_name_3          *                    test_compat_2
of_map-$BITNESS            *                    test_type_3          test_compat_3
of_map-$BITNESS            test_name_4          test_type_4          test_compat_4" ]

mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
mv $MODULE_DIR/modules.pcimap $MODULE_DIR/modules.pcimap.old
mv $MODULE_DIR/modules.usbmap $MODULE_DIR/modules.usbmap.old
mv $MODULE_DIR/modules.ccwmap $MODULE_DIR/modules.ccwmap.old
mv $MODULE_DIR/modules.ieee1394map $MODULE_DIR/modules.ieee1394map.old
mv $MODULE_DIR/modules.isapnpmap $MODULE_DIR/modules.isapnpmap.old
mv $MODULE_DIR/modules.inputmap $MODULE_DIR/modules.inputmap.old
mv $MODULE_DIR/modules.ofmap $MODULE_DIR/modules.ofmap.old

# Synonyms
[ "`depmod -a`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
diff -u $MODULE_DIR/modules.pcimap.old $MODULE_DIR/modules.pcimap
diff -u $MODULE_DIR/modules.usbmap.old $MODULE_DIR/modules.usbmap
diff -u $MODULE_DIR/modules.ccwmap.old $MODULE_DIR/modules.ccwmap
diff -u $MODULE_DIR/modules.ieee1394map.old $MODULE_DIR/modules.ieee1394map
diff -u $MODULE_DIR/modules.isapnpmap.old $MODULE_DIR/modules.isapnpmap
diff -u $MODULE_DIR/modules.inputmap.old $MODULE_DIR/modules.inputmap
diff -u $MODULE_DIR/modules.ofmap.old $MODULE_DIR/modules.ofmap

[ "`depmod -A`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
diff -u $MODULE_DIR/modules.pcimap.old $MODULE_DIR/modules.pcimap
diff -u $MODULE_DIR/modules.usbmap.old $MODULE_DIR/modules.usbmap
diff -u $MODULE_DIR/modules.ccwmap.old $MODULE_DIR/modules.ccwmap
diff -u $MODULE_DIR/modules.ieee1394map.old $MODULE_DIR/modules.ieee1394map
diff -u $MODULE_DIR/modules.isapnpmap.old $MODULE_DIR/modules.isapnpmap
diff -u $MODULE_DIR/modules.inputmap.old $MODULE_DIR/modules.inputmap
diff -u $MODULE_DIR/modules.ofmap.old $MODULE_DIR/modules.ofmap

[ "`depmod -e -F /empty -A`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
diff -u $MODULE_DIR/modules.pcimap.old $MODULE_DIR/modules.pcimap
diff -u $MODULE_DIR/modules.usbmap.old $MODULE_DIR/modules.usbmap
diff -u $MODULE_DIR/modules.ccwmap.old $MODULE_DIR/modules.ccwmap
diff -u $MODULE_DIR/modules.ieee1394map.old $MODULE_DIR/modules.ieee1394map
diff -u $MODULE_DIR/modules.isapnpmap.old $MODULE_DIR/modules.isapnpmap
diff -u $MODULE_DIR/modules.inputmap.old $MODULE_DIR/modules.inputmap
diff -u $MODULE_DIR/modules.ofmap.old $MODULE_DIR/modules.ofmap

[ "`depmod -e -F /empty -A $MODTEST_VERSION`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
diff -u $MODULE_DIR/modules.pcimap.old $MODULE_DIR/modules.pcimap
diff -u $MODULE_DIR/modules.usbmap.old $MODULE_DIR/modules.usbmap
diff -u $MODULE_DIR/modules.ccwmap.old $MODULE_DIR/modules.ccwmap
diff -u $MODULE_DIR/modules.ieee1394map.old $MODULE_DIR/modules.ieee1394map
diff -u $MODULE_DIR/modules.isapnpmap.old $MODULE_DIR/modules.isapnpmap
diff -u $MODULE_DIR/modules.inputmap.old $MODULE_DIR/modules.inputmap
diff -u $MODULE_DIR/modules.ofmap.old $MODULE_DIR/modules.ofmap

# We expect the same from -n.
grep -hv '^#' $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.pcimap.old $MODULE_DIR/modules.usbmap.old $MODULE_DIR/modules.ccwmap.old $MODULE_DIR/modules.ieee1394map.old $MODULE_DIR/modules.isapnpmap.old $MODULE_DIR/modules.inputmap.old $MODULE_DIR/modules.ofmap.old > tests/tmp/out

# Stdout versions.
depmod -n | grep -v '^#' > tests/tmp/stdout
diff -u tests/tmp/out tests/tmp/stdout

depmod -a -n | grep -v '^#' > $MODULE_DIR/modules.dep
diff -u tests/tmp/out tests/tmp/stdout

depmod -n -a $MODTEST_VERSION | grep -v '^#' > $MODULE_DIR/modules.dep
diff -u tests/tmp/out tests/tmp/stdout

depmod -e -F /empty -n -A $MODTEST_VERSION | grep -v '^#' > $MODULE_DIR/modules.dep
diff -u tests/tmp/out tests/tmp/stdout

done
done
