#! /bin/sh
# Complex tests of generation of 32-bit and 64-bit modules.dep
# Direct dependencies:
#   A: B C D
#   B: E
#   C: B E
#   D: B
#   E:

for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS$ENDIAN/complex/complex_a-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/complex/complex_b-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/complex/complex_c-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/complex/complex_d-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/complex/complex_e-$BITNESS.ko \
   $MODULE_DIR

touch tests/tmp/empty

# Expect no output.
[ "`depmod 2>&1`" = "" ]

# Check results: expect 5 lines
[ `grep -vc '^#' < $MODULE_DIR/modules.dep` = 5 ]

# Could be a: c d b e or A: d c b e.
[ "`grep -w complex_a-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "complex_a-$BITNESS.ko: complex_c-$BITNESS.ko complex_d-$BITNESS.ko complex_b-$BITNESS.ko complex_e-$BITNESS.ko" ] || [ "`grep -w complex_a-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "complex_a-$BITNESS.ko: complex_d-$BITNESS.ko complex_c-$BITNESS.ko complex_b-$BITNESS.ko complex_e-$BITNESS.ko" ]

[ "`grep -w complex_b-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "complex_b-$BITNESS.ko: complex_e-$BITNESS.ko" ]

[ "`grep -w complex_c-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "complex_c-$BITNESS.ko: complex_b-$BITNESS.ko complex_e-$BITNESS.ko" ]

[ "`grep -w complex_d-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "complex_d-$BITNESS.ko: complex_b-$BITNESS.ko complex_e-$BITNESS.ko" ]

[ "`grep -w complex_e-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "complex_e-$BITNESS.ko:" ]

mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

# Synonyms
[ "`depmod -a`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

[ "`depmod -A`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

[ "`depmod -e -F /empty -A`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

[ "`depmod -e -F /empty -A $MODTEST_VERSION`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

# Combined should form stdout versions.
grep -vh '^#' $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.symbols.old > $MODULE_DIR/modules.all.old

# Stdout versions.
depmod -n | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -a -n | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -n -a $MODTEST_VERSION | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -e -F /empty -n -A $MODTEST_VERSION | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

done
done
