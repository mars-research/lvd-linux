#ifndef PRIV_MEMPOOL_H
#define PRIV_MEMPOOL_H

#define MTU                     1470
#define HEADERS                 42
#define SKB_LCD_MEMBERS_SZ	48
#define SKB_SHARED_INFO         (sizeof(struct skb_shared_info))
#define DATA_ALIGNED_SZ   	(SKB_DATA_ALIGN(MTU + HEADERS + SKB_LCD_MEMBERS_SZ))
#define SKB_DATA_SIZE           (DATA_ALIGNED_SZ + SKB_DATA_ALIGN(SKB_SHARED_INFO))

#define SKB_CONTAINER_SIZE	128

typedef enum {
	/* for skb->data */
	SKB_DATA_POOL = 0,

	/* for skb->page_frag */
	SKB_FRAG_POOL,

	/* for skb_container */
	SKB_CONTAINER_POOL,

	POOL_MAX,
} pool_type_t;

struct object {
	struct object *next;
};

struct bundle {
	struct object *list;
	struct bundle *next;
};

struct atom {
	struct bundle *head;
	long version;
} __attribute__((aligned(16)));

typedef struct {
	struct object __percpu **head;
	struct object __percpu **marker;
	char __percpu **buf;
	char __percpu **bufend;
	int __percpu *cached;
	unsigned int obj_size;
	unsigned int total_pages;
	unsigned int num_objs_percpu;
	unsigned int num_cpus;
	void *pool;
	void *gpool;
	struct atom stack;
	unsigned int pool_order;
	spinlock_t pool_spin_lock;
} priv_pool_t;


void *priv_alloc(pool_type_t type);
void priv_free(void *p, pool_type_t type);
priv_pool_t *priv_pool_init(pool_type_t type, unsigned int num_objs, unsigned int obj_size);
void priv_pool_destroy(priv_pool_t *p);

#endif /* PRIV_MEMPOOL_H */
