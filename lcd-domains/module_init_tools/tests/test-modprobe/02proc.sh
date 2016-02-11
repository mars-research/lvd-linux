#! /bin/sh

# Test handling of /proc/modules.

BITNESS=32

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko \
   tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko \
   $MODULE_DIR

mkdir tests/tmp/proc

# Now create modules.dep
cat > tests/tmp/lib/modules/$MODTEST_UNAME/modules.dep <<EOF
noexport_nodep-$BITNESS.ko:
export_nodep-$BITNESS.ko:
EOF

SIZE_NOEXPORT_NODEP=$(echo `wc -c < tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko`)
SIZE_EXPORT_NODEP=$(echo `wc -c < tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko`)

# If it can't open /proc/modules, it should try anyway.
rm -f tests/tmp/proc/modules

[ "`modprobe noexport_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_NOEXPORT_NODEP " ]
[ "`modprobe export_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE_EXPORT_NODEP " ]

[ "`modprobe -r noexport_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL " ]
[ "`modprobe -r export_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: export_nodep_$BITNESS EXCL " ]

# If it doesn't exist in /proc/modules, remove should succeed.
touch tests/tmp/proc/modules
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

# Old-style /proc (no unload support).
echo "noexport_nodep_$BITNESS $SIZE_NOEXPORT_NODEP" > tests/tmp/proc/modules
echo "export_nodep_$BITNESS $SIZE_EXPORT_NODEP" >> tests/tmp/proc/modules

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

# Removal should still try.
[ "`modprobe -r noexport_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL " ]
[ "`modprobe -r export_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: export_nodep_$BITNESS EXCL " ]

# New-style /proc without unload support
echo "noexport_nodep_$BITNESS $SIZE_NOEXPORT_NODEP - -" > tests/tmp/proc/modules
echo "export_nodep_$BITNESS $SIZE_EXPORT_NODEP - -" >> tests/tmp/proc/modules

[ "`modprobe --first-time noexport_nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time export_nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time noexport-nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time export-nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS already in kernel." ]
# Removal should still try.
[ "`modprobe -r noexport_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL " ]
[ "`modprobe -r export_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: export_nodep_$BITNESS EXCL " ]

# Should fail if refcounts non-zero.  Old-style
echo "noexport_nodep_$BITNESS $SIZE_NOEXPORT_NODEP 1" > tests/tmp/proc/modules
echo "export_nodep_$BITNESS $SIZE_EXPORT_NODEP 1" >> tests/tmp/proc/modules

[ "`modprobe -r noexport_nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS is in use." ]
[ "`modprobe -r export_nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS is in use." ]
[ "`modprobe -r noexport-nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS is in use." ]
[ "`modprobe -r export-nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS is in use." ]

# New-style /proc
echo "noexport_nodep_$BITNESS $SIZE_NOEXPORT_NODEP 1 -" > tests/tmp/proc/modules
echo "export_nodep_$BITNESS $SIZE_EXPORT_NODEP 1 something_else, extrafield" >> tests/tmp/proc/modules

[ "`modprobe --first-time noexport_nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time export_nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time noexport-nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS already in kernel." ]
[ "`modprobe --first-time export-nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS already in kernel." ]

[ "`modprobe -r noexport_nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS is in use." ]
[ "`modprobe -r export_nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS is in use." ]
[ "`modprobe -r noexport-nodep-$BITNESS 2>&1`" = "FATAL: Module noexport_nodep_$BITNESS is in use." ]
[ "`modprobe -r export-nodep-$BITNESS 2>&1`" = "FATAL: Module export_nodep_$BITNESS is in use." ]

