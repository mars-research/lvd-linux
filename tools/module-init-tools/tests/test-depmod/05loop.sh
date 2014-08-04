#! /bin/sh
# Loop detection test.

for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS$ENDIAN/loop/loop1-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/loop/loop2-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/loop/loop_dep-$BITNESS.ko \
   $MODULE_DIR

# Expect no normal output.
[ "`depmod 2>tests/tmp/stderr`" = "" ]

# Check results: expect 0 lines (all have loops).
[ `grep -vc '^#' < $MODULE_DIR/modules.dep` = 0 ]

# Now, we should have only one error report, for loop1, and 3
# warnings.
[ `grep -vc '^#' < tests/tmp/stderr` = 4 ]

[ "`grep -w /lib/modules/$MODTEST_UNAME/loop1-$BITNESS.ko\ ignored tests/tmp/stderr`" = "WARNING: Module /lib/modules/$MODTEST_UNAME/loop1-$BITNESS.ko ignored, due to loop" ]
[ "`grep -w /lib/modules/$MODTEST_UNAME/loop2-$BITNESS.ko tests/tmp/stderr`" = "WARNING: Module /lib/modules/$MODTEST_UNAME/loop2-$BITNESS.ko ignored, due to loop" ]
[ "`grep -w /lib/modules/$MODTEST_UNAME/loop_dep-$BITNESS.ko tests/tmp/stderr`" = "WARNING: Module /lib/modules/$MODTEST_UNAME/loop_dep-$BITNESS.ko ignored, due to loop" ]
[ "`grep -w detected tests/tmp/stderr`" = "WARNING: Loop detected: /lib/modules/$MODTEST_UNAME/loop1-$BITNESS.ko needs loop2-$BITNESS.ko which needs loop1-$BITNESS.ko again!" ]

done
done
