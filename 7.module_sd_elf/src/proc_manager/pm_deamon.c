#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>



static struct mailbox pm_mail[mail_size]; /*Mailbox*/
static struct mailbox rendezvous; /*Rendezvous*/
extern struct pcb_struct *thread_id_table[4096];
extern unsigned long mod_process;
static int index_push = 0;
static int index_pop = 0;

/*FIFO*/

void pm_daemon(void)
{
	

	printf("Process Manager Starts running....\n\r");
	printf("Process Manager Starts receiving messages....\n\r");
	
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
	
        while(1){
		//listening
		/*pop*/
		struct pcb_struct *tmp_pcb = task[pm_mail[index_pop].dst_task];
		switch(pm_mail[index_pop].letter_type){
			case Rendezvous:
				tmp_pcb->Rdv = pm_mail[index_pop];
				pm_mail[index_pop].letter_type = 0;	
				index_pop++;			
				if(index_pop== mail_size){index_pop=0;}
				break;

			case Mailbox:
				pm_mail[index_pop++].letter_type = 0;
				if(index_pop== mail_size){index_pop=0;}
				break;

			case END_Thread:
				tmp_pcb-> prevp->nextp = tmp_pcb-> nextp;
				tmp_pcb-> nextp ->prevp = tmp_pcb-> prevp;
				free_page(pm_mail[index_pop].dst_task);	
				pm_mail[index_pop++].letter_type = 0;
				if(index_pop== mail_size){index_pop=0;}				
				break;
			default:
				schedule();
			
		}

		
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


/*type: mailbox or rendezvous or end_thread*/
void send_msg(unsigned int type, int pid, int msg){
	/*push*/	
	if(pm_mail[index_push].letter_type != 0){
		printf("Push pm_mail error! Mailbox is Full");	
	}
	else{
		pm_mail[index_push].letter_type = type;/*0:empty*/
		pm_mail[index_push].dst_task = pid;/*pid?*/
		pm_mail[index_push].from = current;
		pm_mail[index_push++].msg = msg;
		if(index_push == mail_size){index_push=0;}
		accept_reply();
	}
	
}
