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
 * @file        priv_tasks.c
 * @brief       Two privileged tasks: priv_task1 and priv_task2
 *
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 *
 * @note        Each task is in an infinite loop. These Tasks never terminate.
 *
 *****************************************************************************/

#include <ae_mailbox.h>
#include "Serial.h"
#include "printf.h"

/**************************************************************************//**
 * @brief       a task that prints AAAAA, BBBBB, CCCCC,...., ZZZZZ on each line.
 *              It yields the cpu every 6 lines are printed.
 *****************************************************************************/



void receiver_task(){

	printf("I AM THE RECEIVER\n");
	task_t sender_tid = 1;
    mbx_create(1000);
    unsigned char buffer[100];
    recv_msg(&sender_tid, buffer, 100);
    printf("%c", buffer[0]);
}

void sender_task(){
	task_t receiver_tid = 0;
	printf("I AM THE SENDER\n");
    mbx_create(1000);
    unsigned char buffer[100];
    buffer[0] = 'h';

    int sendVal = send_msg(receiver_tid + 10, buffer);
    printf("Test with invalid receiverTid: %d\n", sendVal);

//    sendVal = send_msg(no_mail_tid, buffer);
//    printf(Test with no mailbox tid: %d\n", sendVal);
//
//    sendVal = send_msg(receiver_tid, null);
//    printf(Test with invalid buffer: %d\n", sendVal);
//
//    sendVal = send_msg(receiverTid, buffer);
//    printf("Test with valid tid: %d\n", sendVal);
}

void mailbox_task1() {

	printf("Creating first mailbox\n");
	int returnVal = mbx_create(0x300000000);
	if (returnVal == -1){
		printf("Mailbox returned an error\n");
	};

	printf("Creating second mailbox\n");
	returnVal = mbx_create(0);
	if (returnVal == -1){
		printf("Mailbox returned an error\n");
	};

	printf("Creating third mailbox\n");
	returnVal = mbx_create(1000);
	if (returnVal == 0){
		printf("Mailbox was successfully created\n");
	};

	printf("Creating fourth mailbox\n");
	returnVal = mbx_create(1000);
	if (returnVal == -1){
		printf("Mailbox returned an error\n");
	};
	return;
}

//void mailbox_task2() {
//	printf("I HAVE ENTERED\n");
//	int return_val = k_tsk_create(&receiver_tid, &receiver_task, HIGH, 0x200);
//	printf("return: %d\n", return_val);
//	k_tsk_create(&sender_tid, &sender_task, MEDIUM, 0x200);
//	k_tsk_exit();
//}




//void mailbox_task2(void) {
//	task_t tid;
//	task_t no_mail_tid;
//	task_t receiver_tid;
//	task_t sender_tid;
//	//k_tsk_create(&tid, &test1, HIGH, 0x200);
//	printf("I RAM RUNNNIGNGIGNIGN\n");
//	return;
//}

/**************************************************************************//**
 * @brief:      a task that prints 00000, 11111, 22222,....,99999 on each line.
 *              It yields the cpu every 6 lines are printed
 *              before printing these lines indefinitely, it creates a
 *              new task and and obtains the task information. It then
 *              changes the newly created task's priority.
 *****************************************************************************/
//
//void priv_task2(void)
//{
//    long int x = 0;
//    int ret_val = 10;
//    int i = 0;
//    int j = 0;
//    RTX_TASK_INFO task_info;
//    task_t tid;
//
//    k_tsk_create(&tid, &task1, LOW, 0x200);  /*create a user task */
//    k_tsk_get(tid, &task_info);
//    k_tsk_set_prio(tid, LOWEST);
//
//
//    for (i = 1;;i++) {
//        char out_char = '0' + i%10;
//        for (j = 0; j < 5; j++ ) {
//            SER_PutChar(0,out_char);
//        }
//        SER_PutStr(0,"\n\r");
//
//        for ( x = 0; x < DELAY; x++); // some artifical delay
//        if ( i%6 == 0 ) {
//            SER_PutStr(0,"priv_task2 before yielding cpu.\n\r");
//            ret_vaal = k_tsk_yield();
//            SER_PutStr(0,"priv_task2 after yielding cpu.\n\r");
//            printf("priv_task2: ret_val=%d\n\r", ret_val);
//#ifdef DEBUG_0
//            //printf("priv_task2: ret_val=%d\n\r", ret_val);
//#endif /* DEBUG_0 */
//        }
//    }
//}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
