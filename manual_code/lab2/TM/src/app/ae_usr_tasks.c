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
#include <stdbool.h>



/**
 * @brief: a dummy task1
 */
void task1(void)
{
    task_t tid;
    RTX_TASK_INFO task_info;
    
    SER_PutStr ("task1: entering \n\r");
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
    SER_PutStr ("task2: entering \n\r");
    /* do something */
    /* terminating */
    tsk_exit();
}

// user tasks for test case 1
void task_create(void)
{
	SER_PutStr("Entering Task_Create Task\r\n");
	task_t tid;
	RTX_TASK_INFO task_info[10];
	// create a simple task a bunch of tasks that basically just terminate
	for (int i = 0; i < 5; i++) {
		// create 10 tasks that will get just terminate when they run
		SER_PutStr("creating sub user-task\r\n");
		tsk_create(&tid, &task_terminate, MEDIUM, 0x200);
		printf("tid: %d\r\n", tid);
		tsk_get(tid, &task_info[i]);
	}
	// after creating all the tests, you can exit this task to run the terminate tasks as well
	SER_PutStr("Before tsk_exit in task_create\r\n");
	tsk_exit();

}

void task_terminate(void)
{
	SER_PutStr("task created...\r\n");
	// allocate your data
	void *p;
	SER_PutStr("allocating 8 bytes...\r\n");
	p = mem_alloc(8);

	// deallocate your data
	SER_PutStr("deallocating 8 bytes...\r\n");
	if (mem_dealloc(p) != 0) {
		SER_PutStr("deallocation failed!");
	}
	// exit
	SER_PutStr("task exiting...\r\n");
	tsk_exit();
}

// user tasks for test case 2
void task_low_prio(void)
{
	SER_PutStr("low prio task running...\r\n");
	tsk_exit();
}
void task_medium_prio(void)
{
	SER_PutStr("medium prio task running...\r\n");
	tsk_exit();
}
void task_high_prio(void)
{
	SER_PutStr("high prio task running...\r\n");
	tsk_exit();
}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
