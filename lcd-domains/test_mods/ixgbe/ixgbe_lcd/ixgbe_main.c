#include <lcd_config/pre_hook.h>

#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include "./ixgbe_caller.h"
#include <lcd_config/post_hook.h>

#define IXGBE_DEV_ID_82599_SFP_SF2 0x154D

static const struct pci_device_id ixgbe_pci_tbl[] = {
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_SFP_SF2) },
	{ 0 },
};

const char ixgbe_driver_name[] = "ixgbe_lcd";

static int ixgbe_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
        LIBLCD_MSG("claimed by ixgbe_lcd\n");
        return 0;
}

static void ixgbe_remove(struct pci_dev *dev)
{
        LIBLCD_MSG("remove called : ixgbe_lcd\n");
}

struct pci_driver_container ixgbe_driver_container = {
	.pci_driver = {
	        .name     = ixgbe_driver_name,
	        .id_table = ixgbe_pci_tbl,
	        .probe    = ixgbe_probe,
		.remove   = ixgbe_remove,
	}
};


int ixgbe_init_module(void)
{
	int ret;
	ret = pci_register_driver(&ixgbe_driver_container.pci_driver);
        if (ret) {
		printk("registration failed %d \n", ret);
                return ret;
        }
	printk("registered \n");
	return 0;
}

void ixgbe_exit_module(void)
{
	pci_unregister_driver(&ixgbe_driver_container.pci_driver);
}
