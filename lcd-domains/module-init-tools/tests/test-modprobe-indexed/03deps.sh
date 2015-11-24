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

# Now create modules.dep.bin
cat > $MODULE_DIR/modules.dep.bin.temp <<EOF
noexport_nodep_$BITNESS noexport_nodep-$BITNESS.ko:
noexport_doubledep_$BITNESS noexport_doubledep-$BITNESS.ko: export_dep-$BITNESS.ko export_nodep-$BITNESS.ko
noexport_dep_$BITNESS noexport_dep-$BITNESS.ko: export_nodep-$BITNESS.ko
export_nodep_$BITNESS export_nodep-$BITNESS.ko:
export_dep_$BITNESS export_dep-$BITNESS.ko: export_nodep-$BITNESS.ko
EOF
modindex -o $MODULE_DIR/modules.dep.bin < $MODULE_DIR/modules.dep.bin.temp

# Insertion
SIZE_NOEXPORT_NODEP=`wc -c < tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko`
SIZE_EXPORT_NODEP=`wc -c < tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko`
SIZE_NOEXPORT_DEP=`wc -c < tests/data/$BITNESS/normal/noexport_dep-$BITNESS.ko`
SIZE_EXPORT_DEP=`wc -c < tests/data/$BITNESS/normal/export_dep-$BITNESS.ko`
SIZE_NOEXPORT_DOUBLEDEP=`wc -c < tests/data/$BITNESS/normal/noexport_doubledep-$BITNESS.ko`

# Empty /sys/module/
mkdir -p tests/tmp/sys/module

[ "`modprobe noexport_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_NOEXPORT_NODEP " ]
[ "`modprobe noexport_nodep-$BITNESS OPTIONS 2>&1`" = "INIT_MODULE: $SIZE_NOEXPORT_NODEP OPTIONS" ]

[ "`modprobe export_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP " ]
[ "`modprobe export_nodep-$BITNESS OPTIONS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP OPTIONS" ]

[ "`modprobe noexport_dep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP 
INIT_MODULE: $SIZE_NOEXPORT_DEP " ]
[ "`modprobe noexport_dep-$BITNESS OPTIONS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP 
INIT_MODULE: $SIZE_NOEXPORT_DEP OPTIONS" ]

[ "`modprobe export_dep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP 
INIT_MODULE: $SIZE_EXPORT_DEP " ]
[ "`modprobe export_dep-$BITNESS OPTIONS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP 
INIT_MODULE: $SIZE_EXPORT_DEP OPTIONS" ]

[ "`modprobe noexport_doubledep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP 
INIT_MODULE: $SIZE_EXPORT_DEP 
INIT_MODULE: $SIZE_NOEXPORT_DOUBLEDEP " ]
[ "`modprobe noexport_doubledep-$BITNESS OPTIONS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP 
INIT_MODULE: $SIZE_EXPORT_DEP 
INIT_MODULE: $SIZE_NOEXPORT_DOUBLEDEP OPTIONS" ]

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
[ "`modprobe -r noexport_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL " ]
[ "`modprobe -r export_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: export_nodep_$BITNESS EXCL " ]
[ "`modprobe -r noexport_dep-$BITNESS 2>&1`" = "DELETE_MODULE: noexport_dep_$BITNESS EXCL 
DELETE_MODULE: export_nodep_$BITNESS EXCL " ]
[ "`modprobe -r export_dep-$BITNESS 2>&1`" = "DELETE_MODULE: export_dep_$BITNESS EXCL 
DELETE_MODULE: export_nodep_$BITNESS EXCL " ]
[ "`modprobe -r noexport_doubledep-$BITNESS 2>&1`" = "DELETE_MODULE: noexport_doubledep_$BITNESS EXCL 
DELETE_MODULE: export_dep_$BITNESS EXCL 
DELETE_MODULE: export_nodep_$BITNESS EXCL " ]
