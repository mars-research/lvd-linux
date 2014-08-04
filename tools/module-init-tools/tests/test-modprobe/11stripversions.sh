#! /bin/sh
# Test the version removal code

section_attributes()
{
    readelf -W -S "$1" | cut -d\] -f2- | awk '{print $1 " " $7}' | grep -w -- "$2"
}

for ENDIAN in $TEST_ENDIAN; do
for BITNESS in $TEST_BITS; do

rm -rf tests/tmp/*

# We need to dump the module to make sure the name has changed.
MODTEST_DUMP_INIT=1
export MODTEST_DUMP_INIT

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS$ENDIAN/rename/rename-version-$BITNESS.ko \
   $MODULE_DIR

# Set up modules.dep file (neither has dependencies).
echo "# A comment" > $MODULE_DIR/modules.dep
echo "/lib/modules/$MODTEST_UNAME/rename-version-$BITNESS.ko:" >> $MODULE_DIR/modules.dep

# Check it without removing.
[ "`modprobe rename-version-$BITNESS 2> tests/tmp/out`" = "" ]
[ "`section_attributes tests/tmp/out __versions`" = "__versions A" ]
[ "`section_attributes tests/tmp/out __vermagic`" = "__vermagic A" ]

# Now remove them (turns off ALLOC bit)
[ "`modprobe --force rename-version-$BITNESS 2> tests/tmp/out`" = "" ]
[ "`section_attributes tests/tmp/out __versions`" = "__versions 0" ]
[ "`section_attributes tests/tmp/out __vermagic`" = "__vermagic 0" ]

# Now remove them individually instead.
[ "`modprobe --force-vermagic rename-version-$BITNESS 2> tests/tmp/out`" = "" ]
[ "`section_attributes tests/tmp/out __versions`" = "__versions A" ]
[ "`section_attributes tests/tmp/out __vermagic`" = "__vermagic 0" ]
[ "`modprobe --force-modversion rename-version-$BITNESS 2> tests/tmp/out`" = "" ]
[ "`section_attributes tests/tmp/out __versions`" = "__versions 0" ]
[ "`section_attributes tests/tmp/out __vermagic`" = "__vermagic A" ]

done
done

exit 0
