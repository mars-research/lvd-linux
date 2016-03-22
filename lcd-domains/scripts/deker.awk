#
# Lists the modules in config file that are built for deker.
#
# Invocation example:
#
#     cat config | awk -f deker.awk
#
# where conf is the module build configuration file (test_mods/config).
#

# Skip comment lines
/#.*/ { next }

# Skip empty lines
NF == 0 { next }

# Print out build config define for all others
$2 == "deker" { 
    print $1
}

