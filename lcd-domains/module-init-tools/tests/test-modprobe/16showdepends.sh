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

# Now create modules.dep
cat > $MODULE_DIR/modules.dep <<EOF
# Should handle comments.
/lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko:
/lib/modules/$MODTEST_UNAME/noexport_doubledep-$BITNESS.ko: /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko
/lib/modules/$MODTEST_UNAME/noexport_dep-$BITNESS.ko: /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko
/lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko:
/lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko: /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko
EOF

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

# All in /sys/module/; should make no difference.
mkdir -p tests/tmp/sys/module
mkdir -p tests/tmp/sys/module/noexport_nodep_$BITNESS
mkdir -p tests/tmp/sys/module/export_nodep_$BITNESS
echo live > tests/tmp/sys/module/noexport_nodep_$BITNESS/initstate
echo live > tests/tmp/sys/module/export_nodep_$BITNESS/initstate

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

# Module commands printed, ignored.
mkdir -p tests/tmp/etc/modprobe.d
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
install noexport_nodep-$BITNESS echo noexport_nodep-$BITNESS
install export_nodep-$BITNESS echo export_nodep-$BITNESS
install noexport_dep-$BITNESS echo noexport_dep-$BITNESS
install export_dep-$BITNESS echo export_dep-$BITNESS
install noexport_doubledep-$BITNESS echo noexport_doubledep-$BITNESS
EOF

[ "`modprobe --show-depends noexport_nodep-$BITNESS 2>>tests/tmp/stderr`" = "install echo noexport_nodep-$BITNESS" ]
[ "`modprobe --show-depends export_nodep-$BITNESS 2>>tests/tmp/stderr`" = "install echo export_nodep-$BITNESS" ]
[ "`modprobe --show-depends noexport_dep-$BITNESS 2>>tests/tmp/stderr`" = "install echo export_nodep-$BITNESS
install echo noexport_dep-$BITNESS" ]
[ "`modprobe --show-depends export_dep-$BITNESS 2>>tests/tmp/stderr`" = "install echo export_nodep-$BITNESS
install echo export_dep-$BITNESS" ]
[ "`modprobe --show-depends noexport_doubledep-$BITNESS 2>>tests/tmp/stderr`" = "install echo export_nodep-$BITNESS
install echo export_dep-$BITNESS
install echo noexport_doubledep-$BITNESS" ]
# Nothing in stderr...
[ `wc -c < tests/tmp/stderr` = 0 ]

# Module options printed.
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
options noexport_nodep-$BITNESS opt1
options export_nodep-$BITNESS opt2
options noexport_dep-$BITNESS opt3
options export_dep-$BITNESS opt4
options noexport_doubledep-$BITNESS opt5
EOF

[ "`modprobe --show-depends noexport_nodep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko opt1" ]
[ "`modprobe --show-depends export_nodep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko opt2" ]
[ "`modprobe --show-depends noexport_dep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko opt2
insmod /lib/modules/$MODTEST_UNAME/noexport_dep-$BITNESS.ko opt3" ]
[ "`modprobe --show-depends export_dep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko opt2
insmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko opt4" ]
[ "`modprobe --show-depends noexport_doubledep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko opt2
insmod /lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko opt4
insmod /lib/modules/$MODTEST_UNAME/noexport_doubledep-$BITNESS.ko opt5" ]
# Nothing in stderr...
[ `wc -c < tests/tmp/stderr` = 0 ]

# Via aliases works.
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
options noexport_nodep-$BITNESS opt1
alias foo noexport_nodep-$BITNESS
options foo fooopt
EOF

[ "`modprobe --show-depends foo`" = "insmod /lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko opt1 fooopt" ]
# Nothing in stderr...
[ `wc -c < tests/tmp/stderr` = 0 ]
