#! /bin/sh

# Test handling of /sys/module/.

BITNESS=32

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko \
   tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko \
   $MODULE_DIR

# Now create modules.dep
cat > $MODULE_DIR/modules.dep <<EOF
noexport_nodep-$BITNESS.ko:
export_nodep-$BITNESS.ko:
EOF

SIZE_NOEXPORT_NODEP=`wc -c < tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko`
SIZE_EXPORT_NODEP=`wc -c < tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko`

# If it can't open /sys/module, it should try anyway.
[ "`modprobe noexport_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_NOEXPORT_NODEP " ]
[ "`modprobe export_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP " ]

[ "`modprobe -r noexport_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL " ]
[ "`modprobe -r export_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: export_nodep_$BITNESS EXCL " ]

# If it doesn't exist in /sys/module/, remove should succeed.
mkdir -p tests/tmp/sys/module

[ "`modprobe noexport_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_NOEXPORT_NODEP " ]
[ "`modprobe export_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP " ]
[ "`modprobe -r -v noexport_nodep-$BITNESS 2>&1; echo $?`" = "0" ]
[ "`modprobe -r -v export_nodep-$BITNESS 2>&1; echo $?`" = "0" ]
[ "`modprobe -r -v noexport-nodep-$BITNESS 2>&1; echo $?`" = "0" ]
[ "`modprobe -r -v export-nodep-$BITNESS 2>&1; echo $?`" = "0" ]
# ... unless --first-time is specified (won't print status due to set -e).
[ "`modprobe --first-time -r noexport_nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS is not in kernel." ]
[ "`modprobe --first-time -r export_nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS is not in kernel." ]
[ "`modprobe --first-time -r noexport-nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS is not in kernel." ]
[ "`modprobe --first-time -r export-nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS is not in kernel." ]

# Now make a fake /sys/module structure for the test
# Note: no refcnt files (no unload support)
mkdir -p tests/tmp/sys/module
mkdir -p tests/tmp/sys/module/noexport_nodep_$BITNESS
mkdir -p tests/tmp/sys/module/export_nodep_$BITNESS
echo live > tests/tmp/sys/module/noexport_nodep_$BITNESS/initstate
echo live > tests/tmp/sys/module/export_nodep_$BITNESS/initstate

# If it does exist, insertion should "succeed".
[ "`modprobe -v noexport_nodep-$BITNESS 2>&1; echo $?`" = "0" ]
[ "`modprobe -v export_nodep-$BITNESS 2>&1; echo $?`" = "0" ]
[ "`modprobe -v noexport-nodep-$BITNESS 2>&1; echo $?`" = "0" ]
[ "`modprobe -v export-nodep-$BITNESS 2>&1; echo $?`" = "0" ]
# .. unless --first-time is specified.
[ "`modprobe --first-time noexport_nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time export_nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time noexport-nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time export-nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS already in kernel." ]

# Removal should try (despite lack of unload support)
[ "`modprobe -r noexport_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL " ]
[ "`modprobe -r export_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: export_nodep_$BITNESS EXCL " ]

# Note: refcnt files (unload support)
rm -rf tests/tmp/sys/module
mkdir -p tests/tmp/sys/module
mkdir -p tests/tmp/sys/module/noexport_nodep_$BITNESS
mkdir -p tests/tmp/sys/module/export_nodep_$BITNESS
echo live > tests/tmp/sys/module/noexport_nodep_$BITNESS/initstate
echo live > tests/tmp/sys/module/export_nodep_$BITNESS/initstate
echo 1 > tests/tmp/sys/module/export_nodep_$BITNESS/refcnt
echo 1 > tests/tmp/sys/module/noexport_nodep_$BITNESS/refcnt

[ "`modprobe --first-time noexport_nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time export_nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time noexport-nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time export-nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS already in kernel." ]

[ "`modprobe -r noexport_nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS is in use." ]
[ "`modprobe -r export_nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS is in use." ]
[ "`modprobe -r noexport-nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS is in use." ]
[ "`modprobe -r export-nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS is in use." ]
