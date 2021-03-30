enum glue_type {
	GLUE_TYPE_DEVICE_ATTRIBUTE_CONTAINER,
	GLUE_TYPE_DEVICE_CONTAINER,
	GLUE_TYPE_HOTPLUG_SLOT_CONTAINER,
	GLUE_TYPE_HOTPLUG_SLOT_INFO_CONTAINER,
	GLUE_TYPE_IRQ_HANDLER_CONTAINER,
	GLUE_TYPE_LOCK_CLASS_KEY_CONTAINER,
	GLUE_TYPE_MODULE_CONTAINER,
	GLUE_TYPE_PCI_BUS_CONTAINER,
	GLUE_TYPE_PCI_DEV_CONTAINER,
	GLUE_TYPE_PCI_DRIVER_CONTAINER,
	GLUE_TYPE_RESOURCE_CONTAINER,
	GLUE_TYPE_TIMER_LIST_CONTAINER,
	GLUE_NR_TYPES,

};
int glue_cap_insert_device_attribute_type(struct glue_cspace *cspace,
		struct device_attribute_container *device_attribute_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		device_attribute_container,
		glue_libcap_type_ops[ GLUE_TYPE_DEVICE_ATTRIBUTE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_device_attribute_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_attribute_container **device_attribute_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_DEVICE_ATTRIBUTE_CONTAINER ].libcap_type,
		( void  ** )device_attribute_container);

}

int glue_cap_insert_device_type(struct glue_cspace *cspace,
		struct device_container *device_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		device_container,
		glue_libcap_type_ops[ GLUE_TYPE_DEVICE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_container **device_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_DEVICE_CONTAINER ].libcap_type,
		( void  ** )device_container);

}

int glue_cap_insert_hotplug_slot_type(struct glue_cspace *cspace,
		struct hotplug_slot_container *hotplug_slot_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		hotplug_slot_container,
		glue_libcap_type_ops[ GLUE_TYPE_HOTPLUG_SLOT_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_hotplug_slot_type(struct glue_cspace *cspace,
		struct cptr c,
		struct hotplug_slot_container **hotplug_slot_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_HOTPLUG_SLOT_CONTAINER ].libcap_type,
		( void  ** )hotplug_slot_container);

}

int glue_cap_insert_hotplug_slot_info_type(struct glue_cspace *cspace,
		struct hotplug_slot_info_container *hotplug_slot_info_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		hotplug_slot_info_container,
		glue_libcap_type_ops[ GLUE_TYPE_HOTPLUG_SLOT_INFO_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_hotplug_slot_info_type(struct glue_cspace *cspace,
		struct cptr c,
		struct hotplug_slot_info_container **hotplug_slot_info_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_HOTPLUG_SLOT_INFO_CONTAINER ].libcap_type,
		( void  ** )hotplug_slot_info_container);

}

int glue_cap_insert_irq_handler_type(struct glue_cspace *cspace,
		struct irq_handler_container *irq_handler_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		irq_handler_container,
		glue_libcap_type_ops[ GLUE_TYPE_IRQ_HANDLER_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_irq_handler_type(struct glue_cspace *cspace,
		struct cptr c,
		struct irq_handler_container **irq_handler_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_IRQ_HANDLER_CONTAINER ].libcap_type,
		( void  ** )irq_handler_container);

}

int glue_cap_insert_lock_class_key_type(struct glue_cspace *cspace,
		struct lock_class_key_container *lock_class_key_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		lock_class_key_container,
		glue_libcap_type_ops[ GLUE_TYPE_LOCK_CLASS_KEY_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_lock_class_key_type(struct glue_cspace *cspace,
		struct cptr c,
		struct lock_class_key_container **lock_class_key_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_LOCK_CLASS_KEY_CONTAINER ].libcap_type,
		( void  ** )lock_class_key_container);

}

int glue_cap_insert_module_type(struct glue_cspace *cspace,
		struct module_container *module_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		module_container,
		glue_libcap_type_ops[ GLUE_TYPE_MODULE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_module_type(struct glue_cspace *cspace,
		struct cptr c,
		struct module_container **module_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_MODULE_CONTAINER ].libcap_type,
		( void  ** )module_container);

}

int glue_cap_insert_pci_bus_type(struct glue_cspace *cspace,
		struct pci_bus_container *pci_bus_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		pci_bus_container,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_BUS_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_pci_bus_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_bus_container **pci_bus_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_BUS_CONTAINER ].libcap_type,
		( void  ** )pci_bus_container);

}

int glue_cap_insert_pci_dev_type(struct glue_cspace *cspace,
		struct pci_dev_container *pci_dev_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		pci_dev_container,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DEV_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_pci_dev_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_dev_container **pci_dev_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DEV_CONTAINER ].libcap_type,
		( void  ** )pci_dev_container);

}

int glue_cap_insert_pci_driver_type(struct glue_cspace *cspace,
		struct pci_driver_container *pci_driver_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		pci_driver_container,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DRIVER_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_pci_driver_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_driver_container **pci_driver_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DRIVER_CONTAINER ].libcap_type,
		( void  ** )pci_driver_container);

}

int glue_cap_insert_resource_type(struct glue_cspace *cspace,
		struct resource_container *resource_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		resource_container,
		glue_libcap_type_ops[ GLUE_TYPE_RESOURCE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_resource_type(struct glue_cspace *cspace,
		struct cptr c,
		struct resource_container **resource_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_RESOURCE_CONTAINER ].libcap_type,
		( void  ** )resource_container);

}

int glue_cap_insert_timer_list_type(struct glue_cspace *cspace,
		struct timer_list_container *timer_list_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		timer_list_container,
		glue_libcap_type_ops[ GLUE_TYPE_TIMER_LIST_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_timer_list_type(struct glue_cspace *cspace,
		struct cptr c,
		struct timer_list_container **timer_list_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_TIMER_LIST_CONTAINER ].libcap_type,
		( void  ** )timer_list_container);

}

static struct cap_type_system *glue_libcap_type_system;

int glue_cap_init(void)
{
	int ret;
	int i;
	cap_type_t libcap_type;
	/*
	 * Alloc and init microkernel type system
	 */
	ret = cap_type_system_alloc(&glue_libcap_type_system);
	if (ret) {
		LIBLCD_ERR("alloc glue type system failed");
		goto fail1;
	}
	ret = cap_type_system_init(glue_libcap_type_system);
	if (ret) {
		LIBLCD_ERR("init glue type system failed");
		goto fail2;
	}
	/*
	 * Add types
	 */
	for (i = 0; i < GLUE_NR_TYPES; i++) {

		libcap_type = cap_register_private_type(
			glue_libcap_type_system,
			0,
			&glue_libcap_type_ops[i].ops);
		if (libcap_type == CAP_TYPE_ERR) {
			LIBLCD_ERR("failed to register glue cap type %s",
				glue_libcap_type_ops[i].ops.name);
			ret = -EIO;
			goto fail3;
		}
		glue_libcap_type_ops[i].libcap_type = libcap_type;
	}

	return 0;

fail3:
	cap_type_system_destroy(glue_libcap_type_system);
fail2:
	cap_type_system_free(glue_libcap_type_system);
	glue_libcap_type_system = NULL;
fail1:
	return ret;
}

int glue_cap_create(struct glue_cspace **cspace_out)
{
	return glue_cspace_alloc_init(glue_libcap_type_system, cspace_out);
}

void glue_cap_destroy(struct glue_cspace *cspace)
{
	glue_cspace_destroy_free(cspace);
}

void glue_cap_exit(void)
{
	/*
	 * Destroy and free type system if necessary
	 */
	if (glue_libcap_type_system) {
		cap_type_system_destroy(glue_libcap_type_system);
		cap_type_system_free(glue_libcap_type_system);
		glue_libcap_type_system = NULL;
	}
}
