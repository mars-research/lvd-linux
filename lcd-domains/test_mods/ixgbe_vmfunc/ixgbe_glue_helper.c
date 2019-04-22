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

int glue_cap_insert_net_device_type(struct glue_cspace *cspace,
		struct net_device_container *net_device_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		net_device_container,
		glue_libcap_type_ops[ GLUE_TYPE_NET_DEVICE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_net_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct net_device_container **net_device_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_NET_DEVICE_CONTAINER ].libcap_type,
		( void  ** )net_device_container);

}

int glue_cap_insert_net_device_ops_type(struct glue_cspace *cspace,
		struct net_device_ops_container *net_device_ops_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		net_device_ops_container,
		glue_libcap_type_ops[ GLUE_TYPE_NET_DEVICE_OPS_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_net_device_ops_type(struct glue_cspace *cspace,
		struct cptr c,
		struct net_device_ops_container **net_device_ops_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_NET_DEVICE_OPS_CONTAINER ].libcap_type,
		( void  ** )net_device_ops_container);

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

int glue_cap_insert_pci_device_id_type(struct glue_cspace *cspace,
		struct pci_device_id_container *pci_device_id_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		pci_device_id_container,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DEVICE_ID_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_pci_device_id_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_device_id_container **pci_device_id_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DEVICE_ID_CONTAINER ].libcap_type,
		( void  ** )pci_device_id_container);

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

int glue_cap_insert_rtnl_link_stats64_type(struct glue_cspace *cspace,
		struct rtnl_link_stats64_container *rtnl_link_stats64_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		rtnl_link_stats64_container,
		glue_libcap_type_ops[ GLUE_TYPE_RTNL_LINK_STATS64_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_rtnl_link_stats64_type(struct glue_cspace *cspace,
		struct cptr c,
		struct rtnl_link_stats64_container **rtnl_link_stats64_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_RTNL_LINK_STATS64_CONTAINER ].libcap_type,
		( void  ** )rtnl_link_stats64_container);

}

int glue_cap_insert_sk_buff_type(struct glue_cspace *cspace,
		struct sk_buff_container *sk_buff_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		sk_buff_container,
		glue_libcap_type_ops[ GLUE_TYPE_SK_BUFF_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_sk_buff_type(struct glue_cspace *cspace,
		struct cptr c,
		struct sk_buff_container **sk_buff_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_SK_BUFF_CONTAINER ].libcap_type,
		( void  ** )sk_buff_container);

}

int glue_cap_insert_napi_struct_type(struct glue_cspace *cspace,
		struct napi_struct_container *napi_struct_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		napi_struct_container,
		glue_libcap_type_ops[ GLUE_TYPE_NAPI_STRUCT_CONTAINER ].libcap_type,
		c_out);

}
int glue_cap_lookup_napi_struct_type(struct glue_cspace *cspace,
		struct cptr c,
		struct napi_struct_container **napi_struct_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_NAPI_STRUCT_CONTAINER ].libcap_type,
		( void  ** )napi_struct_container);

}

int glue_cap_insert_irqhandler_type(struct glue_cspace *cspace,
		struct irqhandler_t_container *irqhandler_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		irqhandler_container,
		glue_libcap_type_ops[ GLUE_TYPE_NAPI_STRUCT_CONTAINER ].libcap_type,
		c_out);

}
int glue_cap_lookup_irqhandler_type(struct glue_cspace *cspace,
		struct cptr c,
		struct irqhandler_t_container **irqhandler_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_IRQ_HANDLER_CONTAINER ].libcap_type,
		( void  ** )irqhandler_container);

}
