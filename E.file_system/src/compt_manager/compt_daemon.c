#include <stddef.h>
#include <sched.h>
#include <printf.h>
struct mailbox compt_mail[mail_size]={NULL};
static int index_pop = 0;
//int support_type[4] = {0}; /*0:fat16*/

void compt_daemon(void)
{	

	//printf("File System send a message (int 3) to ipc_test (use pid as an address)\n\r");
	//send_msg(Rendezvous, 2, 3);

    
	printf("Component manager Starts running....\n\r");
	printf("Component manager starts receiving messages....\n\r");

	
	
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
	while(1){
	
		while(compt_mail[index_pop].letter_type){	
			switch(compt_mail[index_pop].letter_type){
				case 1:
					//tmp_pcb = task[fs_mail[index_pop].dst_task];
					//tmp_pcb->Rdv = fs_mail[index_pop];
					compt_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}
					break;
				case 2:
					break;
				
				default:
					break;
			}
		}
		
		schedule();
	}
	


}

