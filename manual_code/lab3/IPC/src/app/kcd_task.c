/* The KCD Task Template File */
// hash table implementation from https://www.tutorialspoint.com/data_structures_algorithms/hash_table_program_in_c.htm
#include "rtx.h"
#include "Serial.h"
#include "printf.h"
#include "k_inc.h"

#define SIZE 62 // all possible command characters
extern TCB *gp_current_task;

char str[64];
int counter = 0;

char hash_table[MAX_TASKS];

void kcd_task(void)
{
	// create mailbox for kcd task
    mbx_create(KCD_MBX_SIZE);

    for (int i = 0; i < MAX_TASKS; i++) {
    	hash_table[i] = NULL;
    }

    void *ptr = mem_alloc(sizeof(RTX_MSG_HDR) + sizeof(char));

    while(1) {
    	task_t sender_tid = NULL;

    	if (recv_msg(&sender_tid, ptr, sizeof(RTX_MSG_HDR)+sizeof(char)) == -1){
    		continue;
    	}

		RTX_MSG_HDR *hdr = (RTX_MSG_HDR*) ptr;
		size_t msg_hdr_size = sizeof(RTX_MSG_HDR);
		char *c = (char*)ptr + msg_hdr_size;

		// registering command
		if (hdr->type == KCD_REG) {
			hash_table[sender_tid] = *c;

		// keyboard input
		} else if (hdr->type == KEY_IN) {
			if (*c == '\n' || *c == '\r') {
				// ready to send the string
				if (str[0] != '%' || counter > 64) {
					printf("Invalid Command\n");
				} else {

					char *string = (char*) mem_alloc((counter-1)*sizeof(char));
					for (int i = 0; i < counter-1; i++) {
						string[i] = str[i+1];
					}

					task_t tid = MAX_TASKS;
					for (int i = 0; i < MAX_TASKS; i++) {
						if (hash_table[i] == string[1]) {
							tid = i;
							break;
						}
					}
					if (tid == MAX_TASKS) {
						printf("Command cannot be processed\n");
					}
					send_msg(tid, (void *)string);

				}
				counter = 0;

			} else {
				// recieving a character
				str[counter] = *c;
				++counter;
			}
		}
    }
}

//void main_task(){
//    RTX_TASK_INFO task_info;
//    task_t tid;
//    task_t no_mail_tid;
//    task_t receiver_tid;
//    task_t sender_tid;
//    k_tsk_create(&tid, &test1, LOW, 0x200);
//    k_tsk_get(tid, &task_info);
//    printf("%d\n", task_info->mb_capacity);
//    //So on... So on... So on...
//
//    // A simple task that creates a mailbox
//    k_tsk_create(&no_mail_tid, &no_mail_task, LOW, 0x200);
//    k_tsk_create(&receiver_tid, &recievertask, LOW, 0x200);
//    k_tsk_create(&sender_tid, &sendertask, LOW, 0x200);
//
//
//}
//
//Test1:
//1. Check if mailbox gets created
//2. Checks if mailbox already exists
//3. Check if size lower than minimum size gets returns -1
//4. Check if it returns -1 when there is not enough memory
//5. Check if the parameters gets assigned to gp_current_task
//
//void test1(){
//
//    printf("Creating first mailbox");
//    int returnVal = mbx_create(0x300);
//    if (returnVal == -1){
//        printf("Mailbox returned an error);
//    };
//
//    printf("Creating second mailbox");
//    int returnVal = mbx_create(0);
//    if (returnVal == -1){
//        printf("Mailbox returned an error);
//    };
//
//    printf("Creating third mailbox");
//    int returnVal = mbx_create(1000);
//    if (returnVal == 0){
//        printf("Mailbox was successfully created);
//    };
//
//    printf("Creating fourth mailbox");
//    returnVal = mbx_create(1000);
//    if (returnVal == -1){
//        printf("Mailbox returned an error);
//    };
//}
//
//void no_mail_task(){
//    printf("I was created\n");
//}
//
//void receivertask(){
//    mbx_create(1000);
//    unsigned char buffer[100];
//    recv_msg(senderTid, buffer, 100);
//    printf("%c", buffer[0]);
//}
//
//void sendertask(){
//    mbx_create(1000);
//    unsigned char buffer[100];
//    buffer[0] = 'h';
//
//    int sendVal = send_msg(receiverTid + 10, buffer);
//    printf("Test with invalid receiverTid: %d\n", sendVal);
//
//    sendVal = send_msg(no_mail_tid, buffer);
//    printf("Test with no mailbox tid: %d\n", sendVal);
//
//    sendVal = send_msg(receiverTid, null);
//    printf("Test with invalid buffer: %d\n", sendVal);
//
//    sendVal = send_msg(receiverTid, buffer);
//    printf("Test with valid tid: %d\n", sendVal);
//}




