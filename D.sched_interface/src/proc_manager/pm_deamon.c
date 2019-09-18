#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>
#include <stddef.h>

struct mailbox pm_mail[mail_size]={NULL}; /*Mailbox*/
struct mailbox rendezvous; /*Rendezvous*/
extern struct pcb_struct *thread_id_table[4096];
extern unsigned long mod_process;
int index_push = 0;
int index_pop = 0;
struct mailbox user_ipc_mail[mail_size]; /*Mailbox*/
int ipc_index_pop=0;
extern unsigned char _start_;
/*FIFO*/
char name[11];

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
				
				case Change_Sched:
					compt_sched_file(pm_mail[0].msg);
					pm_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}
					break;

				case END_Thread:	
					tmp_pcb = pm_mail[index_pop].from;
						
					if(tmp_pcb->main_thread == tmp_pcb){/*main thread*/
						while(tmp_pcb!=NULL){
							tmp_pcb -> prevp -> nextp = tmp_pcb-> nextp;
					
							if(tmp_pcb -> nextp!=NULL){
								tmp_pcb -> nextp -> prevp = tmp_pcb-> prevp;
							}
							
							free_page(tmp_pcb);
						        free_page(&(tmp_pcb->cpu_context->x19));
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
						
						free_page(tmp_pcb);
						free_page(&(tmp_pcb->cpu_context->x19));
						/*free used memory*/	

					}				
					
					pm_mail[index_pop++].letter_type = 0;
					if(index_pop== mail_size){index_pop=0;}				
					break;
				default:
					break;
			}
		}
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
		schedule();
	}

	


}

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
char name[11];

/*type: mailbox or rendezvous or end_thread*/
void send_msg(unsigned int type, int tid, void* msg){/*without size*/
	/*push*/	
	if(pm_mail[index_push].letter_type != 0){
		printf("Push pm_mail error! Mailbox is Full\n\r");	
	}
	else{
		pm_mail[index_push].letter_type = type;/*0:empty*/
		pm_mail[index_push].dst_task = tid;/*exit_thread*/
		pm_mail[index_push].from = current;
		struct mm_info msg_mm = allocate_kernel_page(4096);	
		pm_mail[index_push].msg = msg_mm.start;
		
		memcpy(msg, msg_mm.start,11);
		index_push++;
		if(index_push == mail_size){index_push=0;}
		//accept_reply();
	}
	
}


