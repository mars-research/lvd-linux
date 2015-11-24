#! /bin/sh
# Check underscore synonymity everywhere.

BITNESS=32

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko \
   tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko \
   $MODULE_DIR

# Set up modules.dep file.
echo "# A comment" > $MODULE_DIR/modules.dep
echo "noexport_nodep-$BITNESS.ko:" >> $MODULE_DIR/modules.dep
echo "export_nodep-$BITNESS.ko:" >> $MODULE_DIR/modules.dep

# Set up config file.
mkdir -p tests/tmp/etc/modprobe.d
echo "alias alias-_ noexport-nodep_$BITNESS" > tests/tmp/etc/modprobe.d/modprobe.conf
echo "options export-nodep_$BITNESS option-_" >> tests/tmp/etc/modprobe.d/modprobe.conf
echo "install test-_ echo install-_" >> tests/tmp/etc/modprobe.d/modprobe.conf
echo "remove test-_ echo remove-_" >> tests/tmp/etc/modprobe.d/modprobe.conf
echo "install test-include echo Included" >> tests/tmp/etc/modprobe.d/modprobe-include.conf

SIZE1=`wc -c < tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko`
SIZE2=`wc -c < tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko`

# On command line (-r and normal)
[ "`modprobe noexport-nodep_$BITNESS 2>&1`" = "INIT_MODULE: $SIZE1 " ]
[ "`modprobe -r noexport-nodep_$BITNESS 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL " ]

# In alias commands (source and target)
[ "`modprobe alias-_ 2>&1`" = "INIT_MODULE: $SIZE1 " ]
[ "`modprobe alias_- 2>&1`" = "INIT_MODULE: $SIZE1 " ]
[ "`modprobe -r alias-_ 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL " ]
[ "`modprobe -r alias_- 2>&1`" = "DELETE_MODULE: noexport_nodep_$BITNESS EXCL " ]

# In option commands (NOT in arguments)
[ "`modprobe export_nodep-$BITNESS 2>&1`" = "INIT_MODULE: $SIZE2 option-_" ]
[ "`modprobe export-nodep_$BITNESS 2>&1`" = "INIT_MODULE: $SIZE2 option-_" ]

# In install commands
[ "`modprobe test-_ 2>&1`" = "SYSTEM: echo install-_" ]
[ "`modprobe test_- 2>&1`" = "SYSTEM: echo install-_" ]

# In remove commands
[ "`modprobe -r test-_ 2>&1`" = "SYSTEM: echo remove-_" ]
[ "`modprobe -r test_- 2>&1`" = "SYSTEM: echo remove-_" ]

# NOT in include commands
[ "`modprobe test-include 2>&1`" = "SYSTEM: echo Included" ]
[ "`modprobe test_include 2>&1`" = "SYSTEM: echo Included" ]
