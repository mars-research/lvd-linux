#include "../skx_edac_callee.h"
#include <liblcd/trampoline.h>
static struct cptr c;
static struct glue_cspace *c_cspace;

#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(pbus_hash, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(pdev_hash, CPTR_HASH_BITS);

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
int debugfs_create_dir_callee(struct fipc_message *_request)
{
	char name = 0;
	struct dentry_container *parent_container = NULL;
	struct dentry *parent = NULL;
	int ret = 0;
	struct dentry_container *func_ret_container = NULL;
	struct dentry *func_ret = NULL;
	parent_container = kzalloc(sizeof( struct dentry_container   ),
		GFP_KERNEL);
	if (!parent_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ret = glue_cap_insert_dentry_type(c_cspace,
		parent_container,
		&parent_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	parent_container->other_ref.cptr = fipc_get_reg5(_request);
	name = fipc_get_reg1(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg3(_request);
	func_ret = debugfs_create_dir(name,
		( &parent_container->dentry ));
	ret = glue_cap_insert_dentry_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg4(_response,
			parent_container->other_ref.cptr);
	fipc_set_reg2(_response,
			func_ret_container->other_ref.cptr);
fail_alloc:
	return ret;

}

int debugfs_create_file_callee(struct fipc_message *_request)
{
	char name = 0;
	unsigned 	int mode = 0;
	struct dentry *parent = NULL;
	int data = 0;
	struct file_operations_container *fops_container = NULL;
	struct file_operations *fops = NULL;
	int ret = 0;
	struct dentry_container *func_ret_container = NULL;
	struct dentry *func_ret = NULL;
	fops_container = kzalloc(sizeof( struct file_operations_container   ),
		GFP_KERNEL);
	if (!fops_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ret = glue_cap_insert_file_operations_type(c_cspace,
		fops_container,
		&fops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fops_container->other_ref.cptr = fipc_get_reg7(_request);
	parent = kzalloc(sizeof( *parent ),
		GFP_KERNEL);
	if (!parent) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	name = fipc_get_reg1(_request);
	mode = fipc_get_reg2(_request);
	data = fipc_get_reg3(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg3(_request);
	func_ret = debugfs_create_file(name,
		mode,
		parent,
		data,
		( &fops_container->file_operations ));
	ret = glue_cap_insert_dentry_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg6(_response,
			fops_container->other_ref.cptr);
	fipc_set_reg2(_response,
			func_ret_container->other_ref.cptr);
fail_alloc:
	return ret;

}

int debugfs_remove_recursive_callee(struct fipc_message *_request)
{
	struct dentry *dentry = NULL;
	int ret = 0;
	dentry = kzalloc(sizeof( *dentry ),
		GFP_KERNEL);
	if (!dentry) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	debugfs_remove_recursive(dentry);
fail_alloc:
	return ret;

}
#endif

int edac_mc_add_mc_with_groups_callee(struct fipc_message *_request)
{
	struct mem_ctl_info *mci = NULL;
	struct attribute_group **groups = NULL;
	int ret = 0;
	int func_ret = 0;
	mci = kzalloc(sizeof( *mci ),
		GFP_KERNEL);
	if (!mci) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	groups = kzalloc(sizeof( void  * ),
		GFP_KERNEL);
	if (!groups) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc2;
	}
	( *groups ) = kzalloc(sizeof( *( *groups ) ),
		GFP_KERNEL);
	if (!( *groups )) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = edac_mc_add_mc_with_groups(mci,
		( *groups ));
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int edac_mc_alloc_callee(struct fipc_message *_request)
{
	unsigned 	int mc_num = 0;
	unsigned 	int n_layers = 0;
	struct edac_mc_layer *layers = NULL;
	unsigned 	int sz_pvt = 0;
	int ret = 0;
	struct mem_ctl_info_container *mci_container = NULL;
	struct mem_ctl_info *mci = NULL;
	struct ext_registers *ext_regs = get_register_page(smp_processor_id());

	mc_num = fipc_get_reg0(_request);
	n_layers = fipc_get_reg1(_request);

	layers = kzalloc(sizeof(*layers ) * n_layers, GFP_KERNEL);

	if (!layers) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	for (i = 0; i < n_layers; i++) {
		layers[i].type = ext_regs[i*3+0];
		layers[i].size = ext_regs[i*3+1];
		layers[i].is_virt_csrow = ext_regs[i*3+2];
	}

	sz_pvt = fipc_get_reg2(_request);

	mci = edac_mc_alloc(mc_num,
		n_layers,
		layers,
		sz_pvt);
	mci_container = kzalloc(sizeof(*mci_container), GFP_KERNEL);

	if (!mci_container) {
		LIBLCD_ERR("alloc failed");
		goto fail_alloc;
	}

	glue_insert_mci(&mci_container);

	fipc_set_reg0(_response, mci_container->my_ref.cptr);

	/* fill kson objects */
	/* struct mem_ctl_info ptr (root object) */
	kson_data[objs].obj_type = POINTER;
	kson_data[objs].type_id = MEM_CTL_INFO;
	kson_data[objs].ref = 0;
	kson_data[objs].obj_id = objs++;
	
	/* struct csrow_info **csrows; (array) */
	kson_data[objs].obj_type = ARRAY;
	kson_data[objs].type_id = CSROW_INFO;
	kson_data[objs].num_elements = mci->nr_csrows;
	kson_data[objs].obj_id = objs++;

	/* struct csrow_info csrow; (array element) */
	for (i = 0; i < mci->nr_csrows; i++) {
		kson_data[objs++].obj_type = ARRAY_ELEMENT;
		kson_data[objs++].type_id = CSROW_INFO;
		kson_data[objs].obj_id = objs++;
	}

	/* struct csrow_info csrow; */
	for (i = 0; i < mci->nr_csrows; i++) {
		kson_data[objs++].obj_type = ARRAY_ELEMENT;
		kson_data[objs++].type_id = CSROW_INFO;
		kson_data[objs].obj_id = objs++;
	}
	/* first in the object tree */
	if (mci->cs_rows) {
		ext_regs[reg++] = mci->nr_csrows;
		ext_regs[reg++] = 
	}

	if (mci->layers) {
		
	}
fail_alloc:
	return ret;
}

int edac_mc_del_mc_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	int ret = 0;
	struct mem_ctl_info_container *func_ret_container = NULL;
	struct mem_ctl_info *func_ret = NULL;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = edac_mc_del_mc(dev);
	ret = glue_cap_insert_mem_ctl_info_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_response,
			func_ret_container->other_ref.cptr);
fail_alloc:
	return ret;

}

int edac_mc_free_callee(struct fipc_message *_request)
{
	struct mem_ctl_info *mci = NULL;
	int ret = 0;
	mci = kzalloc(sizeof( *mci ),
		GFP_KERNEL);
	if (!mci) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	edac_mc_free(mci);
fail_alloc:
	return ret;

}

int edac_mc_handle_error_callee(struct fipc_message *_request)
{
	int type = 0;
	struct mem_ctl_info *mci = NULL;
	unsigned 	short error_count = 0;
	unsigned 	long page_frame_number = 0;
	unsigned 	long offset_in_page = 0;
	unsigned 	long syndrome = 0;
	int low_layer = 0;
	int ret = 0;
	mci = kzalloc(sizeof( *mci ),
		GFP_KERNEL);
	if (!mci) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	type = fipc_get_reg1(_request);
	error_count = fipc_get_reg2(_request);
	page_frame_number = fipc_get_reg3(_request);
	offset_in_page = fipc_get_reg4(_request);
	syndrome = fipc_get_reg5(_request);
	low_layer = fipc_get_reg6(_request);
	edac_mc_handle_error(type,
			mci,
			error_count,
			page_frame_number,
			offset_in_page,
			syndrome,
			low_layer);
fail_alloc:
	return ret;

}

int mce_register_decode_chain_callee(struct fipc_message *_request)
{
	struct notifier_block_container *nb_container = NULL;
	int ret = 0;
	int func_ret = 0;
	struct trampoline_hidden_args *notifier_hidden_args;

	nb_container = kzalloc(sizeof(struct notifier_block_container),
			GFP_KERNEL);
	if (!nb_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	notifier_hidden_args = kzalloc(sizeof( *notifier_hidden_args ),
			GFP_KERNEL);

	if (!notifier_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc;
	}

	notifier_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(notifier_call_trampoline);

	if (!notifier_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup;
	}

	notifier_hidden_args->t_handle->hidden_args = notifier_hidden_args;
	notifier_hidden_args->struct_container = nb_container;
	nb_container->notifier_block.notifier_call = LCD_HANDLE_TO_TRAMPOLINE(
				notifier_hidden_args->t_handle);
	ret = set_memory_x(((unsigned  long)notifier_hidden_args->t_handle)
				& (PAGE_MASK),
				(ALIGN(LCD_TRAMPOLINE_SIZE(notifier_call_trampoline),
				PAGE_SIZE)) >> ( PAGE_SHIFT ));

	ret = glue_cap_insert_notifier_block_type(c_cspace, nb_container,
				&nb_container->my_ref);
	if (ret) {
		LIBLCD_MSG("cap insert failed %d", ret);
		goto fail_insert;
	}

	nb_container->other_ref = __cptr(fipc_get_reg0(_request));
	mce_register_decode_chain(&nb_container->notifier_block);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, nb_container->my_ref.cptr);
fail_alloc:
fail_dup:
fail_insert:
	return ret;
}

int mce_unregister_decode_chain_callee(struct fipc_message *_request)
{
	struct notifier_block_container *nb_container = NULL;
	struct notifier_block *nb = NULL;
	int ret = 0;

	ret = glue_cap_lookup_notifier_block_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&nb_container);
	if (ret) {
		LIBLCD_MSG("cap lookup failed %d", ret);
		goto fail_lookup;
	}

	nb = &nb_container->notifier_block;

	mce_unregister_decode_chain(&nb_container->notifier_block);
fail_looku:
	return ret;
}

int pci_dev_get_callee(struct fipc_message *_request)
{
	struct pci_dev *dev = NULL;
	int ret = 0;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *func_ret = NULL;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (dev_container) {
		dev = dev_container->pdev;
		func_ret = pci_dev_get(dev);
	}

	if (func_ret) {
		struct pci_dev_container *func_ret_container = NULL;

		glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
				&func_ret_container);
		if (!func_ret_container) {
			func_ret_container = kzalloc(*func_ret_container, GFP_KERNEL);

			if (!func_ret_container) {
				LIBLCD_ERR("alloc failed");
				goto fail_alloc;
			}
			func_ret_container->pdev = func_ret;
			glue_insert_pdev_hash(func_ret_container);
		}

		fipc_set_reg0(_response, func_ret_container->my_ref.cptr);
	}
fail_alloc:
	return ret;
}

int pci_dev_put_callee(struct fipc_message *_request)
{
	struct pci_dev *dev = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	pci_dev_put(dev);
fail_alloc:
	return ret;

}

int pci_enable_device_callee(struct fipc_message *_request)
{
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	func_ret = pci_enable_device(pdev);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_get_device_callee(struct fipc_message *_request)
{
	unsigned 	int vendor = 0;
	unsigned 	int device = 0;
	struct pci_dev *from = NULL;
	int ret = 0;
	struct pci_dev_container *func_ret_container = NULL;
	struct pci_dev_container *from_container = NULL;
	struct pci_dev *func_ret = NULL;

	glue_lookup_pdev_hash(__cptr((unsigned long) fipc_get_reg0(_request)), &from_container);

	if (from_container) {
		from = from_container->pdev;
	}

	vendor = fipc_get_reg1(_request);
	device = fipc_get_reg2(_request);

	func_ret = pci_get_device(vendor, device, from);

	if (func_ret) {
		func_ret_container = kzalloc(*func_ret_container, GFP_KERNEL);
		pbus_container = kzalloc(*pbus_container, GFP_KERNEL);

		if (func_ret_container && pbus_container) {
			func_ret_container->pdev = func_ret;
			glue_insert_pdev_hash(func_ret_container);

			pbus_container->pbus = func_ret->bus;
			glue_insert_pbus_hash(pbus_container);

			fipc_set_reg0(_request, func_ret_container->my_ref.cptr);
			fipc_set_reg1(_request, pbus_container->my_ref.cptr);
		}
	}

fail_alloc:
	return ret;
}

int pci_bus_read_config_word_callee(struct fipc_message *_request)
{
	struct pci_bus *bus;
	struct pci_bus_container *bus_container = NULL;
	unsigned int devfn;
	int where;
	unsigned short val;
	int func_ret;

	glue_lookup_pbus_hash(__cptr(fipc_get_reg0(_request)), &bus_container);

	if (!bus_container) {
		LIBLCD_ERR("couldn't locate pci_bus in pbus_hash");
		goto fail_find;
	}

	bus = pdev->bus;

	devfn = fipc_get_reg1(_request);
	where = fipc_get_reg2(_request);

	func_ret = pci_bus_read_config_word(bus, devfn, where, &val);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, val);

fail_find:
	return 0;
}

#if 0
int generic_file_llseek_callee(struct fipc_message *_request)
{
	struct file *file = NULL;
	long 	long offset = 0;
	int whence = 0;
	int ret = 0;
	long 	long func_ret = 0;
	file = kzalloc(sizeof( *file ),
		GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	offset = fipc_get_reg1(_request);
	whence = fipc_get_reg2(_request);
	func_ret = generic_file_llseek(file,
		offset,
		whence);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int simple_attr_read_callee(struct fipc_message *_request)
{
	struct file *file = NULL;
	char *buf = 0;
	unsigned 	int len = 0;
	long 	long *ppos = 0;
	int ret = 0;
	unsigned 	long func_ret = 0;
	file = kzalloc(sizeof( *file ),
		GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	buf = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!buf) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	ppos = kzalloc(sizeof( long  long   ),
		GFP_KERNEL);
	if (!ppos) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	buf = fipc_get_reg1(_request);
	len = fipc_get_reg2(_request);
	ppos = fipc_get_reg3(_request);
	func_ret = simple_attr_read(file,
		buf,
		len,
		ppos);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int simple_attr_release_callee(struct fipc_message *_request)
{
	struct inode *inode = NULL;
	struct file *file = NULL;
	int ret = 0;
	int func_ret = 0;
	inode = kzalloc(sizeof( *inode ),
		GFP_KERNEL);
	if (!inode) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	file = kzalloc(sizeof( *file ),
		GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = simple_attr_release(inode,
		file);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int simple_attr_write_callee(struct fipc_message *_request)
{
	struct file *file = NULL;
	char *buf = 0;
	unsigned 	int len = 0;
	long 	long *ppos = 0;
	int ret = 0;
	int func_ret = 0;
	file = kzalloc(sizeof( *file ),
		GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	buf = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!buf) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	ppos = kzalloc(sizeof( long  long   ),
		GFP_KERNEL);
	if (!ppos) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	buf = fipc_get_reg1(_request);
	len = fipc_get_reg2(_request);
	ppos = fipc_get_reg3(_request);
	func_ret = simple_attr_write(file,
		buf,
		len,
		ppos);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int simple_attr_open_callee(struct fipc_message *_request)
{
	struct inode *inode = NULL;
	struct file *file = NULL;
	struct get_container *get_container = NULL;
*get = NULL;
	struct set_container *set_container = NULL;
*set = NULL;
	char fmt = 0;
	int ret = 0;
	int func_ret = 0;
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
	inode = kzalloc(sizeof( *inode ),
		GFP_KERNEL);
	if (!inode) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	file = kzalloc(sizeof( *file ),
		GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	fmt = fipc_get_reg1(_request);
	func_ret = simple_attr_open(inode,
		file,
		( &get_container->get ),
		( &set_container->set ),
		fmt);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int get(int p,
		unsigned long long p1,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			GET);
	fipc_set_reg1(_request,
			p);
	fipc_set_reg2(_request,
			p1);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int set(int p,
		unsigned long long p1,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			SET);
	fipc_set_reg1(_request,
			p);
	fipc_set_reg2(_request,
			p1);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}
#endif

int x86_match_cpu_callee(struct fipc_message *_request)
{
	struct x86_cpu_id *match = NULL;
	int ret = 0;
	struct x86_cpu_id_container *func_ret_container = NULL;
	struct x86_cpu_id *func_ret = NULL;
	match = kzalloc(sizeof( *match ),
		GFP_KERNEL);
	if (!match) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = x86_match_cpu(match);
	ret = glue_cap_insert_x86_cpu_id_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_response,
			func_ret_container->other_ref.cptr);
fail_alloc:
	return ret;

}

int notifier_call(struct notifier_block *nb,
		unsigned long action,
		void *hcpu,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct notifier_block_container *nb_container;

	nb_container = hidden_args->struct_container;

	async_msg_set_fn_type(_request, NOTIFIER_FN_T);

	fipc_set_reg0(_request, nb_container->other_ref.cptr);
	fipc_set_reg1(_request, action);
	fipc_set_reg2(_request, (unsigned long) hcpu);

	ret = vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(notifier_call_trampoline);
int LCD_TRAMPOLINE_LINKAGE(notifier_call_trampoline)
notifier_call_trampoline(struct notifier_block *nb,
		unsigned long action,
		void *data)
{
	int ( *volatile notifier_call_fp )(struct notifier_block *,
			unsigned long, void *,
			struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			notifier_call_trampoline);
	notifier_call_fp = notifier_call;
	return notifier_call_fp(nb, action, data, hidden_args);
}
