#include <linux/debugfs.h>

struct dentry *lvd_debug_dir;
DECLARE_PER_CPU(unsigned long long, vmfunc_counter);
DECLARE_PER_CPU(unsigned long long, vmfunc_irq_counter);
DECLARE_PER_CPU(unsigned long long, irq_counter);
DECLARE_PER_CPU(unsigned long long, exception_counter);

static int vmfunc_stats_show(struct seq_file *s, void *data)
{
	int cpu = smp_processor_id();
	seq_printf(s, "stats for cpu %d\n", cpu);
	seq_puts(s, "------------------\n");

	seq_printf(s, "vmfunc_count: %llu\n", per_cpu(vmfunc_counter, cpu));
	seq_printf(s, "vmfunc_irq_count: %llu\n", per_cpu(vmfunc_irq_counter, cpu));
	seq_printf(s, "total_irq_count: %llu\n", per_cpu(irq_counter, cpu));
	seq_printf(s, "exception_count: %llu\n", per_cpu(exception_counter, cpu));

	return 0;
}


static int vmfunc_stats_open(struct inode *inode, struct file *file)
{
	return single_open(file, vmfunc_stats_show, inode->i_private);
}

static const struct file_operations vmfunc_stats_fops = {
	.open		= vmfunc_stats_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

int lcd_debugfs_init(void)
{
	int ret = -ENOMEM;
	struct dentry *d;
	lvd_debug_dir = debugfs_create_dir("lvd_stats", NULL);

	if (!lvd_debug_dir) {
		printk("%s, debugfs_create_dir failed\n", __func__);
		goto out;
	}

	d = debugfs_create_file("vmfunc_count", S_IRUGO, lvd_debug_dir, NULL,
					&vmfunc_stats_fops);
	if (!d) {
		printk("%s, debugfs_create_file failed\n", __func__);
		goto fail;
	}

	ret = 0;
	goto out;
fail:
	debugfs_remove_recursive(lvd_debug_dir);
out:
	return ret;
}

void lcd_debugfs_exit(void)
{
	if (lvd_debug_dir)
		debugfs_remove_recursive(lvd_debug_dir);
}
