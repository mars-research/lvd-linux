#! /bin/sh
# modprobe -r supports multiple module names now.

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

# Now create modules.dep
cat > $MODULE_DIR/modules.dep <<EOF
# Should handle comments.
noexport_nodep-$BITNESS.ko:
noexport_doubledep-$BITNESS.ko: export_dep-$BITNESS.ko export_nodep-$BITNESS.ko
noexport_dep-$BITNESS.ko: export_nodep-$BITNESS.ko
export_nodep-$BITNESS.ko:
export_dep-$BITNESS.ko: export_nodep-$BITNESS.ko
EOF

# Now make a fake /sys/module structure for the test
mkdir -p tests/tmp/sys/module
mkdir -p tests/tmp/sys/module/noexport_nodep_$BITNESS
mkdir -p tests/tmp/sys/module/noexport_dep_$BITNESS
mkdir -p tests/tmp/sys/module/export_nodep_$BITNESS
mkdir -p tests/tmp/sys/module/export_dep_$BITNESS
mkdir -p tests/tmp/sys/module/noexport_doubledep_$BITNESS
echo live > tests/tmp/sys/module/noexport_nodep_$BITNESS/initstate
echo live > tests/tmp/sys/module/noexport_dep_$BITNESS/initstate
echo live > tests/tmp/sys/module/export_nodep_$BITNESS/initstate
echo live > tests/tmp/sys/module/export_dep_$BITNESS/initstate
echo live > tests/tmp/sys/module/noexport_doubledep_$BITNESS/initstate

# Removal
[ "`modprobe -r noexport_nodep-$BITNESS export_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL 
DELETE_MODULE: export_nodep_$BITNESS EXCL " ]
