#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>
#include <stddef.h>

struct mailbox pm_mail[mail_size]={NULL}; /*Mailbox*/
struct mailbox rendezvous; /*Rendezvous*/
extern struct pcb_struct *thread_id_table[4096];
extern unsigned long mod_process;
static int index_pop = 0;
//struct mailbox user_ipc_mail[mail_size]; /*Mailbox*/
int ipc_index_pop=0;
extern unsigned char _start_;
/*FIFO*/

void pm_daemon(void)
{
	

	printf("Process Manager Starts running....\n\r");
	printf("Process Manager Starts receiving messages....\n\r");
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
	struct pcb_struct *tmp_pcb;
        while(1){
		//printf("PM");
		//listening
		/*pop*/
		while(pm_mail[index_pop].letter_type){	
			switch(pm_mail[index_pop].letter_type){
				case Rendezvous:
					tmp_pcb = task[pm_mail[index_pop].dst_task];
					tmp_pcb->Rdv = pm_mail[index_pop];
					pm_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}
					break;

				case Mailbox:
					tmp_pcb = task[pm_mail[index_pop].dst_task];
					pm_mail[index_pop++].letter_type = 0;
					if(index_pop== mail_size){index_pop=0;}
					break;
				
				case END_Thread:	
					tmp_pcb = pm_mail[index_pop].from;
						
					if(tmp_pcb->main_thread == tmp_pcb){/*main thread*/
						while(tmp_pcb!=NULL){
							tmp_pcb -> prevp -> nextp = tmp_pcb-> nextp;
					
							if(tmp_pcb -> nextp!=NULL){
								tmp_pcb -> nextp -> prevp = tmp_pcb-> prevp;
							}
						
							free_page(tmp_pcb,1);
						        free_page(&(tmp_pcb->cpu_context->x19),1);
							tmp_pcb = tmp_pcb->thread_n;
						}

					}
					else/*child thread*/
					{
					    
						tmp_pcb -> prevp -> nextp = tmp_pcb-> nextp;
					
						if(tmp_pcb -> nextp!=NULL){
							tmp_pcb -> nextp -> prevp = tmp_pcb-> prevp;			
						}
						tmp_pcb -> thread_p -> thread_n = tmp_pcb-> thread_n;
							

						if(tmp_pcb -> thread_n!=NULL){
							tmp_pcb -> nextp -> prevp = tmp_pcb-> thread_p;
						}
						
						free_page(tmp_pcb,1);
						free_page(&(tmp_pcb->cpu_context->x19),1);
						/*free used memory*/	

					}				
					
					pm_mail[index_pop++].letter_type = 0;
					if(index_pop== mail_size){index_pop=0;}				
					break;
				default:
					break;
			}
		}
		/*
		while(user_ipc_mail[ipc_index_pop].letter_type){
			switch(user_ipc_mail[ipc_index_pop].letter_type){
				case Rendezvous:
					printf("User IPC Service recieve a msg!\n\r");
					tmp_pcb = task[user_ipc_mail[ipc_index_pop].dst_task];
					tmp_pcb->Rdv = user_ipc_mail[ipc_index_pop];
					user_ipc_mail[ipc_index_pop].letter_type = 0;	
					ipc_index_pop++;			
					if(ipc_index_pop == mail_size){ipc_index_pop=0;}
					break;	
				default:
					break;
			}	
		}
		*/
		schedule();
	}

	


}
/*
void reply(struct pcb_struct *letter){
	struct pcb_struct *from_pcb = letter; 
	from_pcb -> reply = 1; 

}

void accept_reply(void){

	while(current->reply == 0){
		schedule();
	}
	current->reply = 0;
}


struct mailbox recieve_msg(unsigned int ipc_type){
	if(ipc_type == Rendezvous){

		while(current->Rdv.letter_type == 0){
						
			schedule();
		}

		struct mailbox ret = current->Rdv;
		current->Rdv.letter_type = 0;	
		reply(current->Rdv.from);
		return ret;
	}else if(ipc_type == Mailbox){ 
		return current->Box[0];
	}

}


*/


