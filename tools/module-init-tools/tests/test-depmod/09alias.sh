#! /bin/sh

# Test alias generation.
for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS$ENDIAN/alias/alias-$BITNESS.ko \
   $MODULE_DIR

# Expect no normal output.
[ "`depmod 2>&1`" = "" ]

# Check results: expect 2 lines.
[ `grep -vc '^#' < $MODULE_DIR/modules.alias` = 2 ]

[ "`grep alias1 < $MODULE_DIR/modules.alias`" = "alias alias1 alias_$BITNESS" ]
[ "`grep alias2 < $MODULE_DIR/modules.alias`" = "alias alias2 alias_$BITNESS" ]

# Version with stuff in .modinfo section.

rm -rf tests/tmp/*

mkdir -p $MODULE_DIR
ln tests/data/$BITNESS$ENDIAN/modinfo/modinfo-$BITNESS.ko \
   $MODULE_DIR

# Expect no normal output.
[ "`depmod 2>&1`" = "" ]

# Check results: expect 2 lines.
[ `grep -vc '^#' < $MODULE_DIR/modules.alias` = 2 ]

[ "`grep ALIAS1 < $MODULE_DIR/modules.alias`" = "alias ALIAS1 modinfo_$BITNESS" ]
[ "`grep ALIAS2 < $MODULE_DIR/modules.alias`" = "alias ALIAS2 modinfo_$BITNESS" ]

done
done
