#! /bin/sh

BITNESS=32

rm -rf tests/tmp/*

MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME

# Set up modules
mkdir -p $MODULE_DIR
ln tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko $MODULE_DIR/a.ko
ln -s a.ko $MODULE_DIR/b.ko
ln -s a.ko $MODULE_DIR/c.ko
ln -s a.ko $MODULE_DIR/d.ko
ln -s a.ko $MODULE_DIR/e.ko
ln -s a.ko $MODULE_DIR/f.ko

# Set up dependencies
cat > $MODULE_DIR/modules.dep << EOF
/lib/modules/$MODTEST_UNAME/a.ko:
/lib/modules/$MODTEST_UNAME/b.ko:
/lib/modules/$MODTEST_UNAME/c.ko:
/lib/modules/$MODTEST_UNAME/d.ko:
/lib/modules/$MODTEST_UNAME/e.ko:
/lib/modules/$MODTEST_UNAME/f.ko:
EOF

# Test softdeps

mkdir -p tests/tmp/etc/modprobe.d
cat > tests/tmp/etc/modprobe.d/modprobe.conf << EOF
alias a_alias a
alias c_alias c
softdep c pre: a_alias b post: d e
softdep e post: f
softdep f pre: a
EOF

# Insert-test

R1=`modprobe -v c_alias 2>&1 | \
	grep -v INIT_MODULE\: | \
	sed -e "s/\/lib\/modules\/$MODTEST_UNAME\///g"`
R2=`echo $R1`	# remove newlines
[ "$R2" = "insmod a.ko insmod b.ko insmod c.ko insmod d.ko insmod e.ko insmod a.ko insmod f.ko" ]

# Remove-test

R1=`modprobe -v -r c_alias 2>&1 | \
	grep -v DELETE_MODULE\: | \
	sed -e "s/\/lib\/modules\/$MODTEST_UNAME\///g"`
R2=`echo $R1`	# remove newlines
[ "$R2" = "rmmod f.ko rmmod a.ko rmmod e.ko rmmod d.ko rmmod c.ko rmmod b.ko rmmod a.ko" ]

# Test loop detector

mkdir -p tests/tmp/etc/modprobe.d
cat > tests/tmp/etc/modprobe.d/modprobe.conf << EOF
alias a_alias a
softdep a pre: b
softdep b pre: a
EOF

R1=`modprobe -v a_alias 2>&1 | cat` # shell won't assign to R1 without no-op cat.
# This comparison will fail if MAX_RECURSION is odd. In that case: s/b/a/g
[ "$R1" = "FATAL: modprobe: softdep dependency loop encountered inserting b" ]

