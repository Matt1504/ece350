/* The KCD Task Template File */
// hash table implementation from https://www.tutorialspoint.com/data_structures_algorithms/hash_table_program_in_c.htm
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

void kcd_task(void)
{
	// create mailbox for kcd task
    mbx_create(KCD_MBX_SIZE);

    while(1) {
    	void *ptr;
    	int res = recv_msg(TID_KCD, ptr, sizeof(struct rtx_msg_hdr)+sizeof(char));

    	if (res == 0) {
	    	struct rtx_msg_hdr *hdr = (struct rtx_msg_hdr) ptr;

	    	// registering command 
	    	if (hdr->type == KCD_REG) {
	    		char *c = ptr + msg_hdr_size;
	    		insert(c, gp_current_task->tid);

	    	// keyboard input
	    	} else if (hdr->type == KEY_IN) {
	    		char *c = ptr + msg_hdr_size;
				if (c == '\n' || c == '\r') {
					// ready to send the string
					if (str[0] != '%' || counter > 64) {
						SER_PutChar(1, "Invalid Command");
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
	    			str[counter] = c;
	    			++counter;
	    		}
	    	}
    	}

    }

}

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

void insert(char key,task_t data) {

	struct DataItem *item = (struct DataItem*) mem_alloc(sizeof(struct DataItem));
	item->data = data;  
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