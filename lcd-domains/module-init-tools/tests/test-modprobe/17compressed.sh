#! /bin/sh
# Test compressed modules.

[ -n "$CONFIG_HAVE_ZLIB" ] || exit 0

for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# Copy modules instead of linking, so we can compress them
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
cp tests/data/$BITNESS$ENDIAN/normal/noexport_nodep-$BITNESS.ko \
   $MODULE_DIR

gzip $MODULE_DIR/noexport_nodep-$BITNESS.ko

# Set up modules.dep file.
echo "# A comment" > $MODULE_DIR/modules.dep
echo "/lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko.gz:" >> $MODULE_DIR/modules.dep

SIZE=`wc -c < tests/data/$BITNESS$ENDIAN/normal/noexport_nodep-$BITNESS.ko`

# No args
[ "`modprobe noexport_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE " ]

# With args
[ "`modprobe noexport_nodep-$BITNESS foo=\"bar baz\" 2>&1`" = "INIT_MODULE: $SIZE foo=\"bar baz\"" ]

# Check underscore equivalence.
[ "`modprobe noexport-nodep-$BITNESS foo=\"bar baz\" 2>&1`" = "INIT_MODULE: $SIZE foo=\"bar baz\"" ]
[ "`modprobe noexport-nodep_$BITNESS foo=\"bar baz\" 2>&1`" = "INIT_MODULE: $SIZE foo=\"bar baz\"" ]
[ "`modprobe noexport_nodep_$BITNESS foo=\"bar baz\" 2>&1`" = "INIT_MODULE: $SIZE foo=\"bar baz\"" ]

done
done
