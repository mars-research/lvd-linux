#! /bin/sh
# Test for modules.alias usage.

BITNESS=32

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR/kernel
ln tests/data/$BITNESS/alias/alias-$BITNESS.ko \
   $MODULE_DIR/kernel

SIZE=`wc -c < tests/data/$BITNESS/alias/alias-$BITNESS.ko`

echo "alias_$BITNESS /lib/modules/$MODTEST_UNAME/kernel/alias-$BITNESS.ko:" > $MODULE_DIR/modules.dep.bin.temp
echo "foo /lib/modules/$MODTEST_UNAME/kernel/foo.ko:" >> $MODULE_DIR/modules.dep.bin.temp
modindex -o $MODULE_DIR/modules.dep.bin < $MODULE_DIR/modules.dep.bin.temp

rm -f $MODULE_DIR/modules.alias
rm -f $MODULE_DIR/modules.alias.bin
rm -f tests/tmp/etc/modprobe.d/modprobe.conf

cp tests/data/$BITNESS/complex/complex_a-$BITNESS.ko $MODULE_DIR/kernel/foo.ko
SIZE2=`wc -c < $MODULE_DIR/kernel/foo.ko`

# Shouldn't complain if can't open modules.alias
[ "`modprobe bar 2>&1`" = "FATAL: Module bar not found." ]

# Now, alias found in modules.alias works.
echo "bar alias_$BITNESS" > $MODULE_DIR/modules.alias.bin.temp
modindex -o $MODULE_DIR/modules.alias.bin < $MODULE_DIR/modules.alias.bin.temp
[ "`modprobe bar 2>&1`" = "INIT_MODULE: $SIZE " ]

# Normal alias should override it.
mkdir -p tests/tmp/etc/modprobe.d
echo 'alias bar foo' > tests/tmp/etc/modprobe.d/modprobe.conf
[ "`modprobe foo 2>&1`" = "INIT_MODULE: $SIZE2 " ]

# If there's a real module, alias from modules.alias must NOT override.
echo "foo alias_$BITNESS" > $MODULE_DIR/modules.alias.bin.temp
modindex -o $MODULE_DIR/modules.alias.bin < $MODULE_DIR/modules.alias.bin.temp
[ "`modprobe foo 2>&1`" = "INIT_MODULE: $SIZE2 " ]

# If there's an install command, modules.alias must not override.
echo 'install bar echo foo' > tests/tmp/etc/modprobe.d/modprobe.conf
[ "`modprobe bar 2>&1`" = "SYSTEM: echo foo" ]
echo 'remove bar echo foo remove' > tests/tmp/etc/modprobe.d/modprobe.conf
[ "`modprobe -r bar 2>&1`" = "SYSTEM: echo foo remove" ]

# Should gather up options from other alias name as well.
echo "bar alias_$BITNESS" > $MODULE_DIR/modules.alias.bin.temp
modindex -o $MODULE_DIR/modules.alias.bin < $MODULE_DIR/modules.alias.bin.temp
echo "options bar option1" > tests/tmp/etc/modprobe.d/modprobe.conf
echo "options alias_$BITNESS option2" >> tests/tmp/etc/modprobe.d/modprobe.conf
[ "`modprobe bar 2>&1`" = "INIT_MODULE: $SIZE option2 option1" ]

# Duplicated alias: both get probed (either order)
echo "bar foo" >> $MODULE_DIR/modules.alias.bin.temp
modindex -o $MODULE_DIR/modules.alias.bin < $MODULE_DIR/modules.alias.bin.temp
OUT="`modprobe bar 2>&1`"

[ "$OUT" = "INIT_MODULE: $SIZE option2 option1
INIT_MODULE: $SIZE2 option1" ] || [ "$OUT" = "INIT_MODULE: $SIZE2 option1
INIT_MODULE: $SIZE option2 option1" ]
