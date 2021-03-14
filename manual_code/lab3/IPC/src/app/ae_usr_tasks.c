/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO ECE 350 RTOS LAB
 *
 *                     Copyright 2020-2021 Yiqing Huang
 *                          All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice and the following disclaimer.
 *
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************
 */

/**************************************************************************//**
 * @file        a usr_tasks.c
 * @brief       Two user/unprivileged  tasks: task1 and task2
 *
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 *****************************************************************************/

#include "ae_usr_tasks.h"
#include "rtx.h"
#include "Serial.h"
#include "printf.h"
#include "k_task.h"

extern TCB *gp_current_task;

/**
 * @brief: a dummy task1
 */
void task1(void)
{
    task_t tid;
    RTX_TASK_INFO task_info;
    
    SER_PutStr (0,"task1: entering \n\r");
    /* do something */
    tsk_create(&tid, &task2, LOW, 0x200);  /*create a user task */
    tsk_get(tid, &task_info);
    tsk_set_prio(tid, LOWEST);
    /* terminating */
    tsk_exit();
}

/**
 * @brief: a dummy task2
 */
void task2(void)
{
    SER_PutStr (0,"task2: entering \n\r");
    /* do something */
    long int x = 0;
    int ret_val = 10;
    int i = 0;
    int j = 0;
    for (i = 1;;i++) {
            char out_char = 'a' + i%10;
            for (j = 0; j < 5; j++ ) {
                SER_PutChar(0,out_char);
            }
            SER_PutStr(0,"\n\r");

            for ( x = 0; x < 5000000; x++); // some artifical delay
            if ( i%6 == 0 ) {
                SER_PutStr(0,"usr_task2 before yielding cpu.\n\r");
                ret_val = tsk_yield();
                SER_PutStr(0,"usr_task2 after yielding cpu.\n\r");
                printf("usr_task2: ret_val=%d\n\r", ret_val);
            }
        }
    /* terminating */
    //tsk_exit();
}

void receiver_task(){
    printf("I AM THE RECEIVER\n");
    task_t sender_tid = 2;
    mbx_create(1000);
    unsigned char buffer[100];
    recv_msg(&sender_tid, buffer, 100);
    printf("%c", buffer[0]);
}

void sender_task(){
    task_t receiver_tid = 1;
    printf("I AM THE SENDER\n");
    mbx_create(1000);
    unsigned char buffer[100];
    buffer[0] = 'h';

    int sendVal = send_msg(receiver_tid + 10, buffer);
    printf("Test with invalid receiverTid: %d\n", sendVal);
}

void mailbox_task2() {
    task_t receiver_tid;
    task_t sender_tid;
    printf("I HAVE ENTERED\n");
    int return_val = k_tsk_create(&receiver_tid, &receiver_task, HIGH, 0x200);
    printf("return: %d\n", return_val);
    k_tsk_create(&sender_tid, &sender_task, MEDIUM, 0x200);
    k_tsk_exit();
}

void mailbox_task1() {
    // the tid of this task should be 1 since null task is 0
    task_t sender_id;
    task_t receiver_id = 1;
    void *buffer = mem_alloc(60);
    printf("TEST CASE 1: Mailbox Creation\n");
    // first try to receive a message before mailbox is created
//    if (recv_msg(&sender_id, &buffer, 5) == 0) {
//        printf("TEST CASE FAILED: recv_message passed before mailbox was created\n");
//        return;
//    }

    if (mbx_create(0) == 0){
        printf("TEST CASE FAILED: mailbox created of size 0\n");
        return;
    }
    // size of the mailbox will be 100 bytes
    if (mbx_create(100) == -1){
        printf("TEST CASE FAILED: mailbox was not created\n");
        return;
    }
    if (mbx_create(100) == 0){
        printf("TEST CASE FAILED: second mailbox was created on single task\n");
        return;
    }

    printf("TEST CASE 1 PASSED\n");
    // also test blocking for no messages
    tsk_yield();
//    int retVal = recv_msg(&sender_id, buffer, 5);
    // this should pass, but such that the state becomes blocked and k_tsk_run_new() will run so send_task2 will now run

    // when send_task2 yields after sending the message, this should run again
    printf("TEST CASE 3: Message Receiving\n");

    // receive the message sent by task send_task2
    if (recv_msg(&sender_id, buffer, 60) == -1) {
        printf("TEST CASE FAILED: failed to receive first message in mailbox\n");
        return;
    } else {
        printf("sender id: %d\n", sender_id);
    }

    // receive the second message
    if (recv_msg(&sender_id, buffer, 10) == -1) {
        printf("TEST CASE FAILED: failed to receive second message in mailbox\n");
        return;
    } else {
        printf("sender id: %d\n", sender_id);
    }

    tsk_yield();

    // Since mailbox_task1 has a higher priority than send_task2, tsk_yield will not do anything
    if (recv_msg(&sender_id, buffer, 60) == -1) {
        printf("TEST CASE FAILED: failed to receive overwritten message in mailbox\n");
    } else {
        printf("sender id: %d\n", sender_id);
    }

    printf("TEST CASE 3 PASSED\n");
    mem_dealloc(buffer);
    tsk_exit();
}

void send_task2() {
    printf("TEST CASE 2: Message Sending\n");
    // the tid of this task should be 2
    // first have to create the message
    // alloc space for the message (our message data will just be a char to make life easy)
    U8 *buf = (U8*) mem_alloc(sizeof(struct rtx_msg_hdr) + 1);
    struct rtx_msg_hdr *ptr = (void*)buf;
    // size of message is 60 bytes
    ptr-> length = 60;
    ptr-> type = DEFAULT;
    buf += sizeof(struct rtx_msg_hdr);
    *buf = 't';

    // send to a task that doesn't exist
    if(send_msg(20, (void*)ptr) == 0) {
        printf("TEST CASE FAILED: message sent to task that doesn't exist\n");
        return;
    }
    // send to a task with no mailbox
    if (send_msg(0, (void*)ptr) == 0) {
        printf("TEST CASE FAILED: message sent to task with no mailbox\n");
        return;
    }
    // send the first message to the task 1
    if (send_msg(1, (void*)ptr) == -1) {
        printf("TEST CASE FAILED: fist message could not be sent to mailbox\n");
    }

    // send the second message to the task 2
    ptr-> length = 10;
    if (send_msg(1, (void*)ptr) == -1) {
        printf("TEST CASE FAILED: second message could not be sent to mailbox\n");
    }
//  // send the message again (should not be possible since there's not enough space)
//  if (send_msg(1, (void*)ptr) == 0) {
//      printf("TEST CASE FAILED: message was sent when mailbox does not have enough space\n");
//      return;
//  }
    printf("TEST CASE 2 PASSED\n");

    tsk_yield();

    // when it gets to run again, the task 1 should have received the message, send the message again
    if (send_msg(1, (void*)ptr) == -1) {
        printf("TEST CASE FAILED: message could not be sent to mailbox after previous message was received\n");
        return;
    }

    mem_dealloc(buf);
    tsk_exit();
}

void recv_task3(){
    task_t tid = 3;
    task_t sender_id;
    void *buffer = mem_alloc(60);
    void *small_buffer = mem_alloc(30);
    void *fake_buffer;
    // Call receive before mailbox is created
    if (recv_msg(&sender_id, buffer, 60) == 0){
        printf("TEST CASE FAILED: receive is possible without a mailbox\n");
    }

    // size of the mailbox will be 100 bytes
    if (mbx_create(100) == -1){
        printf("TEST CASE FAILED: mailbox was not created\n");
        return;
    }

    if (recv_msg(&sender_id, fake_buffer, 60) == 0){
        printf("TEST CASE FAILED: receive is running with a null buffer\n");
    }
    k_tsk_set_prio(tid, 107);

    int retVal = recv_msg(&sender_id, buffer, 60);
    printf("Comment 2: This should appear after comment 1\n");
//  printf("The message was %c\n", (*char) *buffer);
    printf("sender id: %d\n", sender_id);

    mem_dealloc(buffer);
    tsk_exit();
}

void send_task4(){
    U8 *buf = (U8*) mem_alloc(sizeof(struct rtx_msg_hdr) + 1);
    struct rtx_msg_hdr *ptr = (void*)buf;
    // size of message is 60 bytes
    ptr-> length = 60;
    ptr-> type = DEFAULT;
    buf += sizeof(struct rtx_msg_hdr);
    *buf = 't';

    // send the message to the task 3
    if (send_msg(3, (void*)ptr) == -1) {
        printf("TEST CASE FAILED: message could not be sent to mailbox\n");
    }

    printf("Comment 1: This should appear before comment 2\n");
    mem_dealloc(buf);
    tsk_exit();
}

void recv_task5(){
  printf("Entering Task 5\n");
  task_t sender_id;
  unsigned char *buffer = mem_alloc(200);

  // size of the mailbox will be 100 bytes
  if (mbx_create(200) == -1){
      printf("TEST CASE FAILED: mailbox was not created\n");
      return;
  }
  tsk_set_prio(5, 111);
  if (recv_msg(&sender_id, buffer, 200) == -1){
      printf("TEST CASE FAILED: receive is running with a null buffer\n");
  } else {
      for (int i = 0; i < 200; i++){
          printf("%c\n", buffer[i]);
      }
  }
  mem_dealloc(buffer);
  tsk_exit();
}

void send_task6(){
  printf("Entering Task 6\n");
  U8 *buf = (U8*) mem_alloc(sizeof(struct rtx_msg_hdr) + 1);
  struct rtx_msg_hdr *ptr = (void*)buf;
  // size of message is 60 bytes
  ptr-> length = 60;
  ptr-> type = DEFAULT;
  buf += sizeof(struct rtx_msg_hdr);
  *buf = 't';
  if (send_msg(5, (void*)ptr) == -1) {
      printf("TEST CASE FAILED: message could not be sent to mailbox\n");
  }
  *buf = 'a';
  if (send_msg(5, (void*)ptr) == -1) {
      printf("TEST CASE FAILED: message could not be sent to mailbox\n");
  }
  *buf = 'd';
  if (send_msg(5, (void*)ptr) == -1) {
      printf("TEST CASE FAILED: message could not be sent to mailbox\n");
  }
  mem_dealloc(buf);
  tsk_exit();
}

void kcd_task_1() {
    SER_PutStr (0,"task2: entering \n\r");
	// create mailbox
    task_t sender_id;
    mbx_create(128);

    U8 *buffer = (U8*) mem_alloc(sizeof(struct rtx_msg_hdr) + 1);

    size_t msg_hdr_size = sizeof(struct rtx_msg_hdr);
    U8 *buf = buffer; /* buffer is allocated by the caller somewhere else
    */
    struct rtx_msg_hdr *ptr = (void *)buf;
    ptr->length = msg_hdr_size + 1;
    ptr->type = KCD_REG;
    buf += msg_hdr_size;
    *buf = 'a';
    send_msg(TID_KCD, (void *)ptr);

    unsigned char *buffer2 = mem_alloc(64 + sizeof(struct rtx_msg_hdr));

    while(1) {
		while(gp_current_task->num_msgs == 0);
		int ret = recv_msg(&sender_id, buffer2, 64 + sizeof(struct rtx_msg_hdr));
		printf("value: %d\n", ret);
    }
}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
