#include <lcd_config/pre_hook.h>

#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include "./nvme_caller.h"
#include <lcd_config/post_hook.h>

//#define IXGBE_DEV_ID_82599_SFP_SF2 0x154D

static const struct pci_device_id nvme_pci_tbl[] = {
	{ PCI_DEVICE_CLASS(PCI_CLASS_STORAGE_EXPRESS, 0xffffff) },
	{ 0 },
};

const char nvme_driver_name[] = "nvme_lcd";

static int nvme_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
        LIBLCD_MSG("claimed by nvme_lcd\n");
        return 0;
}

static void nvme_remove(struct pci_dev *dev)
{
        LIBLCD_MSG("remove called : nvme_lcd\n");
}

struct pci_driver_container nvme_driver_container = {
	.pci_driver = {
	        .name     = nvme_driver_name,
	        .id_table = nvme_pci_tbl,
	        .probe    = nvme_probe,
		.remove   = nvme_remove,
	}
};


int nvme_init_module(void)
{
	int ret;
	ret = pci_register_driver(&nvme_driver_container.pci_driver);
        if (ret) {
		printk("registration failed %d \n", ret);
                return ret;
        }
	printk("registered \n");
	return 0;
}

void nvme_exit_module(void)
{
	pci_unregister_driver(&ixgbe_driver_container.pci_driver);
}
