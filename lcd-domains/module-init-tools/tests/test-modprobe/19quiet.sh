#! /bin/sh

BITNESS=32

rm -rf tests/tmp/*

MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko \
   $MODULE_DIR

# Set up modules.dep file.
echo "# A comment" > $MODULE_DIR/modules.dep
echo "noexport_nodep-$BITNESS.ko:" >> $MODULE_DIR/modules.dep
echo "bogus-$BITNESS.ko:" >> $MODULE_DIR/modules.dep

mkdir -p tests/tmp/etc/modprobe.d
echo "install some-command modprobe crap && echo SUCCESS" > tests/tmp/etc/modprobe.d/modprobe.conf 
echo "remove some-command modprobe -r crap && echo SUCCESS" >> tests/tmp/etc/modprobe.d/modprobe.conf 
echo "alias foobar crap" >> tests/tmp/etc/modprobe.d/modprobe.conf 

SIZE=`wc -c < tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko`

# -q works as normal.
[ "`modprobe -q noexport_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE " ]

# -q on non-existent fail, quietly.
[ "`modprobe -q crap 2>&1`" = "" ]
if modprobe -q crap; then exit 1; fi

# -q on alias to non-existent succeeds, quietly.
[ "`modprobe -q foobar 2>&1`" = "" ]
if modprobe -q foobar; then exit 1; fi

# -q on some other problem gives errors.
[ "`modprobe -q bogus-$BITNESS 2>&1`" != "" ]
if modprobe -q bogus-$BITNESS 2>/dev/null; then exit 1; fi

MODTEST_DO_SYSTEM=1
export MODTEST_DO_SYSTEM
# Normal install command will fail.
[ "`modprobe some-command 2>&1`" = "FATAL: Module crap not found.
FATAL: Error running install command for some_command" ]
if modprobe some-command 2>/dev/null; then exit 1; fi

# -q doesn't cause "modprobe crap" to succeed, but is passed through install.
[ "`modprobe -q some-command 2>&1`" = "FATAL: Error running install command for some_command" ]
if modprobe -q some-command 2>/dev/null; then exit 1; fi

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

# -q works as normal.
[ "`modprobe -r -q noexport_nodep-$BITNESS 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL " ]

# -q on non-existent module fails, silently.
[ "`modprobe -r -q crap 2>&1`" = "" ]
if modprobe -r -q crap; then exit 1; fi

MODTEST_DO_SYSTEM=1
export MODTEST_DO_SYSTEM
# Normal remove command will fail.
[ "`modprobe -r some-command 2>&1`" = "FATAL: Module crap not found.
FATAL: Error running remove command for some_command" ]
if modprobe -r some-command 2>/dev/null; then exit 1; fi

# -q doesn't cause "modprobe -r crap" to succeed, but silences it.
[ "`modprobe -r -q some-command 2>&1`" = "FATAL: Error running remove command for some_command" ]
if modprobe -r -q some-command 2>/dev/null; then exit 1; fi
