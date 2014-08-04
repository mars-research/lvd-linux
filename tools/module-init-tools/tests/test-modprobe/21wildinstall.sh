#! /bin/sh
# Test wildcard install/remove commands.

rm -rf tests/tmp/*

MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
touch $MODULE_DIR/modules.dep

MODTEST_DO_SYSTEM=1
export MODTEST_DO_SYSTEM

# Create a simple config file.
mkdir -p tests/tmp/etc/modprobe.d
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
# Various aliases
install *wildcard-_* echo installing \$MODPROBE_MODULE.
remove *wildcard-_* echo removing \$MODPROBE_MODULE.
EOF

# Install...
[ "`modprobe wildcard-_aaa 2>&1`" = "installing wildcard__aaa." ]
[ "`modprobe wildcard-_ 2>&1`" = "installing wildcard__." ]
[ "`modprobe wildcard_- 2>&1`" = "installing wildcard__." ]
[ "`modprobe anotherwildcard-_ 2>&1`" = "installing anotherwildcard__." ]
[ "`modprobe anotherwildcard-_aaa 2>&1`" = "installing anotherwildcard__aaa." ]

# Remove...
[ "`modprobe -r wildcard-_aaa 2>&1`" = "removing wildcard__aaa." ]
[ "`modprobe -r wildcard-_ 2>&1`" = "removing wildcard__." ]
[ "`modprobe -r wildcard_- 2>&1`" = "removing wildcard__." ]
[ "`modprobe -r anotherwildcard-_ 2>&1`" = "removing anotherwildcard__." ]
[ "`modprobe -r anotherwildcard-_aaa 2>&1`" = "removing anotherwildcard__aaa." ]
