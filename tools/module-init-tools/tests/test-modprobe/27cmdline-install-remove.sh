#! /bin/sh

BITNESS=32

rm -rf tests/tmp/*

MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko \
   $MODULE_DIR

# Set up modules.dep file.
echo "# A comment" > $MODULE_DIR/modules.dep
echo "/lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko:" >> $MODULE_DIR/modules.dep

# Set up modprobe.conf file: one at end, one with text either side.
mkdir -p tests/tmp/etc/modprobe.d
echo "options noexport_nodep-$BITNESS file-options" > tests/tmp/etc/modprobe.d/modprobe.conf
echo "install noexport_nodep-$BITNESS modprobe --ignore-install noexport_nodep-$BITNESS \$CMDLINE_OPTS" >> tests/tmp/etc/modprobe.d/modprobe.conf
echo "install othertarget echo \$CMDLINE_OPTS otheropts" >> tests/tmp/etc/modprobe.d/modprobe.conf

# Empty /sys/module/ for install commands
mkdir -p tests/tmp/sys/module

# With quoted args
[ "`modprobe noexport_nodep-$BITNESS 'foo="bar baz"' 2>&1`" = "SYSTEM: modprobe --ignore-install noexport_nodep-$BITNESS foo=\"bar baz\"" ]
# With unquoted args
[ "`modprobe noexport_nodep-$BITNESS foo=\"bar baz\" 2>&1`" = "SYSTEM: modprobe --ignore-install noexport_nodep-$BITNESS foo=\"bar baz\"" ]

# Same with other target.
[ "`modprobe othertarget 'foo="bar baz"' 2>&1`" = "SYSTEM: echo foo=\"bar baz\" otheropts" ]
# With unquoted args
[ "`modprobe othertarget foo=\"bar baz\" 2>&1`" = "SYSTEM: echo foo=\"bar baz\" otheropts" ]
