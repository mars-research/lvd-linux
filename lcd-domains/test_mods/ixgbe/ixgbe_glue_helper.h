#ifndef __IXGBE_GLUE_HELPER_H__
#define __IXGBE_GLUE_HELPER_H__

struct device_container {
	struct device device;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct module_container {
	struct module module;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct net_device_container {
	struct net_device net_device;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct net_device_ops_container {
	struct net_device_ops net_device_ops;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_bus_container {
	struct pci_bus pci_bus;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_dev_container {
	struct pci_dev pci_dev;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_device_id_container {
	struct pci_device_id pci_device_id;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_driver_container {
	struct pci_driver pci_driver;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct rtnl_link_stats64_container {
	struct rtnl_link_stats64 rtnl_link_stats64;
	struct cptr other_ref;
	struct cptr my_ref;
};

struct sk_buff_container {
	/* just store the pointer */
	struct sk_buff *skb;
	/* store the order when volunteered. comes handy during unmap */
	unsigned int skb_ord, skbd_ord;
	cptr_t skb_cptr, skbh_cptr;
	/*
	 * as head, data pointer is different in LCD and KLCD, store it
	 * while crossing the boundary
	 */
	unsigned char *head, *data;
	/* for hashtable insertion */
	struct hlist_node hentry;
	struct cptr other_ref;
	struct cptr my_ref;
	struct task_struct *tsk;
};

struct trampoline_hidden_args {
	void *struct_container;
	struct glue_cspace *cspace;
	struct lcd_trampoline_handle *t_handle;
	struct thc_channel *async_chnl;
	struct cptr sync_ep;
};
struct sync_container {
	int ( *sync )(struct net_device *, const unsigned char*);
	cptr_t my_ref;
	cptr_t other_ref;
};

struct unsync_container {
	int ( *unsync )(struct net_device *, const unsigned char*);
	cptr_t my_ref;
	cptr_t other_ref;
};

struct poll_container {
	int ( *poll )(struct napi_struct *, int);
	cptr_t my_ref;
	cptr_t other_ref;
};

int glue_cap_insert_device_type(struct glue_cspace *cspace,
		struct device_container *device_container,
		struct cptr *c_out);
int glue_cap_insert_module_type(struct glue_cspace *cspace,
		struct module_container *module_container,
		struct cptr *c_out);
int glue_cap_insert_net_device_type(struct glue_cspace *cspace,
		struct net_device_container *net_device_container,
		struct cptr *c_out);
int glue_cap_insert_net_device_ops_type(struct glue_cspace *cspace,
		struct net_device_ops_container *net_device_ops_container,
		struct cptr *c_out);
int glue_cap_insert_pci_bus_type(struct glue_cspace *cspace,
		struct pci_bus_container *pci_bus_container,
		struct cptr *c_out);
int glue_cap_insert_pci_dev_type(struct glue_cspace *cspace,
		struct pci_dev_container *pci_dev_container,
		struct cptr *c_out);
int glue_cap_insert_pci_device_id_type(struct glue_cspace *cspace,
		struct pci_device_id_container *pci_device_id_container,
		struct cptr *c_out);
int glue_cap_insert_pci_driver_type(struct glue_cspace *cspace,
		struct pci_driver_container *pci_driver_container,
		struct cptr *c_out);
int glue_cap_insert_rtnl_link_stats64_type(struct glue_cspace *cspace,
		struct rtnl_link_stats64_container *rtnl_link_stats64_container,
		struct cptr *c_out);
int glue_cap_insert_sk_buff_type(struct glue_cspace *cspace,
		struct sk_buff_container *sk_buff_container,
		struct cptr *c_out);
int glue_cap_lookup_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_container **device_container);
int glue_cap_lookup_module_type(struct glue_cspace *cspace,
		struct cptr c,
		struct module_container **module_container);
int glue_cap_lookup_net_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct net_device_container **net_device_container);
int glue_cap_lookup_net_device_ops_type(struct glue_cspace *cspace,
		struct cptr c,
		struct net_device_ops_container **net_device_ops_container);
int glue_cap_lookup_pci_bus_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_bus_container **pci_bus_container);
int glue_cap_lookup_pci_dev_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_dev_container **pci_dev_container);
int glue_cap_lookup_pci_device_id_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_device_id_container **pci_device_id_container);
int glue_cap_lookup_pci_driver_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_driver_container **pci_driver_container);
int glue_cap_lookup_rtnl_link_stats64_type(struct glue_cspace *cspace,
		struct cptr c,
		struct rtnl_link_stats64_container **rtnl_link_stats64_container);
int glue_cap_lookup_sk_buff_type(struct glue_cspace *cspace,
		struct cptr c,
		struct sk_buff_container **sk_buff_container);

#endif /* __IXGBE_GLUE_HELPER_H__ */
