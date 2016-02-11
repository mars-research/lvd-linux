#! /bin/sh
# Check that -A works correctly.

for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# Copy modules instead of linking, so we can play with their timestamps.
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
cp tests/data/$BITNESS$ENDIAN/normal/export_dep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/noexport_dep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/export_nodep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/noexport_nodep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/noexport_doubledep-$BITNESS.ko \
   $MODULE_DIR

# First time, there is no modules.dep, so it will be generated.
# Expect no output.
[ "`depmod -A 2>&1`" = "" ]

# Check modules.dep results: expect 5 lines
[ `grep -vc '^#' < $MODULE_DIR/modules.dep` = 5 ]

[ "`grep -w export_dep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "export_dep-$BITNESS.ko: export_nodep-$BITNESS.ko" ]
[ "`grep -w noexport_dep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "noexport_dep-$BITNESS.ko: export_nodep-$BITNESS.ko" ]
[ "`grep -w export_nodep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "export_nodep-$BITNESS.ko:" ]
[ "`grep -w noexport_nodep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "noexport_nodep-$BITNESS.ko:" ]
[ "`grep -w noexport_doubledep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "noexport_doubledep-$BITNESS.ko: export_dep-$BITNESS.ko export_nodep-$BITNESS.ko" ]

# Check modules.symbols results: expect 3 lines
[ `grep -vc '^#' < $MODULE_DIR/modules.symbols` = 3 ]

[ "`grep -w symbol:exported1 $MODULE_DIR/modules.symbols`" = "alias symbol:exported1 export_nodep_$BITNESS" ]
[ "`grep -w symbol:exported2 $MODULE_DIR/modules.symbols`" = "alias symbol:exported2 export_nodep_$BITNESS" ]
[ "`grep -w symbol:exported3 $MODULE_DIR/modules.symbols`" = "alias symbol:exported3 export_dep_$BITNESS" ]

cp $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
cp $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

# Now, second run should do nothing.
cp /dev/null $MODULE_DIR/modules.dep
cp /dev/null $MODULE_DIR/modules.symbols
[ "`depmod -A 2>&1`" = "" ]
diff -u /dev/null $MODULE_DIR/modules.dep
diff -u /dev/null $MODULE_DIR/modules.symbols

# Touch a directory, nothing.
sleep 1
touch $MODULE_DIR

[ "`depmod -A 2>&1`" = "" ]
diff -u /dev/null $MODULE_DIR/modules.dep
diff -u /dev/null $MODULE_DIR/modules.symbols

# Touch a non-module, nothing.
echo "Test" > $MODULE_DIR/README
[ "`depmod -A 2>&1`" = "" ]
diff -u /dev/null $MODULE_DIR/modules.dep
diff -u /dev/null $MODULE_DIR/modules.symbols

# Touch a module, wham.
touch $MODULE_DIR/noexport_doubledep-$BITNESS.ko
[ "`depmod -A 2>&1`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols

# Deleting a module does nothing.
cp /dev/null $MODULE_DIR/modules.dep
cp /dev/null $MODULE_DIR/modules.symbols
rm $MODULE_DIR/noexport_doubledep-$BITNESS.ko
[ "`depmod -A 2>&1`" = "" ]
diff -u /dev/null $MODULE_DIR/modules.dep
diff -u /dev/null $MODULE_DIR/modules.symbols

done
done
