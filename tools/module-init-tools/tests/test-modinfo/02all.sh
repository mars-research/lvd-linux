#! /bin/sh

# Test modinfo extraction: works for *any* endiannes.
for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS$ENDIAN/modinfo/modinfo-$BITNESS.ko \
   $MODULE_DIR

echo "/lib/modules/$MODTEST_UNAME/modinfo-$BITNESS.ko: /lib/modules/$MODTEST_UNAME/modinfo-crap-$BITNESS.ko" > $MODULE_DIR/modules.dep

# Found by reading modules.dep
OUTPUT1=`modinfo modinfo-$BITNESS 2>&1 | md5sum`
# Found by absolute path.
OUTPUT2=`modinfo /lib/modules/$MODTEST_UNAME/modinfo-$BITNESS.ko 2>&1 | md5sum`

# Same except for filename
[ x"$OUTPUT1" = x"$OUTPUT2" ]

# Expect 10 lines.
OUTPUT=`modinfo tests/data/$BITNESS$ENDIAN/modinfo/modinfo-$BITNESS.ko 2>&1`
[ `echo "$OUTPUT" | wc -l` -eq 10 ]

# Test each one.
[ "`echo "$OUTPUT" | grep filename`" =    "filename:       tests/data/$BITNESS$ENDIAN/modinfo/modinfo-$BITNESS.ko" ]
[ "`echo "$OUTPUT" | grep vermagic`" =    "vermagic:       my magic" ]
[ "`echo "$OUTPUT" | grep author`" =      "author:         AUTHOR" ]
[ "`echo "$OUTPUT" | grep description`" = "description:    DESCRIPTION" ]
[ "`echo "$OUTPUT" | grep ALIAS1`" =      "alias:          ALIAS1" ]
[ "`echo "$OUTPUT" | grep ALIAS2`" =      "alias:          ALIAS2" ]
[ "`echo "$OUTPUT" | grep foo`" =         "parm:           foo:The number of foos on the card" ]
[ "`echo "$OUTPUT" | grep ' described'`" =         "parm:           described:A well-described parameter (uint)" ]
[ "`echo "$OUTPUT" | grep ' undescribed'`" =         "parm:           undescribed:int" ]

# Test filename from modules.dep
[ "`modinfo modinfo-$BITNESS | grep filename`" =    "filename:       /lib/modules/$MODTEST_UNAME/modinfo-$BITNESS.ko" ]

# Test multiple modules on cmdline.
[ `modinfo tests/data/$BITNESS$ENDIAN/modinfo/modinfo-$BITNESS.ko tests/data/$BITNESS$ENDIAN/modinfo/modinfo-$BITNESS.ko 2>&1 | wc -l` -eq 20 ]

# Test 0-fill
[ "`modinfo -0 tests/data/$BITNESS$ENDIAN/modinfo/modinfo-$BITNESS.ko 2>&1 | tr -dc '\000' | tr '\000' @`" = "@@@@@@@@@@" ]

done
done
