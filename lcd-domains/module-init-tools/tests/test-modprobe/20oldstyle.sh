#! /bin/sh
# Test old-style module crap.

BITNESS=32

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR/drivers/type
ln tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko \
   $MODULE_DIR/drivers/type

mkdir -p $MODULE_DIR/other/type
ln tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko \
   $MODULE_DIR/other/type

# Set up modules.dep file.
echo "# A comment" > $MODULE_DIR/modules.dep
echo "/lib/modules/$MODTEST_UNAME/drivers/type/noexport_nodep-$BITNESS.ko:" >> $MODULE_DIR/modules.dep
echo "/lib/modules/$MODTEST_UNAME/other/type/export_nodep-$BITNESS.ko:" >> $MODULE_DIR/modules.dep

SIZE1=`wc -c < tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko`
SIZE2=`wc -c < tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko`

# -l lists all of them (either order)
[ "`modprobe -l 2>&1`" = "/lib/modules/$MODTEST_UNAME/drivers/type/noexport_nodep-$BITNESS.ko
/lib/modules/$MODTEST_UNAME/other/type/export_nodep-$BITNESS.ko" ] ||
[ "`modprobe -l 2>&1`" = "/lib/modules/$MODTEST_UNAME/other/type/export_nodep-$BITNESS.ko
/lib/modules/$MODTEST_UNAME/drivers/type/noexport_nodep-$BITNESS.ko" ]

# -l -t foo lists none of them.
[ "`modprobe -l -t foo 2>&1`" = "" ]

# -l -t type lists all of them (either order)
[ "`modprobe -l -t type 2>&1`" = "/lib/modules/$MODTEST_UNAME/drivers/type/noexport_nodep-$BITNESS.ko
/lib/modules/$MODTEST_UNAME/other/type/export_nodep-$BITNESS.ko" ] ||
[ "`modprobe -l -t type 2>&1`" = "/lib/modules/$MODTEST_UNAME/other/type/export_nodep-$BITNESS.ko
/lib/modules/$MODTEST_UNAME/drivers/type/noexport_nodep-$BITNESS.ko" ]

# -l -t drivers lists one.
[ "`modprobe -l -t drivers 2>&1`" = "/lib/modules/$MODTEST_UNAME/drivers/type/noexport_nodep-$BITNESS.ko" ]

# -l -t drivers/type lists one.
[ "`modprobe -l -t drivers/type 2>&1`" = "/lib/modules/$MODTEST_UNAME/drivers/type/noexport_nodep-$BITNESS.ko" ]

# -l -t other lists one.
[ "`modprobe -l -t other 2>&1`" = "/lib/modules/$MODTEST_UNAME/other/type/export_nodep-$BITNESS.ko" ]

# -l -t other/type lists one.
[ "`modprobe -l -t other/type 2>&1`" = "/lib/modules/$MODTEST_UNAME/other/type/export_nodep-$BITNESS.ko" ]

# Wildcard works.
[ "`modprobe -l -t type 'noexport-nodep*' 2>&1`" = "/lib/modules/$MODTEST_UNAME/drivers/type/noexport_nodep-$BITNESS.ko" ]

# -t type without -l not supported
modprobe -t type 2>&1 | grep -q Usage
modprobe -a -t type 2>&1 | grep -q Usage

# -a with one arg succeeds.
[ "`modprobe -a noexport_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE1 " ]
# ... even with - and _ confused.
[ "`modprobe -a noexport-nodep_$BITNESS 2>&1`" = "INIT_MODULE: $SIZE1 " ]

# With two args succeeds.
[ "`modprobe -a noexport_nodep-$BITNESS export_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE1 
INIT_MODULE: $SIZE2 " ]

# Does second even if first screws up.
[ "`modprobe -a crap export_nodep-$BITNESS 2>&1`" = "WARNING: Module crap not found.
INIT_MODULE: $SIZE2 " ]
