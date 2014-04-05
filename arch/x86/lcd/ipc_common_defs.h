#ifndef LCD_GUEST_DEFS_H
#define LCD_GUEST_DEFS_H


typedef struct  {
	u32 mr[6];	/* MRs that are mapped to real registers */
	u32 saved_tag;		/* Saved tag field for stacked ipcs */
	u32 saved_sender;	/* Saved sender field for stacked ipcs */
	u32  notify[8]; /* Irq notification slots */
	u32 mr_rest[2];	/* Complete the utcb for up to 64 words */
} utcb_t;

typedef struct  {
	uint mr0;
	uint mr1;
	uint mr2;
	uint mr3;
	uint mr4;
	uint mr5;
} msg_regs_t;

//#define IPC_SEND_SND 1
//#define IPC_SEND_RCV  2
//#define IPC_SEND_SND_RCV  3

#define IPC_PEER_ANY  0xffffffff

#define LCD_IPC_DIR(x)  (x >> 32)
#define LCD_IPC_PEER(x)  (x & 0xffffffff)

enum IPC_DIR {
	IPC_INVALID = 0,
	IPC_SEND = 1,
	IPC_RECV = 2,
	IPC_SENDRECV = 3,
};

/* Per thread kernel stack unified on a single page. */
union utcb_union {
	utcb_t utcb;
	char kstack[PAGE_SIZE];
};

void display_mr(utcb_t *p_utcb) {
    printk(KERN_ERR "Message Regs at utcb %p - %d ,%d , %d\n", p_utcb, p_utcb->mr[0], p_utcb->mr[1], p_utcb->mr[3]);
}

#endif
