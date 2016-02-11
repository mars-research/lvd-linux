#! /bin/sh

BITNESS=32

rm -rf tests/tmp/*

MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko \
   $MODULE_DIR

# Now create modules.dep and modules.conf
echo /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko: > $MODULE_DIR/modules.dep

mkdir -p tests/tmp/etc/modprobe.d
echo install foo modprobe noexport_nodep-$BITNESS > tests/tmp/etc/modprobe.d/modprobe.conf
echo install foo modprobe bar > tests/tmp/etc/modprobe2.conf
echo install bar echo DOING BAR >> tests/tmp/etc/modprobe2.conf

SIZE_NOEXPORT_NODEP=`wc -c < tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko`

# Test normal args, then in env.
[ "`modprobe -v noexport_nodep-$BITNESS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko 
INIT_MODULE: $SIZE_NOEXPORT_NODEP " ]
[ "`MODPROBE_OPTIONS=-v modprobe noexport_nodep-$BITNESS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko 
INIT_MODULE: $SIZE_NOEXPORT_NODEP " ]

[ "`modprobe -q noexport_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_NOEXPORT_NODEP " ]
[ "`MODPROBE_OPTIONS=-q modprobe noexport_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_NOEXPORT_NODEP " ]

[ "`modprobe -n noexport_nodep-$BITNESS 2>&1`" = "" ]
[ "`MODPROBE_OPTIONS=-n modprobe noexport_nodep-$BITNESS 2>&1`" = "" ]

[ "`modprobe -n -v noexport_nodep-$BITNESS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko " ]
[ "`MODPROBE_OPTIONS="-n -v" modprobe noexport_nodep-$BITNESS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko " ]

# Test argument inheritence.
MODTEST_DO_SYSTEM=1
export MODTEST_DO_SYSTEM

[ "`modprobe -v foo 2>&1`" = "install modprobe noexport_nodep-$BITNESS
insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko 
INIT_MODULE: $SIZE_NOEXPORT_NODEP " ]
[ "`MODPROBE_OPTIONS=-v modprobe foo 2>&1`" = "install modprobe noexport_nodep-$BITNESS
insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko 
INIT_MODULE: $SIZE_NOEXPORT_NODEP " ]

[ "`modprobe -C /etc/modprobe2.conf foo 2>&1`" = "DOING BAR" ]
[ "`MODPROBE_OPTIONS='-C /etc/modprobe2.conf' modprobe foo 2>&1`" = "DOING BAR" ]

[ "`modprobe -C /etc/modprobe2.conf -v foo 2>&1`" = "install modprobe bar
install echo DOING BAR
DOING BAR" ]
[ "`MODPROBE_OPTIONS='-C /etc/modprobe2.conf' modprobe -v foo 2>&1`" = "install modprobe bar
install echo DOING BAR
DOING BAR" ]
