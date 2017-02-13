#ifndef __BLK_LCD_H_
#define __BLK_LCD_H_

typedef struct cptr {
        unsigned long cptr;
} cptr_t;

struct request_queue_container {
        struct request_queue rq;
        cptr_t other_ref;
        cptr_t my_ref;
};

struct blk_mq_hw_ctx_container {
        struct blk_mq_hw_ctx blk_mq_hw_ctx;
        cptr_t other_ref;
        cptr_t my_ref;
};
#endif
