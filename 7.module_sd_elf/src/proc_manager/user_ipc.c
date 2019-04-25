#include <pm.h>
#include <sched.h>
#include <printf.h>
struct mailbox user_ipc_mail[mail_size]; /*Mailbox*/
int ipc_index_pop=0;
void user_ipc_service(void)
{
	
	printf("User IPC Service Starts running....\n\r");
	printf("User IPC Service Starts receiving messages....\n\r");
	
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
        while(1){
		//listening
		/*pop*/
		switch(user_ipc_mail[ipc_index_pop].letter_type){
			case 0:
				schedule();			
				break;
			default:
				printf("User IPC Service recieve a msg!\n\r");
				send_msg(user_ipc_mail[ipc_index_pop].letter_type, user_ipc_mail[ipc_index_pop].dst_task, user_ipc_mail[ipc_index_pop].msg);
				user_ipc_mail[ipc_index_pop].letter_type = 0;	
				ipc_index_pop++;			
				if(ipc_index_pop== mail_size){ipc_index_pop=0;}
			
		}

		
	}

}
