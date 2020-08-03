#include "../skx_edac_caller.h"

static struct cptr c;
static struct glue_cspace *c_cspace;

#define CPTR_HASH_BITS	5
static DEFINE_HASHTABLE(pdev_hash, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(pbus_hash, CPTR_HASH_BITS);

int glue_skx_edac_init(void)
{
	int ret;
	ret = glue_cap_init();
	if (ret) {
		LIBLCD_ERR("cap init");
		goto fail1;
	}
	ret = glue_cap_create(&c_cspace);
	if (ret) {
		LIBLCD_ERR("cap create");
		goto fail2;
	}
	return 0;
fail2:
	glue_cap_exit();
fail1:
	return ret;

}

void glue_skx_edac_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();

}

int inline glue_insert_pdev_hash(struct pci_dev_container *dev_container)
{
	BUG_ON(!dev_container->pdev);

	dev_container->my_ref = __cptr((unsigned long)dev_container->pdev);

	hash_add(pdev_hash, &dev_container->hentry,
				(unsigned long) dev_container->pdev);

	return 0;
}

int inline glue_lookup_pdev_hash(struct cptr c, struct pci_dev_container
		**dev_container_out)
{
        struct pci_dev_container *dev_container;

        hash_for_each_possible(pdev_hash, dev_container,
				hentry, (unsigned long) cptr_val(c)) {
		if (!dev_container) {
			WARN_ON(!dev_container);
			continue;
		}
		if (dev_container->pdev == (struct pci_dev*) c.cptr) {
	                *dev_container_out = dev_container;
			break;
		}
        }
        return 0;
}

void inline glue_remove_pdev_hash(struct pci_dev_container *dev_container)
{
	hash_del(&dev_container->hentry);
}

int inline glue_insert_pbus_hash(struct pci_bus_container *bus_container)
{
	BUG_ON(!bus_container->pbus);

	bus_container->my_ref = __cptr((unsigned long)bus_container->pbus);

	hash_add(pbus_hash, &bus_container->hentry,
				(unsigned long) bus_container->pbus);

	return 0;
}

int inline glue_lookup_pbus_hash(struct cptr c, struct pci_bus_container
		**bus_container_out)
{
        struct pci_bus_container *bus_container;

        hash_for_each_possible(pbus_hash, bus_container,
				hentry, (unsigned long) cptr_val(c)) {
		if (!bus_container) {
			WARN_ON(!bus_container);
			continue;
		}
		if (bus_container->pbus == (struct pci_bus*) c.cptr) {
	                *bus_container_out = bus_container;
			break;
		}
        }
        return 0;
}

void inline glue_remove_pbus_hash(struct pci_bus_container *bus_container)
{
	hash_del(&bus_container->hentry);
}

#if 0
struct dentry *debugfs_create_dir(char name,
		struct dentry *parent)
{
	struct dentry_container *parent_container = NULL;
	struct dentry_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	parent_container = container_of(parent,
		struct dentry_container,
		dentry);
	ret = glue_cap_insert_dentry_type(c_cspace,
		parent_container,
		&parent_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	func_ret_container = kzalloc(sizeof( struct dentry_container   ),
		GFP_KERNEL);
	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = &func_ret_container->dentry;
	ret = glue_cap_insert_dentry_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	async_msg_set_fn_type(_request,
			DEBUGFS_CREATE_DIR);
	fipc_set_reg1(_request,
			name);
	fipc_set_reg5(_request,
			parent_container->my_ref.cptr);
	ret = vmfunc_wrapper(_request);
	parent_container->other_ref.cptr = fipc_get_reg5(_response);
	return func_ret;

}

struct dentry *debugfs_create_file(char name,
		unsigned int mode,
		struct dentry *parent,
		int data,
		struct file_operations *fops)
{
	struct file_operations_container *fops_container = NULL;
	struct dentry_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	fops_container = container_of(fops,
		struct file_operations_container,
		file_operations);
	ret = glue_cap_insert_file_operations_type(c_cspace,
		fops_container,
		&fops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	async_msg_set_fn_type(_request,
			DEBUGFS_CREATE_FILE);
	fipc_set_reg1(_request,
			name);
	fipc_set_reg2(_request,
			mode);
	fipc_set_reg3(_request,
			data);
	fipc_set_reg7(_request,
			fops_container->my_ref.cptr);
	ret = vmfunc_wrapper(_request);
	fops_container->other_ref.cptr = fipc_get_reg7(_response);
	return func_ret;

}

void debugfs_remove_recursive(struct dentry *dentry)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			DEBUGFS_REMOVE_RECURSIVE);
	ret = vmfunc_wrapper(_request);
	return;

}
#endif

int edac_mc_add_mc_with_groups(struct mem_ctl_info *mci,
		struct attribute_group **groups)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			EDAC_MC_ADD_MC_WITH_GROUPS);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

struct mem_ctl_info *edac_mc_alloc(unsigned int mc_num,
		unsigned int n_layers,
		struct edac_mc_layer *layers,
		unsigned int sz_pvt)
{
	struct mem_ctl_info_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct ext_registers *_regs = get_register_page(smp_processor_id());
	uint64_t *regs = &_regs->regs[0];

	async_msg_set_fn_type(_request, EDAC_MC_ALLOC);
	regs[i++] = mc_num;
	regs[i++] = n_layers;

	/* marshal layers array */
	/* marshal array size */
	regs[i++] = n_layers;
	for (i = 0; i < n_layers; i++) {
		/* marshall element */
		regs[i++] = layers[i].type;
		regs[i++] = layers[i].size;
		regs[i++] = layers[i].is_virt_csrow;
	}

	regs[i++] = sz_pvt;
	ret = vmfunc_wrapper(_request);

	{

#define MAX_OBJS	(4096/sizeof(struct hash))
#define INIT_VAL(v)	memset(v, 0, sizeof(typeof(*v)))
	struct hash hash = { 0 };
	struct value v;
	i = 0;
	type_id type;
	type = regs[i++];
	//create_object(type);

	struct mem_ctl_info_container *mci_container;

	mci_container = kzalloc(sizeof(*mci_container), GFP_KERNEL);

	if (!mci_container) {
		LIBLCD_ERR("alloc failed");
		goto fail_alloc;
	}

	mci_container->mci = kzalloc(sizeof(*mci_container->mci), GFP_KERNEL);

	if (!mci_container->mci) {
		LIBLCD_ERR("alloc failed");
		goto fail_alloc;
	}
	INIT_VAL(&v);
	/* struct mem_ctl_info ptr (root object) */
	v.dtype = POINTER;
	v.type = MEM_CTL_INFO;
	idx = regs[i++];
	insert_at(ht, (void*) mci_container->mci, &v, idx);

	u32 n_layers = regs[i++];

	mci->n_layers =	regs[i++];
	mci->layers = kzalloc(sizeof(mci->layers*) * n_layers, GFP_KERNEL);

	if (!mci->layers) {
		LIBLCD_ERR("alloc failed");
		goto fail_alloc;
	}

	mci->tot_dimms = regs[i++];

	mci->dimms = kzalloc(sizeof(mci->dimms) * mci->tot_dimms, GFP_KERNEL);

	if (!mci->dimms) {
		LIBLCD_ERR("alloc failed");
		goto fail_alloc;
	}

	for (j = 0; j < mci->tot_dimms; j++) {
		mc->dimms[j] = kzalloc(sizeof(**mci->dimms), GFP_KERNEL);
	}

	for (j = 0; j < n_layers; j++);
	/* struct dimm_info dimm; (array element) */
	for (j = 0; j < mci->tot_dimms; j++) {
		struct dimm_info *dimm = mci->dimms[j];
		u32 ref_id;

		{INIT_VAL(&v);
		v.dtype = POINTER;
		v.type = DIMM_INFO;

		/* marshalling dimm */
		if (!find(ht, (void*) dimm)) {
			insert(ht, (void*) dimm, &v);
		}

		/* marshalling dimm->mci */
		if (!find(ht, (void*) dimm->mci)) {
			INIT_VAL(&v);
			v.dtype = POINTER;
			v.type = MEM_CTL_INFO
			insert(ht, (void*) dimm->mci, MEM_CTL_INFO);
		}
	}

	}
	return func_ret;

}

struct mem_ctl_info *edac_mc_del_mc(struct device *dev)
{
	struct mem_ctl_info_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			EDAC_MC_DEL_MC);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

void edac_mc_free(struct mem_ctl_info *mci)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			EDAC_MC_FREE);
	ret = vmfunc_wrapper(_request);
	return;

}

void edac_mc_handle_error(int type,
		struct mem_ctl_info *mci,
		unsigned short error_count,
		unsigned long page_frame_number,
		unsigned long offset_in_page,
		unsigned long syndrome,
		int low_layer)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			EDAC_MC_HANDLE_ERROR);
	fipc_set_reg1(_request,
			type);
	fipc_set_reg2(_request,
			error_count);
	fipc_set_reg3(_request,
			page_frame_number);
	fipc_set_reg4(_request,
			offset_in_page);
	fipc_set_reg5(_request,
			syndrome);
	fipc_set_reg6(_request,
			low_layer);
	ret = vmfunc_wrapper(_request);
	return;

}

void mce_register_decode_chain(struct notifier_block *nb)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct notifier_block_container *nb_container;

	INIT_FIPC_MSG(_request);

	nb_container = container_of(nb, struct notifier_block_container,
					notifier_block);

	ret = glue_cap_insert_notifier_block_type(c_cspace, nb_container,
				&nb_container->my_ref);
	if (ret) {
		LIBLCD_MSG("cap insert failed %d", ret);
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, MCE_REGISTER_DECODE_CHAIN);

	fipc_set_reg0(_request, nb_container->my_ref.cptr);

	ret = vmfunc_wrapper(_request);

	nb_container->other_ref = __cptr(fipc_get_reg0(_request));

	return;
}

void mce_unregister_decode_chain(struct notifier_block *nb)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MCE_UNREGISTER_DECODE_CHAIN);
	ret = vmfunc_wrapper(_request);
	return;

}

struct pci_dev *pci_dev_get(struct pci_dev *dev)
{
	struct pci_dev_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container = NULL;

	glue_lookup_pdev_hash(__cptr((unsigned long) from), &device_container);

	fipc_set_reg0(_request, device_container->other_ref.cptr);

	async_msg_set_fn_type(_request, PCI_DEV_GET);

	ret = vmfunc_wrapper(_request);

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)), &func_ret_container);

	if (func_ret_container) {
		func_ret = func_ret_container->pdev;
	}

	return func_ret;
}

void pci_dev_put(struct pci_dev *dev)
{
	struct pci_dev_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container = NULL;

	glue_lookup_pdev_hash(__cptr((unsigned long) from), &device_container);

	fipc_set_reg0(_request, device_container->other_ref.cptr);

	async_msg_set_fn_type(_request, PCI_DEV_PUT);

	ret = vmfunc_wrapper(_request);

	return;
}

int pci_enable_device(struct pci_dev *dev)
{
	int func_ret = 0;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);

	glue_lookup_pdev_hash(__cptr((unsigned long) dev), &device_container);

	async_msg_set_fn_type(_request, PCI_ENABLE_DEVICE);

	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

struct pci_dev *pci_get_device(unsigned int vendor,
		unsigned int device,
		struct pci_dev *from)
{
	struct pci_dev_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *pdevc_from = NULL;

	async_msg_set_fn_type(_request,
			PCI_GET_DEVICE);

	glue_lookup_pdev_hash(__cptr((unsigned long) from), &pdevc_from);

	fipc_set_reg0(_request, vendor);
	fipc_set_reg1(_request, device);
	fipc_set_reg2(_request, pdevc_from->other_ref.cptr);

	ret = vmfunc_wrapper(_request);

	func_ret_container = kzalloc(*func_ret_container, GFP_KERNEL);

	if (func_ret_container) {
		func_ret_container->pdev = kzalloc(sizeof(struct pci_dev), GFP_KERNEL);
		if (!func_ret_container->pdev) {
			LIBLCD_ERR("kzalloc failed");
			goto exit;
		}
		func_ret_container->other_ref.cptr = fipc_get_reg0(_request);
		glue_insert_pdev_hash(func_ret_container);
	}

	return func_ret;

}

int pci_bus_read_config_word(struct pci_bus *bus,
		unsigned int devfn,
		int where,
		unsigned short *val)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	INIT_IPC_MSG(&r);
	async_msg_set_fn_type(_request, PCI_BUS_READ_CONFIG_WORD);

	glue_lookup_pdev_hash(__cptr((unsigned long) bus), &bus_container);

	if (bus_container) {
		fipc_set_reg0(_request, bus_container->other_ref.cptr);
	}
	fipc_set_reg1(_request, devfn);
	fipc_set_reg2(_request, where);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	*val = fipc_get_reg1(_request);

	return func_ret;
}

#if 0
long long generic_file_llseek(struct file *file,
		long long offset,
		int whence)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			GENERIC_FILE_LLSEEK);
	fipc_set_reg1(_request,
			offset);
	fipc_set_reg2(_request,
			whence);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

unsigned long simple_attr_read(struct file *file,
		char *buf,
		unsigned int len,
		long long *ppos)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			SIMPLE_ATTR_READ);
	fipc_set_reg1(_request,
			buf);
	fipc_set_reg2(_request,
			len);
	fipc_set_reg3(_request,
			ppos);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int simple_attr_release(struct inode *inode,
		struct file *file)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			SIMPLE_ATTR_RELEASE);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int simple_attr_write(struct file *file,
		char *buf,
		unsigned int len,
		long long *ppos)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			SIMPLE_ATTR_WRITE);
	fipc_set_reg1(_request,
			buf);
	fipc_set_reg2(_request,
			len);
	fipc_set_reg3(_request,
			ppos);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int simple_attr_open(struct inode *inode,
		struct file *file,
		int ( *get )(void),
		int ( *set )(void),
		char fmt)
{
	struct get_container *get_container = NULL;
	struct set_container *set_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	get_container = kzalloc(sizeof( struct get_container   ),
		GFP_KERNEL);
	if (!get_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ret = glue_cap_insert_get_type(c_cspace,
		get_container,
		&get_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	get_container->get = get;
	set_container = kzalloc(sizeof( struct set_container   ),
		GFP_KERNEL);
	if (!set_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ret = glue_cap_insert_set_type(c_cspace,
		set_container,
		&set_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	set_container->set = set;
	async_msg_set_fn_type(_request,
			SIMPLE_ATTR_OPEN);
	fipc_set_reg1(_request,
			fmt);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int get_callee(struct fipc_message *_request)
{
	int p = 0;
	unsigned 	long 	long p1 = 0;
	int ret = 0;
	int func_ret = 0;
	p = fipc_get_reg1(_request);
	p1 = fipc_get_reg2(_request);
	func_ret = get_container->get(p,
		p1);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int set_callee(struct fipc_message *_request)
{
	int p = 0;
	unsigned 	long 	long p1 = 0;
	int ret = 0;
	int func_ret = 0;
	p = fipc_get_reg1(_request);
	p1 = fipc_get_reg2(_request);
	func_ret = set_container->set(p,
		p1);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}
#endif

struct x86_cpu_id *x86_match_cpu(struct x86_cpu_id *match)
{
	struct x86_cpu_id_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			X86_MATCH_CPU);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}
