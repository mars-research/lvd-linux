#ifndef HOST_IPC_H
#define HOST_IPC_H

#include "ipc_common_defs.h"

enum ipc_state {
  IPC_DONT_CARE = 0,
  IPC_RCV_WAIT = 1,
  IPC_SND_WAIT = 2,
  IPC_RUNNING = 3,
};

typedef struct {
  u32 peer;
  struct list_head list;
  struct task_struct *task;
} ipc_wait_list_elem;

typedef struct  {
    // either we put an explicit capid here
    // so that given the capid we can  fetch
    // the peers sync_ipc or lcd_struct
    u32 state;
    u32 my_capid;
    //u32 dir;
    u32 expected_sender;
   // void *waiting_on; -> this might not be reqd as we are modelling spl states
    //struct lcd_struct *lcd_mine;
    //struct lcd_struct *lcd_partner;
    // some waitq
    spinlock_t snd_lock;
    u32 snd_sleepers;
    struct list_head snd_q;
    struct task_struct *task;
    //spinlock_t rcv_lock;
   // struct list_head rcv_q;
 } sync_ipc_t;

//headers used by host for ipc
int ipc_send(u32 myself, u32 recv_capid);
int ipc_recv(u32 myself, u32 send_capid);
void display_mr(utcb_t *p_utcb);

#endif
