#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/priv_mempool.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/percpu.h>
#include <linux/cma.h>

#undef pr_fmt
#define pr_fmt(fmt)	"%s:%d : " fmt, __func__, smp_processor_id()

//#define CMA_ALLOC
static priv_pool_t pool_array[POOL_MAX];

static unsigned long pool_cma_size = 1024 * 1024 * 50;
static struct cma *pool_cma;

static int __init early_parse_pool_cma(char *p)
{
	pool_cma_size = ALIGN(memparse(p, NULL), PAGE_SIZE);
	return 0;
}
early_param("pool_cma", early_parse_pool_cma);

static priv_pool_t *get_pool(pool_type_t type)
{

	if (type < 0 || type > POOL_MAX) {
		printk("%s, unknown type requested %d\n", __func__, type);
		return NULL;
	}
	return &pool_array[type];
}

#define CACHE_SIZE	0x80

void construct_global_pool(priv_pool_t *p)
{
	unsigned int obj_size = p->obj_size;
	unsigned int gpool_objs = p->total_objs;

	char *gpool = p->gpool;
	int i, b;
	char *bpool = gpool;
	unsigned int bundles;
	struct object *objs;
	struct bundle *prev_bund = NULL, *bund = NULL;

	/* Let's have each bundle of size = CACHE_SIZE
	 * That would give us the number of bundles
	 */
	bundles = gpool_objs / CACHE_SIZE;

	printk("%s, gpool_objs %d | bundles %u\n",
		__func__, gpool_objs, bundles);

	for (b = 0; b < bundles; b++) {
		//printk("bundle ===> %d\n", b);
		for (i = 0; i < CACHE_SIZE; i++) {
			objs = (struct object*)((char*)bpool + (i * obj_size));
			/* the last object's next is just null */
			if (i == CACHE_SIZE - 1)
				break;
			objs->next = (struct object*)((char*)bpool + (i + 1) * obj_size);
	//		printk("\tobj %p | obj->next %p\n", objs, objs->next);
		}
		/* break the last object's chain */
		objs->next = NULL;
		/* point to our first object in the list */
		objs = (struct object *) bpool;
		/* our new bundle is the first element of the list */
		bund = (struct bundle *)objs;

		/* assign the list we created to this bundle */
		bund->list = (struct object *)objs->next;

		printk("prev_bund %p | bund %p\n",
			prev_bund, bund);
		/* if prev_bundle is constructed, chain it */
		if (prev_bund)
			prev_bund->next = bund;
		/* save prev bundle */
		prev_bund = bund;
		/* bpool should point to the next chunk */
		bpool += (CACHE_SIZE * obj_size);
	}

	/* the newly created bundle is the last in the chain */
	if (bund)
		bund->next = NULL;

	/* our top bundle is at bpool */
	p->stack.head = (struct bundle *)gpool;
	printk("stak head %p | head->list %p | head->next %p\n",
		p->stack.head, p->stack.head->list, p->stack.head->next);
}

void priv_pool_dumpstats(priv_pool_t *p)
{
	int cpu;
	printk("======================== priv pool stats ======================\n");

	for_each_online_cpu(cpu) {
	printk("[cpu:%d] pool %p | p->head %p | p->marker %p | p->cached %d\n",
			cpu, p,
			*per_cpu_ptr(p->head, cpu),
			*per_cpu_ptr(p->marker, cpu),
			*per_cpu_ptr(p->cached, cpu));
	}

	printk("global pool stack head %p | head->list %p | head->next %p\n",
		p->stack.head, p->stack.head->list, p->stack.head->next);
	p->dump_once = true;
	printk("===============================================================\n");
}

void priv_pool_destroy(priv_pool_t *p)
{
	if (!p)
		return;

	if (p->pool)
		free_pages((unsigned long)p->pool, p->pool_order);

#ifdef PBUF
	if (p->buf)
		free_percpu(p->buf);

	if (p->bufend)
		free_percpu(p->bufend);
#endif
	if (p->head)
		free_percpu(p->head);

	if (p->marker)
		free_percpu(p->marker);

	if (p->cached)
		free_percpu(p->cached);
}
EXPORT_SYMBOL(priv_pool_destroy);

#if 0
priv_pool_t *priv_pool_init(pool_type_t type, unsigned int num_objs,
            unsigned int obj_size)
{
	priv_pool_t *p;
	unsigned int num_pages, num_objs_percpu;
	char *pool, *global_pool;
#ifdef PBUF
	char *pcpu_pool;
#endif
#ifdef CMA_ALLOC
	struct page *page;
#endif
	unsigned int total_pages, num_cpus;
	int cpu;

	if (!num_objs || !obj_size) {
		printk("%s, invalid objsize (%u) or numobjs (%u) requested\n",
		            __func__, obj_size, num_objs);
		return NULL;
	}

	p = get_pool(type);

	if (!p)
		return NULL;

	memset(p, 0, sizeof(priv_pool_t));

#ifdef PBUF
	p->buf = alloc_percpu(char *);
	p->bufend = alloc_percpu(char *);
#endif
	p->head = alloc_percpu(struct object *);
	p->marker = alloc_percpu(struct object *);
	p->cached = alloc_percpu(int);

	/* align obj_size to 32 bit boundary */
	p->obj_size = obj_size = ALIGN(obj_size, 32);

	/* calculate num objs per cpu */
	p->num_objs_percpu = num_objs_percpu =
	        PAGE_ALIGN(num_objs * obj_size) / obj_size;

	/* calculate num_pages per cpu */
	num_pages = PAGE_ALIGN(num_objs * obj_size) / PAGE_SIZE;

	p->num_cpus = num_cpus = num_online_cpus() * 2;
	/* allocate twice the amount of requested pages
	 * one set is for the percpu buf, the remaining pages
	 * would be given to the global buffer
	 */
	//p->total_pages = total_pages = num_pages * (num_cpus + num_online_cpus() * 2);
	p->total_pages = total_pages = num_pages * num_cpus;
#ifdef PBUF
	printk("num objs %d | num_cpus %d | num_pages %d | num_objs_percpu %d "
		"| total_pages %d | page order %d\npcpu_pool %p | global_pool %p\n",
		num_objs, num_online_cpus(), num_pages, num_objs_percpu,
		total_pages, get_order(total_pages * PAGE_SIZE), pcpu_pool,
		global_pool);
#else
	printk("num objs %d | num_cpus %d | num_pages %d | num_objs_percpu %d "
		"| total_pages %d | page order %d\n | global_pool %p\n",
		num_objs, num_online_cpus(), num_pages, num_objs_percpu,
		total_pages, get_order(total_pages * PAGE_SIZE),
		global_pool);
#endif
	p->pool_order = min_t(int, MAX_ORDER,
	                        get_order(total_pages * PAGE_SIZE));

#ifdef CMA_ALLOC
	//page = cma_alloc(pool_cma, total_pages * 16, 0, GFP_KERNEL);
	page = cma_alloc(pool_cma, pool_cma_size >> PAGE_SHIFT, 0);
	pool = p->pool = page_address(page);
	p->total_objs = pool_cma_size / p->obj_size;
#else
	/* alloc total_size pages */
	pool = p->pool = (char*) __get_free_pages(GFP_KERNEL | __GFP_ZERO,
	                            p->pool_order);
#endif
	if (!pool) {
		printk("No memory %p\n", pool);
		return NULL;
	} else {
		printk("Memory %p | size %lu\n", pool, total_pages * PAGE_SIZE);
	}
	/* split the total pages between pcpu pool and the global pool */
#ifdef PBUF
	pcpu_pool = pool;
	p->gpool = global_pool =
	                pool + (num_cpus * num_pages * PAGE_SIZE);
#else
	p->gpool = global_pool = pool;
#endif

	/* update percpu vars */
	for_each_online_cpu(cpu) {
		*per_cpu_ptr(p->marker, cpu) =
		        *per_cpu_ptr(p->head, cpu) = (struct object*) NULL;
#ifdef PBUF
		*per_cpu_ptr(p->buf, cpu) =
		        pcpu_pool + (cpu * num_pages * PAGE_SIZE);
		*per_cpu_ptr(p->bufend, cpu) =
		        pcpu_pool + ((cpu + 1) * num_pages * PAGE_SIZE) - 1;
		*per_cpu_ptr(p->cached, cpu) = p->num_objs_percpu;
		*per_cpu_ptr(p->cached, cpu) = CACHE_SIZE;
		printk("cpu %d | buf %p | bufend %p\n",
				cpu, *per_cpu_ptr(p->buf, cpu),
				*per_cpu_ptr(p->bufend, cpu));
#endif
	}

	construct_global_pool(p);
	spin_lock_init(&p->pool_spin_lock);
	return p;
}
EXPORT_SYMBOL(priv_pool_init);
#endif

priv_pool_t *priv_pool_init(pool_type_t type, void *pool_base,
			size_t pool_size,
			unsigned int obj_size)
{
	priv_pool_t *p;
	char *pool, *global_pool;
	int cpu;

	if (!obj_size) {
		printk("%s, invalid objsize (%u) requested\n",
		            __func__, obj_size);
		return NULL;
	}

	p = get_pool(type);

	if (!p)
		return NULL;

	memset(p, 0, sizeof(priv_pool_t));

	p->head = alloc_percpu(struct object *);
	p->marker = alloc_percpu(struct object *);
	p->cached = alloc_percpu(int);

	/* align obj_size to 32 bit boundary */
	p->obj_size = obj_size = ALIGN(obj_size, 32);

	p->num_cpus = num_online_cpus() * 2;

	p->pool_order = 10;

	if (!pool_base) {
		/* alloc total_size pages */
		pool = p->pool = (char*) __get_free_pages(GFP_KERNEL | __GFP_ZERO,
	                            p->pool_order);
		p->total_objs = ((1 << p->pool_order) * PAGE_SIZE)
						/ p->obj_size;
		printk("Memory %p | size %lx\n", pool, (1 << p->pool_order) * PAGE_SIZE);
	} else {
		pool = p->pool = pool_base;
		p->total_objs = pool_size / p->obj_size;
		printk("Memory %p | size %lx\n", pool, pool_size);
		p->total_pages = pool_size >> PAGE_SHIFT;
	}

	if (!pool) {
		printk("No memory %p\n", pool);
		return NULL;
	}
	/* split the total pages between pcpu pool and the global pool */
	p->gpool = global_pool = pool;

	/* update percpu vars */
	for_each_online_cpu(cpu) {
		*per_cpu_ptr(p->marker, cpu) =
		        *per_cpu_ptr(p->head, cpu) = (struct object*) NULL;
	}

	construct_global_pool(p);
	spin_lock_init(&p->pool_spin_lock);
	return p;
}
EXPORT_SYMBOL(priv_pool_init);

void *priv_alloc(pool_type_t type)
{
	void *m = NULL;
#ifdef PBUF
	char *pbuf;
	char *pbufend;
#endif
	struct object *head;
	priv_pool_t *p = get_pool(type);

	if (!p)
		return NULL;

	/* disable preempt until we manipulate all percpu pointers */
	preempt_disable();

	head = (struct object*) *this_cpu_ptr(p->head);

	/* if head is not null */
	if (head) {
		pr_debug("from head %p | nhead %p\n", head, head->next);
		m = head;
		*this_cpu_ptr(p->head) = head->next;
		this_cpu_dec(*p->cached);
		goto out;
	} else {
		pr_debug("reset cached\n");
		this_cpu_write(*(p->cached), 0);
	}

#ifdef PBUF
	pbuf = (char*)*this_cpu_ptr(p->buf);
	pbufend = (char*)*this_cpu_ptr(p->bufend);

	if (pbuf + p->obj_size <= pbufend) {
		m = (void *)pbuf;

		this_cpu_add(*(unsigned long*)(p->buf), p->obj_size);

		pr_debug("from pbuf old: %p, new: %p | e: %p\n",
				pbuf,
				*this_cpu_ptr(p->buf),
				*this_cpu_ptr(p->bufend));

		/* got a chunk */
		goto out;
	}
#endif
	/* no cached or private pool chunk. Try our luck in the
	* global pool. First, check if global pool has any chunk
	* remaining
	*/
	if (!p->stack.head) {
		WARN_ONCE(!p->stack.head, "Out of memory! This must be crazy\n");
		if (!p->dump_once)
			priv_pool_dumpstats(p);
		goto out;
	}
	{
#ifndef SPINLOCK
		for(;;) {
#endif
		struct atom snapshot, new;

#ifdef SPINLOCK
		/* lock global pool */
		spin_lock(&p->pool_spin_lock);
#endif
		// this is old
		snapshot = p->stack;

		// prepare new
		new.head = snapshot.head->next;
		new.version = snapshot.version + 1;

#ifdef SPINLOCK
		// if cmpxchg is used, this is done automatically
		p->stack.head = new.head;
		p->stack.version = new.version;

		*this_cpu_ptr(p->head) = snapshot.head->list;
#ifdef PBUF
		this_cpu_write(*(p->cached), CACHE_SIZE);
#else
		this_cpu_write(*(p->cached), CACHE_SIZE - 1);
#endif
		/* unlock global pool */
		spin_unlock(&p->pool_spin_lock)
#else
		if (cmpxchg_double(&p->stack.head, &p->stack.version, snapshot.head, snapshot.version,
					new.head, new.version)) {
			*this_cpu_ptr(p->head) = snapshot.head->list;
#ifdef PBUF
			this_cpu_write(*(p->cached), CACHE_SIZE);
#else
			this_cpu_write(*(p->cached), CACHE_SIZE - 1);
#endif
			m = (struct object*) snapshot.head;

			pr_debug("from gpool ohead: %p/%ld, nhead: %p/%ld\n",
				snapshot.head, snapshot.version,
				new.head, new.version);
			WARN_ON_ONCE(!new.head);
			goto out;
		} // if

		} // for
#endif
	}
out:
	/* enable preemption */
	preempt_enable();
	return m;
}

EXPORT_SYMBOL(priv_alloc);

void priv_free(void *addr, pool_type_t type)
{
	struct object *p, *head;
	priv_pool_t *pool;
	if (!addr)
		return;

	pool = get_pool(type);

	if (!pool)
		return;

	p = (struct object*) addr;

	/* disable preempt until we manipulate all percpu pointers */
	preempt_disable();

	head = (struct object*)*this_cpu_ptr(pool->head);
	p->next = (struct object*)head;
	*this_cpu_ptr(pool->head) = p;

	pr_debug("chaining %p to head %p\n", p, head);

#ifdef PBUF
	if (this_cpu_inc_return(*pool->cached) == CACHE_SIZE) {
#else
	if (this_cpu_inc_return(*pool->cached) == CACHE_SIZE + 1) {
#endif
		*this_cpu_ptr(pool->marker) = *this_cpu_ptr(pool->head);
		pr_debug("set marker @ %p\n", *this_cpu_ptr(pool->marker));
		if ((void*)(*this_cpu_ptr(pool->marker))->next < (void*)pool->pool ||
				(void*)(*this_cpu_ptr(pool->marker))->next > (void*)(pool->pool + (pool->total_pages << PAGE_SHIFT))) {
			printk("marker->next is corrupted!! marker %p | marker->next %p\n",
						*this_cpu_ptr(pool->marker), (*this_cpu_ptr(pool->marker))->next);
			dump_stack();
		}
	} else if (*this_cpu_ptr(pool->cached) == (CACHE_SIZE << 1)) {
		struct bundle *donation = (struct bundle *)*this_cpu_ptr(pool->head);
		struct atom snapshot, new;

#ifdef SPINLOCK
		/* lock global pool */
		spin_lock(&pool->pool_spin_lock);
#endif

		new.head = donation;
		donation->list = ((struct object*)*this_cpu_ptr(pool->head))->next;

		if ((void*)(*this_cpu_ptr(pool->marker))->next < (void*)pool->pool ||
				(void*)(*this_cpu_ptr(pool->marker))->next > (void*)(pool->pool + (pool->total_pages << PAGE_SHIFT))) {
			printk("update pool->head with corrupted marker %p | marker->next %p\n",
						*this_cpu_ptr(pool->marker), (*this_cpu_ptr(pool->marker))->next);
			dump_stack();
		}
		//BUG_ON((*this_cpu_ptr(pool->marker))->next < pool->pool || (*this_cpu_ptr(pool->marker))->next > (pool->pool + pool->total_pages))
		*this_cpu_ptr(pool->head) = (*this_cpu_ptr(pool->marker))->next;
		(*this_cpu_ptr(pool->marker))->next = NULL;
#ifdef PBUF
		*this_cpu_ptr(pool->cached) = CACHE_SIZE - 1;
#else
		*this_cpu_ptr(pool->cached) = CACHE_SIZE;
#endif
#ifndef SPINLOCK
		do {
#endif
		snapshot = pool->stack;

		donation->next = snapshot.head;
		new.version = snapshot.version + 1;

#ifdef SPINLOCK
		pool->stack.head = new.head;
		pool->stack.version = new.version;

		/* unlock global pool */
		spin_unlock(&pool->pool_spin_lock);
#else
		} while (!cmpxchg_double(&pool->stack.head, &pool->stack.version, snapshot.head, snapshot.version,
					new.head, new.version));
#endif
		WARN_ON_ONCE(!new.head);
		pr_debug("update gpchain %p to %p | ohead: %p/%ld, nhead: %p/%ld\n",
				donation, snapshot.head,
				snapshot.head, snapshot.version,
				new.head, new.version);
	}

	/* enable preemption */
	preempt_enable();
}
EXPORT_SYMBOL(priv_free);

int priv_init(void)
{
	return 0;
}
module_init(priv_init);

void priv_exit(void)
{
	return;
}
module_exit(priv_exit);

MODULE_LICENSE("GPL");
