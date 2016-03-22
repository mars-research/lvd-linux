#! /bin/bash

config=$1
wllvm_extract_bc=$2

# Parse config to get those modules that were built and linked for deker
mods=`cat ${config} | awk -f scripts/conf.awk | awk '$2 == "deker" { print $1 }'`

# For each deker module, run extract-bc
for mod in $mods; do
    ${wllvm_extract_bc} ${mod}/*.ko
done
