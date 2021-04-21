#!/bin/bash
# For creating a tree structure with empty source files and valid Kbuild files
# necessary for a lcd based module

usage() {
    echo "create_tree.sh <folder_name> <lcd_prefix> <klcd_prefix>"
    exit;
}

MODULE=
LCD=
KLCD=

if [ $# -ne 3 ];
then
    usage;
else
    MODULE=$1
    LCD=$2_"lcd"
    KLCD=$3_"klcd"
fi

create_files() {
    echo "Creating test_module $1 ...";
    mkdir -p ${MODULE}/{boot,${LCD},${KLCD}};
    # root level files followed by boot, LCD, KLCD module files
    # Not touching Kbuild, they are written directly in the below functions
    touch ${MODULE}/boot/main.c \
            ${MODULE}/${LCD}/main.c \
            ${MODULE}/${KLCD}/main.c
}

main_kbuild() {
    # Main kbuild file
    echo -e "obj-\$(LCD_CONFIG_BUILD_${MODULE^^}_BOOT)\t+= boot/\n" >> ${MODULE}/Kbuild
    echo -e "obj-\$(LCD_CONFIG_BUILD_${MODULE^^}_${LCD^^}) += ${LCD}/\n" >> ${MODULE}/Kbuild
    echo -e "obj-\$(LCD_CONFIG_BUILD_${MODULE^^}_${KLCD^^}) += ${KLCD}/\n" >> ${MODULE}/Kbuild
}

boot_kbuild() {
    # boot kbuild file
    echo -e "obj-m += lcd_test_mod_${MODULE}_boot.o\n\n" >> ${MODULE}/boot/Kbuild; 
    echo -e "lcd_test_mod_${MODULE}_boot-y\t\t+= main.o\n" >> ${MODULE}/boot/Kbuild;
    echo -e "ccflags-y += \$(NONISOLATED_CFLAGS)" >> ${MODULE}/boot/Kbuild;
}

klcd_kbuild() {
    # KLCD kbuild file
    echo -e "obj-m += lcd_test_mod_${MODULE}_${KLCD}.o\n\nlcd_test_mod_${MODULE}_${KLCD}-y\t\t+= main.o\n" > ${MODULE}/${KLCD}/Kbuild;
    echo -e "# glue code\nlcd_test_mod_${MODULE}_${KLCD}-y\t\t+= \$(addprefix ../, server.o common.o glue_user.o )\n" >> ${MODULE}/${KLCD}/Kbuild;
    echo -e "cppflags-y += \$(NONISOLATED_CFLAGS)\n" >> ${MODULE}/${KLCD}/Kbuild;
    echo -e "extra-y += ../trampolines.lds\n" >> ${MODULE}/${KLCD}/Kbuild;
    echo -e "ldflags-y += -T \$(LCD_TEST_MODULES_BUILD_DIR)/${MODULE}/trampolines.lds\n" >> ${MODULE}/${KLCD}/Kbuild;
    echo -e "ccflags-y += \$(NONISOLATED_CFLAGS)\n" >> ${MODULE}/${KLCD}/Kbuild;
}

lcd_kbuild() {
    # LCD kbuild file
    echo -e "obj-m += lcd_test_mod_${MODULE}_${LCD}.o\n\nlcd_test_mod_${MODULE}_${LCD}-y\t\t+= main.o\n" > ${MODULE}/${LCD}/Kbuild;
    echo -e "# Original code\nlcd_test_mod_${MODULE}_${LCD}-y\t\t+= " >> ${MODULE}/${LCD}/Kbuild;
    echo -e "\n\nlcd_test_mod_${MODULE}_${LCD}-y\t\t+= \$(LIBLCD)\n" >> ${MODULE}/${LCD}/Kbuild;
    echo -e "# glue code\nlcd_test_mod_${MODULE}_${LCD}-y\t\t+= \$(addprefix ../, client.o common.o glue_user.o )\n" >> ${MODULE}/${LCD}/Kbuild;
    echo -e "ccflags-y += \$(ISOLATED_CFLAGS)\n" >> ${MODULE}/${LCD}/Kbuild;
    echo -e "extra-y += ../../../liblcd_build/common/vmfunc.lds\n" >> ${MODULE}/${LCD}/Kbuild;
    echo -e "ldflags-y += -T \$(LIBLCD_BUILD_DIR)/common/vmfunc.lds\n" >> ${MODULE}/${LCD}/Kbuild;
    echo -e "cppflags-y += \$(ISOLATED_CFLAGS)\n" >> ${MODULE}/${LCD}/Kbuild;
}

create_files;
main_kbuild;
boot_kbuild;
klcd_kbuild;
lcd_kbuild;
echo "Done ...!!";
