#! /bin/sh
# Test module dependencies.

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

[ "`modprobe -v -n noexport_nodep-$BITNESS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko " ]
[ "`modprobe -v -n noexport_nodep-$BITNESS OPTIONS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko OPTIONS" ]

[ "`modprobe -v -n export_nodep-$BITNESS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko " ]
[ "`modprobe -v -n export_nodep-$BITNESS OPTIONS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko OPTIONS" ]

[ "`modprobe -v -n noexport_dep-$BITNESS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/noexport_dep-$BITNESS.ko " ]
[ "`modprobe -v -n noexport_dep-$BITNESS OPTIONS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/noexport_dep-$BITNESS.ko OPTIONS" ]

[ "`modprobe -v -n export_dep-$BITNESS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko " ]
[ "`modprobe -v -n export_dep-$BITNESS OPTIONS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko OPTIONS" ]

[ "`modprobe -v -n noexport_doubledep-$BITNESS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/noexport_doubledep-$BITNESS.ko " ]
[ "`modprobe -v -n noexport_doubledep-$BITNESS OPTIONS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/noexport_doubledep-$BITNESS.ko OPTIONS" ]

# Test alias and options resolution
mkdir -p tests/tmp/etc/modprobe.d
echo "alias foo noexport_doubledep-$BITNESS" > tests/tmp/etc/modprobe.d/modprobe.conf
echo "options noexport_doubledep-$BITNESS OPTIONS" >> tests/tmp/etc/modprobe.d/modprobe.conf
[ "`modprobe -v -n foo 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko 
insmod /lib/modules/$MODTEST_UNAME/noexport_doubledep-$BITNESS.ko OPTIONS" ]

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
[ "`modprobe -v -n -r noexport_nodep-$BITNESS 2>&1`" = "rmmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko" ]
[ "`modprobe -v -n -r export_nodep-$BITNESS 2>&1`" = "rmmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko" ]
[ "`modprobe -v -n -r noexport_dep-$BITNESS 2>&1`" = "rmmod /lib/modules/$MODTEST_UNAME/noexport_dep-$BITNESS.ko
rmmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko" ]
[ "`modprobe -v -n -r export_dep-$BITNESS 2>&1`" = "rmmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko
rmmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko" ]
[ "`modprobe -v -n -r noexport_doubledep-$BITNESS 2>&1`" = "rmmod /lib/modules/$MODTEST_UNAME/noexport_doubledep-$BITNESS.ko
rmmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko
rmmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko" ]
