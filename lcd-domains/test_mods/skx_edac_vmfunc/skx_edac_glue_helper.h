#ifndef __SKX_EDAC_GLUE_HELPER_H__
#define __SKX_EDAC_GLUE_HELPER_H__

struct attribute_group_container {
	struct attribute_group attribute_group;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct dentry_container {
	struct dentry dentry;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct device_container {
	struct device device;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct edac_mc_layer_container {
	struct edac_mc_layer edac_mc_layer;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct file_container {
	struct file file;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct file_operations_container {
	struct file_operations file_operations;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct get_container {
	int ( *get )(int ,
		unsigned long long ), ;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct inode_container {
	struct inode inode;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct mem_ctl_info_container {
	struct mem_ctl_info *mci;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct notifier_block_container {
	struct notifier_block notifier_block;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_dev_container {
	struct pci_dev *pdev;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_bus_container {
	struct pci_bus *pbus;
	struct cptr other_ref;
	struct cptr my_ref;
};

struct x86_cpu_id_container {
	struct x86_cpu_id x86_cpu_id;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct trampoline_hidden_args {
	void *struct_container;
	struct glue_cspace *cspace;
	struct lcd_trampoline_handle *t_handle;
	struct thc_channel *async_chnl;
	struct cptr sync_ep;
};
int glue_cap_insert_attribute_group_type(struct glue_cspace *cspace,
		struct attribute_group_container *attribute_group_container,
		struct cptr *c_out);
int glue_cap_insert_dentry_type(struct glue_cspace *cspace,
		struct dentry_container *dentry_container,
		struct cptr *c_out);
int glue_cap_insert_device_type(struct glue_cspace *cspace,
		struct device_container *device_container,
		struct cptr *c_out);
int glue_cap_insert_edac_mc_layer_type(struct glue_cspace *cspace,
		struct edac_mc_layer_container *edac_mc_layer_container,
		struct cptr *c_out);
int glue_cap_insert_file_type(struct glue_cspace *cspace,
		struct file_container *file_container,
		struct cptr *c_out);
int glue_cap_insert_file_operations_type(struct glue_cspace *cspace,
		struct file_operations_container *file_operations_container,
		struct cptr *c_out);
int glue_cap_insert_get_type(struct glue_cspace *cspace,
		struct get_container *get_container,
		struct cptr *c_out);
int glue_cap_insert_inode_type(struct glue_cspace *cspace,
		struct inode_container *inode_container,
		struct cptr *c_out);
int glue_cap_insert_mem_ctl_info_type(struct glue_cspace *cspace,
		struct mem_ctl_info_container *mem_ctl_info_container,
		struct cptr *c_out);
int glue_cap_insert_notifier_block_type(struct glue_cspace *cspace,
		struct notifier_block_container *notifier_block_container,
		struct cptr *c_out);
int glue_cap_insert_pci_dev_type(struct glue_cspace *cspace,
		struct pci_dev_container *pci_dev_container,
		struct cptr *c_out);
int glue_cap_insert_x86_cpu_id_type(struct glue_cspace *cspace,
		struct x86_cpu_id_container *x86_cpu_id_container,
		struct cptr *c_out);
int glue_cap_lookup_attribute_group_type(struct glue_cspace *cspace,
		struct cptr c,
		struct attribute_group_container **attribute_group_container);
int glue_cap_lookup_dentry_type(struct glue_cspace *cspace,
		struct cptr c,
		struct dentry_container **dentry_container);
int glue_cap_lookup_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_container **device_container);
int glue_cap_lookup_edac_mc_layer_type(struct glue_cspace *cspace,
		struct cptr c,
		struct edac_mc_layer_container **edac_mc_layer_container);
int glue_cap_lookup_file_type(struct glue_cspace *cspace,
		struct cptr c,
		struct file_container **file_container);
int glue_cap_lookup_file_operations_type(struct glue_cspace *cspace,
		struct cptr c,
		struct file_operations_container **file_operations_container);
int glue_cap_lookup_get_type(struct glue_cspace *cspace,
		struct cptr c,
		struct get_container **get_container);
int glue_cap_lookup_inode_type(struct glue_cspace *cspace,
		struct cptr c,
		struct inode_container **inode_container);
int glue_cap_lookup_mem_ctl_info_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mem_ctl_info_container **mem_ctl_info_container);
int glue_cap_lookup_notifier_block_type(struct glue_cspace *cspace,
		struct cptr c,
		struct notifier_block_container **notifier_block_container);
int glue_cap_lookup_pci_dev_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_dev_container **pci_dev_container);
int glue_cap_lookup_x86_cpu_id_type(struct glue_cspace *cspace,
		struct cptr c,
		struct x86_cpu_id_container **x86_cpu_id_container);
typedef enum {
	ARRAY = 1,
	POINTER = 2,
} data_type_t;

typedef enum {
	U8 = 1,
	U16 = 2,
	U32 = 3,
	U64 = 4,
	MEM_CTL_INFO,
	EDAC_MC_LAYER,
	CSROW_INFO,
	DIMM_INFO,
} type_id;

struct value {
	/* value */
	int obj_id;
	/* value attributes */
	type_id type;
	data_type_t dtype;
};

struct kson {
	/* key */
	void *addr;
	struct value value;
};

struct hash {
	u32 num_objs;
	struct kson objs[MAX_OBJS];
};

int find(struct hash *ht, void *addr)
{
	u32 i;
	for (i = 0; i < ht->num_objs; i++) {
		if (ht->objs[i].addr == addr) {
			return ht->objs[i].obj_id;
		}
	}
	return -1;
}

u32 insert(struct hash *ht, void *addr, struct value *val)
{
	ht->objs[ht->num_objs].addr = addr;
	ht->objs[ht->num_objs].value.obj_id = ht->num_objs;
	ht->objs[ht->num_objs].value.type = val->type;
	ht->objs[ht->num_objs].value.dtype = val->dtype;
	return ht->num_objs++;
}
#endif	/* __SKX_EDAC_GLUE_HELPER_H__ */
