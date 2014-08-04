#! /bin/sh
# Test for blacklist usage.

BITNESS=32

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR/kernel
ln tests/data/$BITNESS/alias/alias-$BITNESS.ko \
   $MODULE_DIR/kernel

SIZE=`wc -c < tests/data/$BITNESS/alias/alias-$BITNESS.ko`

echo "/lib/modules/$MODTEST_UNAME/kernel/alias-$BITNESS.ko:" > $MODULE_DIR/modules.dep
echo "/lib/modules/$MODTEST_UNAME/kernel/foo.ko:" >> $MODULE_DIR/modules.dep
cp tests/data/$BITNESS/complex/complex_a-$BITNESS.ko $MODULE_DIR/kernel/foo.ko
SIZE2=`wc -c < $MODULE_DIR/kernel/foo.ko`

# First, alias found in modules.alias works.
echo "alias bar alias-$BITNESS" > $MODULE_DIR/modules.alias
[ "`modprobe bar 2>&1`" = "INIT_MODULE: $SIZE " ]

# Blacklist makes it fail.
mkdir -p tests/tmp/etc/modprobe.d
echo "blacklist alias-$BITNESS" > tests/tmp/etc/modprobe.d/modprobe.conf
[ "`modprobe bar 2>&1`" = "FATAL: Module bar not found." ]

# Blacklist doesn't effect other aliases.
echo "alias bar foo" >> $MODULE_DIR/modules.alias
[ "`modprobe bar 2>&1`" = "INIT_MODULE: $SIZE2 " ]

# Blacklist both.
echo "blacklist foo" >> tests/tmp/etc/modprobe.d/modprobe.conf
[ "`modprobe bar 2>&1`" = "FATAL: Module bar not found." ]

# Remove blacklist, all works.
rm -f tests/tmp/etc/modprobe.d/modprobe.conf
RESULT="`modprobe bar 2>&1`"
[ "$RESULT" = "INIT_MODULE: $SIZE 
INIT_MODULE: $SIZE2 " ] || [ "$RESULT" = "INIT_MODULE: $SIZE2
INIT_MODULE: $SIZE " ]
