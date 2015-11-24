#! /bin/sh
# Simple tests of generation of 32-bit and 64-bit modules.dep

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

# Expect no output.
[ "`depmod 2>&1`" = "" ]

# Check modules.dep results: expect 5 lines
[ "`grep -vc '^#' < $MODULE_DIR/modules.dep`" = "5" ]

[ "`grep -w export_dep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "export_dep-$BITNESS.ko: export_nodep-$BITNESS.ko" ]
[ "`grep -w noexport_dep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "noexport_dep-$BITNESS.ko: export_nodep-$BITNESS.ko" ]
[ "`grep -w export_nodep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "export_nodep-$BITNESS.ko:" ]
[ "`grep -w noexport_nodep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "noexport_nodep-$BITNESS.ko:" ]
[ "`grep -w noexport_doubledep-$BITNESS.ko: $MODULE_DIR/modules.dep`" = "noexport_doubledep-$BITNESS.ko: export_dep-$BITNESS.ko export_nodep-$BITNESS.ko" ]

# Check modules.symbols results: expect 3 lines
[ "`grep -vc '^#' < $MODULE_DIR/modules.symbols`" = "3" ]

[ "`grep -w symbol:exported1 $MODULE_DIR/modules.symbols`" = "alias symbol:exported1 export_nodep_$BITNESS" ]
[ "`grep -w symbol:exported2 $MODULE_DIR/modules.symbols`" = "alias symbol:exported2 export_nodep_$BITNESS" ]
[ "`grep -w symbol:exported3 $MODULE_DIR/modules.symbols`" = "alias symbol:exported3 export_dep_$BITNESS" ]

mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

touch tests/tmp/empty

# Synonyms
[ "`depmod $MODTEST_UNAME`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod -a`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod -a $MODTEST_UNAME`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod -A`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod -A $MODTEST_UNAME`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod -e -F /empty -A`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod -e -F /empty -A $MODTEST_VERSION`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod --all`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod --quick`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod -e -F /empty --quick`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod -e -F /empty --quick $MODTEST_VERSION`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod --errsyms -F /empty --quick`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

[ "`depmod --errsyms -F /empty --quick $MODTEST_VERSION`" = "" ]
diff -u $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.dep >/dev/null
mv $MODULE_DIR/modules.dep $MODULE_DIR/modules.dep.old
diff -u $MODULE_DIR/modules.symbols.old $MODULE_DIR/modules.symbols >/dev/null
mv $MODULE_DIR/modules.symbols $MODULE_DIR/modules.symbols.old

# Combined should form stdout versions.
grep -vh '^#' $MODULE_DIR/modules.dep.old $MODULE_DIR/modules.symbols.old > $MODULE_DIR/modules.all.old

# Stdout versions.
depmod -n | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all >/dev/null
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -a -n | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all >/dev/null
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -n -a $MODTEST_VERSION | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all >/dev/null
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

depmod -e -n -F /empty -A $MODTEST_VERSION | grep -v '^#' > $MODULE_DIR/modules.all
diff -u $MODULE_DIR/modules.all.old $MODULE_DIR/modules.all >/dev/null
mv $MODULE_DIR/modules.all $MODULE_DIR/modules.all.old

done
done
