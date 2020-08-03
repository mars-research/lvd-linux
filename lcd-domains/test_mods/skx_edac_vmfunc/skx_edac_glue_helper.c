enum glue_type {
	GLUE_TYPE_ATTRIBUTE_GROUP_CONTAINER,
	GLUE_TYPE_DENTRY_CONTAINER,
	GLUE_TYPE_DEVICE_CONTAINER,
	GLUE_TYPE_EDAC_MC_LAYER_CONTAINER,
	GLUE_TYPE_FILE_CONTAINER,
	GLUE_TYPE_FILE_OPERATIONS_CONTAINER,
	GLUE_TYPE_GET_CONTAINER,
	GLUE_TYPE_INODE_CONTAINER,
	GLUE_TYPE_MEM_CTL_INFO_CONTAINER,
	GLUE_TYPE_NOTIFIER_BLOCK_CONTAINER,
	GLUE_TYPE_PCI_DEV_CONTAINER,
	GLUE_TYPE_X86_CPU_ID_CONTAINER,
	GLUE_NR_TYPES,

};
int glue_cap_insert_attribute_group_type(struct glue_cspace *cspace,
		struct attribute_group_container *attribute_group_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		attribute_group_container,
		glue_libcap_type_ops[ GLUE_TYPE_ATTRIBUTE_GROUP_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_attribute_group_type(struct glue_cspace *cspace,
		struct cptr c,
		struct attribute_group_container **attribute_group_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_ATTRIBUTE_GROUP_CONTAINER ].libcap_type,
		( void  ** )attribute_group_container);

}

int glue_cap_insert_dentry_type(struct glue_cspace *cspace,
		struct dentry_container *dentry_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		dentry_container,
		glue_libcap_type_ops[ GLUE_TYPE_DENTRY_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_dentry_type(struct glue_cspace *cspace,
		struct cptr c,
		struct dentry_container **dentry_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_DENTRY_CONTAINER ].libcap_type,
		( void  ** )dentry_container);

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

int glue_cap_insert_edac_mc_layer_type(struct glue_cspace *cspace,
		struct edac_mc_layer_container *edac_mc_layer_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		edac_mc_layer_container,
		glue_libcap_type_ops[ GLUE_TYPE_EDAC_MC_LAYER_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_edac_mc_layer_type(struct glue_cspace *cspace,
		struct cptr c,
		struct edac_mc_layer_container **edac_mc_layer_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_EDAC_MC_LAYER_CONTAINER ].libcap_type,
		( void  ** )edac_mc_layer_container);

}

int glue_cap_insert_file_type(struct glue_cspace *cspace,
		struct file_container *file_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		file_container,
		glue_libcap_type_ops[ GLUE_TYPE_FILE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_file_type(struct glue_cspace *cspace,
		struct cptr c,
		struct file_container **file_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_FILE_CONTAINER ].libcap_type,
		( void  ** )file_container);

}

int glue_cap_insert_file_operations_type(struct glue_cspace *cspace,
		struct file_operations_container *file_operations_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		file_operations_container,
		glue_libcap_type_ops[ GLUE_TYPE_FILE_OPERATIONS_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_file_operations_type(struct glue_cspace *cspace,
		struct cptr c,
		struct file_operations_container **file_operations_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_FILE_OPERATIONS_CONTAINER ].libcap_type,
		( void  ** )file_operations_container);

}

int glue_cap_insert_get_type(struct glue_cspace *cspace,
		struct get_container *get_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		get_container,
		glue_libcap_type_ops[ GLUE_TYPE_GET_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_get_type(struct glue_cspace *cspace,
		struct cptr c,
		struct get_container **get_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_GET_CONTAINER ].libcap_type,
		( void  ** )get_container);

}

int glue_cap_insert_inode_type(struct glue_cspace *cspace,
		struct inode_container *inode_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		inode_container,
		glue_libcap_type_ops[ GLUE_TYPE_INODE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_inode_type(struct glue_cspace *cspace,
		struct cptr c,
		struct inode_container **inode_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_INODE_CONTAINER ].libcap_type,
		( void  ** )inode_container);

}

int glue_cap_insert_mem_ctl_info_type(struct glue_cspace *cspace,
		struct mem_ctl_info_container *mem_ctl_info_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		mem_ctl_info_container,
		glue_libcap_type_ops[ GLUE_TYPE_MEM_CTL_INFO_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_mem_ctl_info_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mem_ctl_info_container **mem_ctl_info_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_MEM_CTL_INFO_CONTAINER ].libcap_type,
		( void  ** )mem_ctl_info_container);

}

int glue_cap_insert_notifier_block_type(struct glue_cspace *cspace,
		struct notifier_block_container *notifier_block_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		notifier_block_container,
		glue_libcap_type_ops[ GLUE_TYPE_NOTIFIER_BLOCK_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_notifier_block_type(struct glue_cspace *cspace,
		struct cptr c,
		struct notifier_block_container **notifier_block_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_NOTIFIER_BLOCK_CONTAINER ].libcap_type,
		( void  ** )notifier_block_container);

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

int glue_cap_insert_x86_cpu_id_type(struct glue_cspace *cspace,
		struct x86_cpu_id_container *x86_cpu_id_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		x86_cpu_id_container,
		glue_libcap_type_ops[ GLUE_TYPE_X86_CPU_ID_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_x86_cpu_id_type(struct glue_cspace *cspace,
		struct cptr c,
		struct x86_cpu_id_container **x86_cpu_id_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_X86_CPU_ID_CONTAINER ].libcap_type,
		( void  ** )x86_cpu_id_container);

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
