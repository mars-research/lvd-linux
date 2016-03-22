#! /bin/bash

config=$1
wllvm_extract_bc=$2

# Parse config to get those modules that were built and linked for deker
mods=`cat ${config} | awk -f scripts/deker.awk`

# For each deker module, run extract-bc
for mod in $mods; do
    echo "  EXTRACT-BC     $mod"
    ${wllvm_extract_bc} ${LCD_TEST_MODULES_BUILD_DIR}/${mod}/*.ko
done
