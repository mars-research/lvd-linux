#ifndef _LIBLCD_NETDEV_HELPER_H_
#define _LIBLCD_NETDEV_HELPER_H_

int liblcd__hw_addr_sync_dev(struct netdev_hw_addr_list *list,
		       struct net_device *dev,
		       int (*sync)(struct net_device *, const unsigned char *),
		       int (*unsync)(struct net_device *,
				     const unsigned char *));

void liblcd__hw_addr_unsync_dev(struct netdev_hw_addr_list *list,
			  struct net_device *dev,
			  int (*unsync)(struct net_device *,
					const unsigned char *));

int netif_alloc_netdev_queues(struct net_device *dev);

int liblcd_dev_addr_add(struct net_device *dev,
		const unsigned char *addr,
		unsigned char addr_type);

int liblcd_dev_addr_del(struct net_device *dev,
		const unsigned char *addr,
		unsigned char addr_type);

int netif_alloc_netdev_queues(struct net_device *dev);

#endif /* _LIBLCD_NETDEV_HELPER_H_ */
