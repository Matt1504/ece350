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

struct DataItem  {
   char 	key;   
   task_t 	tid;
};

struct DataItem* hashArray[SIZE]; 
struct DataItem* item;

struct DataItem *search(char key) {
   //get the hash
   int hashIndex = 0;

   //move in array until an empty
   while(hashArray[hashIndex] != NULL) {
      if(hashArray[hashIndex]->key == key)
         return hashArray[hashIndex];
      //go to next cell
      ++hashIndex;

      //wrap around the table
      hashIndex %= SIZE;
   }

   return NULL;
}

void insert(char key, task_t tid) {

	struct DataItem *item = (struct DataItem*) mem_alloc(sizeof(struct DataItem));
	item->tid = tid;
	item->key = key;

	//get the hash
	int hashIndex = 0;

	//move in array until an empty or deleted cell OR key is the same
	while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1 && hashArray[hashIndex]->key != key) {
		//go to next cell
		++hashIndex;

		//wrap around the table
		hashIndex %= SIZE;
	}

	hashArray[hashIndex] = item;
}


void kcd_task(void)
{
	// create mailbox for kcd task
    mbx_create(KCD_MBX_SIZE);

    while(1) {
    	task_t *sender_tid = NULL; 
    	void *ptr = mem_alloc(sizeof(RTX_MSG_HDR) + sizeof(char));
    	int res = recv_msg(sender_tid, ptr, sizeof(RTX_MSG_HDR)+sizeof(char));

    	if (res == 0) {
    		RTX_MSG_HDR *hdr = (RTX_MSG_HDR*) ptr;
	    	size_t msg_hdr_size = sizeof(RTX_MSG_HDR);
	    	// registering command 
	    	if (hdr->type == KCD_REG) {
	    		char *c = (char*)ptr + msg_hdr_size;
	    		insert(*c, gp_current_task->tid);

	    	// keyboard input
	    	} else if (hdr->type == KEY_IN) {
	    		char *c = (char*)ptr + msg_hdr_size;
				if (*c == '\n' || *c == '\r') {
					// ready to send the string
					if (str[0] != '%' || counter > 64) {
						SER_PutChar(1, 'I');
					} else {

						char *string = (char*) mem_alloc((counter-1)*sizeof(char));
						for (int i = 0; i < counter-1; i++) {
							string[i] = str[i+1];
						}

						task_t tid = search(string[0])->tid;
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




