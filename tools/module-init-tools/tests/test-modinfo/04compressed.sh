#! /bin/sh
# Test modinfo extraction on compressed modules.

[ -n "$CONFIG_HAVE_ZLIB" ] || exit 0

for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# Copy modules instead of linking, so we can compress them
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
cp tests/data/$BITNESS$ENDIAN/modinfo/modinfo-$BITNESS.ko \
   $MODULE_DIR

gzip $MODULE_DIR/modinfo-$BITNESS.ko

echo "/lib/modules/$MODTEST_UNAME/modinfo-$BITNESS.ko.gz: /lib/modules/$MODTEST_UNAME/modinfo-crap-$BITNESS.ko.gz" > $MODULE_DIR/modules.dep

# Test individual field extraction: by module search and abs. path
for file in modinfo-$BITNESS $MODULE_DIR/modinfo-$BITNESS.ko.gz; do
    [ "`modinfo -F randomcrap $file 2>&1`" = "my random crap which I use to test stuff with" ]
    [ "`modinfo -F vermagic $file 2>&1`" = "my magic" ]
    [ "`modinfo -F author $file 2>&1`" = "AUTHOR" ]
    [ "`modinfo -F description $file 2>&1`" = "DESCRIPTION" ]
    [ "`modinfo -F alias $file 2>&1`" = "ALIAS1
ALIAS2" ] || [ "`modinfo -F alias $file 2>&1`" = "ALIAS2
ALIAS1" ]
    [ "`modinfo -F parm $file 2>&1`" = "foo:The number of foos on the card
described:A well-described parameter" ] ||
    [ "`modinfo -F parm $file 2>&1`" = "described:A well-described parameter
foo:The number of foos on the card" ]

    [ "`modinfo -F parmtype $file 2>&1`" = "described:uint
undescribed:int" ] ||
    [ "`modinfo -F parmtype $file 2>&1`" = "undescribed:int
described:uint" ]
    [ "`modinfo -F unknown $file 2>&1`" = "" ]
done

# Test multiple modules on cmdline.
[ "`modinfo -F vermagic $MODULE_DIR/modinfo-$BITNESS.ko.gz $MODULE_DIR/modinfo-$BITNESS.ko.gz 2>&1`" = "my magic
my magic" ]

# Test 0-fill
[ "`modinfo -0 -F alias $MODULE_DIR/modinfo-$BITNESS.ko.gz 2>&1 | tr '\000' @`" = "ALIAS1@ALIAS2@" ]

done
done
