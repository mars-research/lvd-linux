#! /bin/sh
# Test --show-depends.

BITNESS=32

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS/normal/export_dep-$BITNESS.ko \
   tests/data/$BITNESS/normal/noexport_dep-$BITNESS.ko \
   tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko \
   tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko \
   tests/data/$BITNESS/normal/noexport_doubledep-$BITNESS.ko \
   $MODULE_DIR

# Now create modules.dep.bin
cat > $MODULE_DIR/modules.dep.temp <<EOF
noexport_nodep_$BITNESS noexport_nodep-$BITNESS.ko:
noexport_doubledep_$BITNESS noexport_doubledep-$BITNESS.ko: export_dep-$BITNESS.ko export_nodep-$BITNESS.ko
noexport_dep_$BITNESS noexport_dep-$BITNESS.ko: export_nodep-$BITNESS.ko
export_nodep_$BITNESS export_nodep-$BITNESS.ko:
export_dep_$BITNESS export_dep-$BITNESS.ko: export_nodep-$BITNESS.ko
EOF
# Build modules.dep.bin, which modprobe will prefer
modindex -o $MODULE_DIR/modules.dep.bin < $MODULE_DIR/modules.dep.temp

[ "`modprobe --show-depends noexport_nodep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko " ]
[ "`modprobe --show-depends export_nodep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko " ]
[ "`modprobe --show-depends noexport_dep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/noexport_dep-$BITNESS.ko " ]
[ "`modprobe --show-depends export_dep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko " ]
[ "`modprobe --show-depends noexport_doubledep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/noexport_doubledep-$BITNESS.ko " ]

# Nothing in stderr...
[ `wc -c < tests/tmp/stderr` = 0 ]
