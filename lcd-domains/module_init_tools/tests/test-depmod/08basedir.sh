#! /bin/sh
# Simple tests of generation of 32-bit and 64-bit modules.dep with basedir

for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/BASEDIR/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS$ENDIAN/normal/export_dep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/noexport_dep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/export_nodep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/noexport_nodep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/noexport_doubledep-$BITNESS.ko \
   $MODULE_DIR

touch tests/tmp/empty

# Expect no output.
[ "`depmod -b /BASEDIR 2>&1`" = "" ]

# Check results: expect 5 lines
[ `grep -vc '^#' < $MODULE_DIR/modules.dep` = 5 ]

[ "`grep -w export_dep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "export_dep-$BITNESS.ko: export_nodep-$BITNESS.ko" ]
[ "`grep -w noexport_dep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "noexport_dep-$BITNESS.ko: export_nodep-$BITNESS.ko" ]
[ "`grep -w export_nodep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "export_nodep-$BITNESS.ko:" ]
[ "`grep -w noexport_nodep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "noexport_nodep-$BITNESS.ko:" ]
[ "`grep -w noexport_doubledep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "noexport_doubledep-$BITNESS.ko: export_dep-$BITNESS.ko export_nodep-$BITNESS.ko" ]

mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

# Synonyms
[ "`depmod -b /BASEDIR  -a`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

[ "`depmod -b /BASEDIR -A`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

[ "`depmod -b /BASEDIR -e -F /empty -A`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

[ "`depmod -b /BASEDIR -e -F /empty -A $MODTEST_VERSION`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

[ "`depmod --basedir /BASEDIR -e -F /empty -A $MODTEST_VERSION 2>&1`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

[ "`depmod --basedir=/BASEDIR -e -F /empty -A $MODTEST_VERSION`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old

# Combined should form stdout versions.
grep -vh '^#' $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.symbols > $MODULE_DIR/modules.all.old

# Stdout versions.
depmod -b /BASEDIR -n | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -b /BASEDIR -a -n | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -b /BASEDIR -n -a $MODTEST_VERSION | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -b /BASEDIR -e -F /empty -n -A $MODTEST_VERSION | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

done
done
