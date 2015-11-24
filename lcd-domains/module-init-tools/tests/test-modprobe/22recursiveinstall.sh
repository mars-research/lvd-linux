#! /bin/sh

BITNESS=32

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko \
   tests/data/$BITNESS/normal/noexport_dep-$BITNESS.ko \
   $MODULE_DIR

# Now create modules.dep
cat > $MODULE_DIR/modules.dep <<EOF
noexport_dep-$BITNESS.ko: export_nodep-$BITNESS.ko
export_nodep-$BITNESS.ko:
EOF

# Insertion
SIZE_EXPORT_NODEP=`wc -c < tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko`
SIZE_NOEXPORT_DEP=`wc -c < tests/data/$BITNESS/normal/noexport_dep-$BITNESS.ko`

# Empty /sys/module/
mkdir -p tests/tmp/sys/module

# Check it pulls in both.
[ "`modprobe noexport_dep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP 
INIT_MODULE: $SIZE_NOEXPORT_DEP " ]

# Check it's happy if we tell it dep is already installed
mkdir -p tests/tmp/sys/module
mkdir -p tests/tmp/sys/module/export_nodep_$BITNESS
echo "live" >tests/tmp/sys/module/export_nodep_$BITNESS/initstate

[ "`modprobe noexport_dep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_NOEXPORT_DEP " ]

# If there's an install command, it will be done.
# Clean up sysfs (so we don't think it's loaded)
rm -rf tests/tmp/sys/module/*

mkdir -p tests/tmp/etc/modprobe.d
echo "install export_nodep-$BITNESS COMMAND" > tests/tmp/etc/modprobe.d/modprobe.conf
[ "`modprobe noexport_dep-$BITNESS 2>&1`" = "SYSTEM: COMMAND
INIT_MODULE: $SIZE_NOEXPORT_DEP " ]

# If it's in /sys/module, install command WONT be done.
mkdir -p tests/tmp/sys/module
mkdir -p tests/tmp/sys/module/export_nodep_$BITNESS
echo "live" >tests/tmp/sys/module/export_nodep_$BITNESS/initstate

[ "`modprobe noexport_dep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_NOEXPORT_DEP " ]

# Do dependencies even if install command.
# clean up sysfs (so we don't think it's loaded)
rm -rf tests/tmp/sys/module/*

echo "install noexport_dep-$BITNESS COMMAND" > tests/tmp/etc/modprobe.d/modprobe.conf

[ "`modprobe noexport_dep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP 
SYSTEM: COMMAND" ]
