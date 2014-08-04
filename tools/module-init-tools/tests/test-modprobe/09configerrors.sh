#! /bin/sh
# Test various config file errors.

rm -rf tests/tmp/*

MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
touch $MODULE_DIR/modules.dep

# Test bad alias syntax
mkdir -p tests/tmp/etc/modprobe.d
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
alias
alias foo
EOF

[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 1: ignoring bad line starting with 'alias'
WARNING: /etc/modprobe.d/modprobe.conf line 2: ignoring bad line starting with 'alias'
FATAL: Module foo not found." ]

# Bad option syntax
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
options
options foo
EOF

[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 1: ignoring bad line starting with 'options'
WARNING: /etc/modprobe.d/modprobe.conf line 2: ignoring bad line starting with 'options'
FATAL: Module foo not found." ]

# Bad include syntax
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
include
EOF

[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 1: ignoring bad line starting with 'include'
FATAL: Module foo not found." ]

# Bad install syntax
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
install
install foo
EOF

[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 1: ignoring bad line starting with 'install'
WARNING: /etc/modprobe.d/modprobe.conf line 2: ignoring bad line starting with 'install'
FATAL: Module foo not found." ]

# Bad remove syntax
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
remove
remove foo
EOF

[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 1: ignoring bad line starting with 'remove'
WARNING: /etc/modprobe.d/modprobe.conf line 2: ignoring bad line starting with 'remove'
FATAL: Module foo not found." ]

# Complete junk
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
complete junk and stuff
rubbish
EOF

[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 1: ignoring bad line starting with 'complete'
WARNING: /etc/modprobe.d/modprobe.conf line 2: ignoring bad line starting with 'rubbish'
FATAL: Module foo not found." ]

# Line numbering counted correctly.
echo "#comment" > tests/tmp/etc/modprobe.d/modprobe.conf
echo "remove" >> tests/tmp/etc/modprobe.d/modprobe.conf

[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 2: ignoring bad line starting with 'remove'
FATAL: Module foo not found." ]

echo "" > tests/tmp/etc/modprobe.d/modprobe.conf
echo "remove" >> tests/tmp/etc/modprobe.d/modprobe.conf

[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 2: ignoring bad line starting with 'remove'
FATAL: Module foo not found." ]

echo "  # Comment" > tests/tmp/etc/modprobe.d/modprobe.conf
echo "remove" >> tests/tmp/etc/modprobe.d/modprobe.conf

[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 2: ignoring bad line starting with 'remove'
FATAL: Module foo not found." ]

echo "  # Comment \\" > tests/tmp/etc/modprobe.d/modprobe.conf
echo "with multiple lines" >> tests/tmp/etc/modprobe.d/modprobe.conf
echo "remove" >> tests/tmp/etc/modprobe.d/modprobe.conf

[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 3: ignoring bad line starting with 'remove'
FATAL: Module foo not found." ]

echo "remove foo \\" > tests/tmp/etc/modprobe.d/modprobe.conf
echo "  bar" >> tests/tmp/etc/modprobe.d/modprobe.conf
echo "remove" >> tests/tmp/etc/modprobe.d/modprobe.conf
[ "`modprobe foo 2>&1`" = "WARNING: /etc/modprobe.d/modprobe.conf line 3: ignoring bad line starting with 'remove'
FATAL: Module foo not found." ]


