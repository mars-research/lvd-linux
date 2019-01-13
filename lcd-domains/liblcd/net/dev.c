#include <lcd_config/pre_hook.h>

#include <linux/netdevice.h>

#include <liblcd/netdev_helper.h>

#include <lcd_config/post_hook.h>

static void netdev_init_one_queue(struct net_device *dev,
				  struct netdev_queue *queue, void *_unused)
{
	/* Initialize queue lock */
	spin_lock_init(&queue->_xmit_lock);
#ifndef LCD_ISOLATE
	netdev_set_xmit_lockdep_class(&queue->_xmit_lock, dev->type);
#endif
	queue->xmit_lock_owner = -1;
	netdev_queue_numa_node_write(queue, NUMA_NO_NODE);
	queue->dev = dev;
#ifdef CONFIG_BQL
	dql_init(&queue->dql, HZ);
#endif
}

int netif_alloc_netdev_queues(struct net_device *dev)
{
	unsigned int count = dev->num_tx_queues;
	struct netdev_queue *tx;
	size_t sz = count * sizeof(*tx);

	if (count < 1 || count > 0xffff)
		return -EINVAL;

	tx = kzalloc(sz, GFP_KERNEL | __GFP_NOWARN | __GFP_REPEAT);
#ifndef LCD_ISOLATE
	if (!tx) {
		tx = vzalloc(sz);
#endif
		if (!tx)
			return -ENOMEM;
#ifndef LCD_ISOLATE
	}
#endif
	dev->_tx = tx;

	netdev_for_each_tx_queue(dev, netdev_init_one_queue, NULL);
	spin_lock_init(&dev->tx_global_lock);

	return 0;
}
EXPORT_SYMBOL(netif_alloc_netdev_queues);
