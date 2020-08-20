#ifndef IPC_HELPER_H
#define IPC_HELPER_H

#define FIPC_MSG_STATUS_AVAILABLE 0xdeaddeadUL
#define FIPC_MSG_STATUS_SENT      0xfeedfeedUL
#define fipc_test_pause()    asm volatile ( "pause\n": : :"memory" );

static inline unsigned long inc_rx_slot(struct fipc_ring_channel *rc)
{
	return (rc->rx.slot++);
}

static inline unsigned long get_rx_idx(struct fipc_ring_channel *rc)
{
	return rc->rx.slot & rc->rx.order_two_mask;
}
static inline struct fipc_message* 
get_current_rx_slot(struct fipc_ring_channel *rc)
{
	return &rc->rx.buffer[get_rx_idx(rc)];
}

static inline int check_rx_slot_msg_waiting(struct fipc_message *slot)
{
	return slot->msg_status == FIPC_MSG_STATUS_SENT;
}

static inline int
thc_ipc_recv_response_inline(struct thc_channel* channel, uint32_t id,
		struct fipc_message** out)
{
	int ret;
	int received_cookie;
	PTState_t *pts = PTS();
retry:
	while ( 1 )
	{
		// Poll until we get a message or error
		*out = get_current_rx_slot( thc_channel_to_fipc(channel));

		if ( ! check_rx_slot_msg_waiting( *out ) )
		{
			// No messages to receive, yield to next async
#ifndef BASE_CASE_NOASYNC
#if  1
			if (pts->reached_dofin) {
				fipc_test_pause();
				continue;
			}
#endif
			THCYieldAndSave(id);
#else
			fipc_test_pause();
#endif
			continue;
		}

		break;
	}

#ifndef BASE_CASE_NOASYNC
	received_cookie = thc_get_msg_id(*out);
	if (received_cookie == id) {
#endif
		inc_rx_slot( thc_channel_to_fipc(channel) );
		return 0;
#ifndef BASE_CASE_NOASYNC
	}
#endif
	
#ifndef BASE_CASE_NOASYNC
	ret = THCYieldToIdAndSave(received_cookie, id);
	 
	if (ret) {
		printk("ALERT: wrong id\n");
		return ret;
	}
#endif
	// We came back here but maybe we're the last AWE and 
        // we're re-started by do finish
	fipc_test_pause();
	goto retry; 
	return 0;
}

#endif /* IPC_HELPER_H */
