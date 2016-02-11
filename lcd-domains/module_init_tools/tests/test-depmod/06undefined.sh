#! /bin/sh
# Check for undefined symbols.

for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS$ENDIAN/unknown/unknown-$BITNESS.ko \
   $MODULE_DIR

touch tests/tmp/empty

# Without -e, doesn't matter.
[ "`depmod -F /empty 2>&1`" = "" ]

# Without -F doesn't matter
[ "`depmod -e 2>&1`" = "WARNING: -e needs -E or -F" ]

[ "`depmod -e -F /empty 2>&1`" = "WARNING: /lib/modules/$MODTEST_UNAME/unknown-$BITNESS.ko needs unknown symbol undefined" ]

done
done
