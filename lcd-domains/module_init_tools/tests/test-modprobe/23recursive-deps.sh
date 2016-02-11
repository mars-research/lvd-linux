#! /bin/sh

# If A depends on B and C, and B is installed, C still needs to be installed!
# (Bug in 3.0-pre6 & 3.0-pre7).

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
noexport_doubledep-$BITNESS.ko: export_dep-$BITNESS.ko export_nodep-$BITNESS.ko
EOF

# Now, export-dep is in /sys/module (actually this can't happen, since it
# depends on export_nodep, but this is to test).
mkdir -p tests/tmp/sys/module
mkdir -p tests/tmp/sys/module/export_dep_$BITNESS
echo "live" > tests/tmp/sys/module/export_dep_$BITNESS/initstate

SIZE_EXPORT_NODEP=`wc -c < tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko`
SIZE_NOEXPORT_DOUBLEDEP=`wc -c < tests/data/$BITNESS/normal/noexport_doubledep-$BITNESS.ko`

[ "`modprobe -v noexport_doubledep-$BITNESS 2>&1`" = "insmod /lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko 
INIT_MODULE: $SIZE_EXPORT_NODEP 
insmod /lib/modules/$MODTEST_UNAME/noexport_doubledep-$BITNESS.ko 
INIT_MODULE: $SIZE_NOEXPORT_DOUBLEDEP " ]
