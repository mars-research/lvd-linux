#! /bin/sh
# Test depmod with specific names on the cmdline (bug report from Andrew Snare)

for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS$ENDIAN/normal/export_dep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/noexport_dep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/export_nodep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/noexport_nodep-$BITNESS.ko \
   tests/data/$BITNESS$ENDIAN/normal/noexport_doubledep-$BITNESS.ko \
   $MODULE_DIR

touch tests/tmp/empty

ALL="/lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko /lib/modules/$MODTEST_UNAME/noexport_dep-$BITNESS.ko /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko /lib/modules/$MODTEST_UNAME/noexport_doubledep-$BITNESS.ko"

# Expect no output.
[ "`depmod $ALL 2>&1`" = "" ]

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

mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

# Synonyms
[ "`depmod $MODTEST_UNAME $ALL`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod -e -F /empty $ALL`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod -e -F /empty $MODTEST_VERSION $ALL`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod --errsyms -F /empty $ALL`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod --errsyms -F /empty $MODTEST_VERSION $ALL`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

# Combined should form stdout versions.
grep -vh '^#' $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.symbols.old > $MODULE_DIR/modules.all.old

# Stdout versions.
depmod -n $ALL | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all >/dev/null
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -n $MODTEST_VERSION $ALL | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all >/dev/null
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -e -F /empty -n $MODTEST_VERSION $ALL | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all >/dev/null
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

done
done

