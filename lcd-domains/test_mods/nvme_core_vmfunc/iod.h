// Need a way to expose this datastructure so I can build wrappers around it.
// The original nvme/pci.c is modded to have include guards around it.
#ifndef NVME_IOD
#define NVME_IOD

struct nvme_iod {
	struct nvme_queue *nvmeq;
	int aborted;
	int npages;		/* In the PRP list. 0 means small pool in use */
	int nents;		/* Used in scatterlist */
	int length;		/* Of data, in bytes */
	dma_addr_t first_dma;
	struct scatterlist meta_sg; /* metadata requires single contiguous buffer */
	struct scatterlist *sg;
	struct scatterlist inline_sg[0];
};

#endif