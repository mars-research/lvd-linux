#
# Generates the Kbuild.conf for test module builds. This just
# defines the right variable depending on how we want to build
# each module.
#
# Invocation example:
#
#     cat config | awk -f conf.awk
#
# where conf is the module build configuration file (test_mods/config).
#

# Skip comment lines
/#.*/ { next }

# Skip empty lines
NF == 0 { next }

# Print out build config define for all others
{ 
    gsub(/\//, "_", $1)
    print "export LCD_CONFIG_" toupper($1) "_" toupper($2) "=y";
}

