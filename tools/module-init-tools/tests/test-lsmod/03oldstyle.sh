#! /bin/sh

mkdir -p tests/tmp/proc

# Old style has dependencies like: "name size usecount [dep] [dep]..."
echo 'foo 100 0' > tests/tmp/proc/modules
echo 'bar 101 1' >> tests/tmp/proc/modules
echo 'baz 102 1 baz_dep' >> tests/tmp/proc/modules
echo 'bazz 2147483648 2147483648 bazz_dep bazz_dep2' >> tests/tmp/proc/modules
echo 'fuzz 104 1 fuzz_dep [permenant]' >> tests/tmp/proc/modules
echo 'fuzzy 105 1 fuzzy_dep [permenant] [unsafe]' >> tests/tmp/proc/modules
echo 'fuzzier 106 0 [permenant] [unsafe]' >> tests/tmp/proc/modules

[ "`lsmod`" = "Module                  Size  Used by
foo                      100  0 
bar                      101  1 
baz                      102  1 baz_dep
bazz                2147483648  2147483648 bazz_dep bazz_dep2
fuzz                     104  1 fuzz_dep [permenant]
fuzzy                    105  1 fuzzy_dep [permenant] [unsafe]
fuzzier                  106  0 [permenant] [unsafe]" ]

# If CONFIG_MODULE_UNLOAD=n: simply "name size"
echo 'baz 102' > tests/tmp/proc/modules
echo 'bazz 2147483648' >> tests/tmp/proc/modules

[ "`lsmod`" = "Module                  Size  Used by
baz                      102
bazz                2147483648" ]
