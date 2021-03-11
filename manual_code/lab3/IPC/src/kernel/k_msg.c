/**
 * @file:   k_msg.c
 * @brief:  kernel message passing routines
 * @author: Yiqing Huang
 * @date:   2020/10/09
 */

#include "k_msg.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* ! DEBUG_0 */

extern TCB *gp_current_task;
extern TCB g_tcbs[MAX_TASKS];

int k_mbx_create(size_t size) {
#ifdef DEBUG_0
    printf("k_mbx_create: size = %d\r\n", size);
#endif /* DEBUG_0 */
    // create a mailboxc for the calling task 
    // size argument - capactiy of the mailbox in bytes
    // capacity used for emssages and meta data kernel might need for each message to manage mailbox 
    // mailbox serves messages using FCFS
    // each task will have at most 1 mailbox
    // owner of memory allocated to each task's mailbox is kernel 
    // memory for mailbox is deallocated when task exits

    // possible cases of failure 
    // calling task already has a mailbox
    if (gp_current_task -> mb_capacity > 0) {
    	return -1;
    }
    // size argument < MIN_MBX_SIZE
    if (size < MIN_MBX_SIZE) {
    	return -1; 
    }
    // available memory at run time is not enough to create requested mailbox 
    gp_current_task-> mb_buffer = k_mem_alloc(size); 
    if (gp_current_task-> mb_buffer == NULL) {
    	// not enough memory at run time to create mailbox 
    	return -1; 
    }
    // should zero-initailize the memory chunk so it is easier to analyze the data 
    unsigned char* p = (unsigned char*) gp_current_task-> mb_buffer;
    int len = size; 
    while(len--) {
    	*p++ = 0; 
    }
    // otherwise the allocation works so can fill in the other field
    gp_current_task-> mb_capacity = size; 
    gp_current_task-> num_msgs = 0; 
    gp_current_task-> mb_buffer_end = gp_current_task->mb_buffer + size;
    gp_current_task-> mb_head = (MSG*) gp_current_task-> mb_buffer;
    gp_current_task-> mb_tail = (MSG*) gp_current_task-> mb_buffer;
    gp_current_task-> mb_head-> next = NULL; 
    return 0;
    // implement mailbox as a ring buffer
}

int k_send_msg(task_t receiver_tid, const void *buf) {
#ifdef DEBUG_0
    printf("k_send_msg: receiver_tid = %d, buf=0x%x\r\n", receiver_tid, buf);
#endif /* DEBUG_0 */
    // send a message to the mailbox of task
    // data specified by buf to amilbox identifed by receiver_tid
    // if task state BLK_MSG, it becomes unblocked 
    // if prio of unblocked task > currently running task, 
    // then unblocked task preempts currently running task, 
    // currently running task is added to queue
    // if prio is not higher, added to back of ready queue
    // message buf starts with message header followed by actual message data
    TCB *p_tcb = &g_tcbs[receiver_tid];
    // causes of failure 
    // receiver_tid does not exist or is dormant
    if (p_tcb == NULL || p_tcb-> state == DORMANT) {
    	return -1; 
    }
    // receiver_tid does not have a mailbox 
    if (p_tcb-> mb_buffer == NULL) {
    	return -1; 
    }
    // buf is a null pointer 
    if (buf == NULL) {
    	return -1; 
    }
    // length field in buf < MIN_MSG_SIZE
    RTX_MSG_HDR *header_msg = (RTX_MSG_HDR*) buf;
    if (header_msg-> length < MIN_MSG_SIZE) { 
    	return -1;
    }
    // receiver_tid mailbox does not have enough free space for message 
    // sending a message will move the tail pointer
    // receiving a message will move the head pointer 
    // mailbox on the heap 
    // msg_send is where head will point to after this write
    // create a pointer that points to the the task that we want to send the message to

    // create a struct pointer that points to the head of the queue
    MSG *msg_send;
    // msg_send will be different for the first time a message is sent since the next prop will be invalid
    if (p_tcb-> mb_head-> next == NULL) {
    	msg_send = p_tcb-> mb_head;
    } else {
    	msg_send = p_tcb-> mb_head->next;
    }
    // check to see if free space from head to end of queue 
    if ((unsigned int) msg_send + header_msg-> length + sizeof(MSG) > (unsigned int) p_tcb-> mb_buffer_end) {
    	// can't write without overflow, can only overwrite read messages at the beginning  (circle back)
    	// set msg_send to point at the start of the queue again (circle back)
    	msg_send = (MSG*)p_tcb-> mb_buffer;
    	// check enough space between front of queue and tail 
    	if ((unsigned int) msg_send + header_msg-> length + sizeof(MSG) > (unsigned int) p_tcb-> mb_tail) {
    		// not enough space
    		return -1; 
    	} else {
    		// there is enough space so I can overwrite messages that have been received 
    		// set head-> next to point to msg_send which has now circled back to the beginning of the queue 
    		p_tcb-> mb_head-> next = msg_send;
    	}
    }
    // can set the meta data for the message
    msg_send-> sender_id = gp_current_task->tid; 
    // the next will point to the end of this message address
    unsigned int endAddr = (unsigned int) msg_send + header_msg-> length + sizeof(MSG); 
    msg_send-> next = (MSG*) endAddr;
    // can just memcpy implementation copy from buf to mailbox tail 
    unsigned char *csrc = (unsigned char*) buf;
    unsigned char *cdest = (unsigned char*) msg_send-> body;
    for (int i = 0; i < header_msg-> length; i++) {
    	cdest[i] = csrc[i]; 
    }
    ++p_tcb-> num_msgs;
    p_tcb-> mb_head = msg_send; 
    // preempt the task based on priority ordering 
    // check if receiver is blocked, changed to unblocked if it is 
    if (p_tcb-> state == BLK_MSG) {
    	p_tcb-> state == READY;
    	// when it was blocked, it was not placed back in the ready queue, so now have to add it back
    	queue_add(p_tcb);
    	// run to check for preemption
    	k_tsk_run_new(); 
    }
    return 0;
}

int k_recv_msg(task_t *sender_tid, void *buf, size_t len) {
#ifdef DEBUG_0
    printf("k_recv_msg: sender_tid  = 0x%x, buf=0x%x, len=%d\r\n", sender_tid, buf, len);
#endif /* DEBUG_0 */
    // receives a message from its mailbox if there are any 
    // gets blocked if there are none 
    // buf will get filled with the received message 

    // possible causes of failure 
    // calling task does not have a mailbox 
    if (gp_current_task-> mb_buffer == NULL) {
    	return -1; 
    }
    // buf argument is a null pointer 
    if (buf == NULL) {
    	return -1; 
    }
    if (gp_current_task-> num_msgs == 0) {
    	// set the state to be blocked
    	gp_current_task-> state = BLK_MSG; 
    	k_tsk_run_new(); 
    	return 0; 
    }
    // can fail if the buffer is too small to hold the message 
    RTX_MSG_HDR *msg_head = (RTX_MSG_HDR*) gp_current_task-> mb_tail-> body;
    if (msg_head-> length > len) {
    	return -1; 
    }
    if (sender_tid != NULL) {
    	// fill with sender task ID
    	*sender_tid = gp_current_task-> mb_tail->sender_id;
    }
    // fill the buf with the received message (memcpy same as in send)
    unsigned char *csrc = (unsigned char*) gp_current_task-> mb_tail-> body;
    unsigned char *cdest = (unsigned char*) buf; 

    for (int i = 0; i < msg_head->length; i++) {
    	cdest[i] = csrc[i]; 
    }
    // message received, the number of messages is now reduced
    --gp_current_task-> num_msgs; 

    // move the tail pointer to the next message
    gp_current_task-> mb_tail = gp_current_task-> mb_tail-> next; 
    return 0;
}

int k_mbx_ls(task_t *buf, int count) {
#ifdef DEBUG_0
    printf("k_mbx_ls: buf=0x%x, count=%d\r\n", buf, count);
#endif /* DEBUG_0 */
    return 0;
}
