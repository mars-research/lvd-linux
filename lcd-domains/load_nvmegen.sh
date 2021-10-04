#!/bin/bash

#pci@0000:06:00.1
DOMAIN="0000"
BUS="04"
DEV="00"
SLOT="0"
PCI_DEVICE="$DOMAIN:$BUS:$DEV.$SLOT";
echo "loading nvme_lcd driver for device $PCI_DEVICE";

#sudo nvme format -n 0 -r  /dev/nvme0n1
#if [ -d /sys/bus/pci/devices/$PCI_DEVICE/driver ]; then
#        sudo sh -c "echo '$PCI_DEVICE' > /sys/bus/pci/devices/$PCI_DEVICE/driver/unbind"
#fi

sudo rmmod nvme
#sudo rmmod nvme_core
./scripts/loadex nvme_gen
