#include <stddef.h>
#include <sched.h>
#include <printf.h>
#include <cm.h>
struct mailbox compt_mail[mail_size]={NULL};
static int index_pop = 0;
//int support_type[4] = {0}; /*0:fat16*/
extern struct com_file cfile[64];/*Component Table*/
extern struct symbol_struct ksym[128];/*Kernel Table*/
void compt_daemon(void)
{	

	//printf("File System send a message (int 3) to ipc_test (use pid as an address)\n\r");
	//send_msg(Rendezvous, 2, 3);

    
	printf("Component manager Starts running....\n\r");
	printf("Component manager starts receiving messages....\n\r");
	memzero(&cfile[0], 64*sizeof(cfile[0]));
	memzero(&ksym[kapi_count], (128-kapi_count)*sizeof(ksym[0]));
	
	
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
	while(1){
	
		while(compt_mail[index_pop].letter_type){	
			switch(compt_mail[index_pop].letter_type){
				case INCOM:
					compt_file(compt_mail[index_pop].msg);
					compt_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}
					break;
				case RMCOM:
					compt_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}
					break;
				case SWAP:
					compt_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}
					break;	
				case Change_Sched:
					compt_sched_file(compt_mail[index_pop].msg);
					compt_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}
					break;
				
				default:
					break;
			}
		}
		
		schedule();
	}
	


}

