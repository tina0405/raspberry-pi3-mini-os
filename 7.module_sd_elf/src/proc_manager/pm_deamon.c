#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>



struct mailbox pm_mail[mail_size]; /*Mailbox*/
struct mailbox rendezvous; /*Rendezvous*/
extern struct pcb_struct *thread_id_table[4096];
extern unsigned long mod_process;
static int index_push = 0;
static int index_pop = 0;
/*FIFO*/
void reply(void){


}
void pm_daemon(void)
{
	

	printf("Process Manager Starts running....\n\r");
	printf("Process Manager Starts receiving messages....\n\r");
	
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
	
        while(1){
		//listening
		/*pop*/
		struct pcb_struct *tmp_pcb = thread_id_table[pm_mail[index_pop].dst_task];
		switch(pm_mail[index_pop].letter_type){
			case Rendezvous:
				//send();
				printf("recieve a  Rendezvous letter!\n\r");
				pm_mail[index_pop++].letter_type = 0;
				if(index_pop== mail_size){index_pop=0;}
				reply();
				break;

			case Mailbox:
				//send();
				printf("recieve a  Rendezvous letter!\n\r");
				pm_mail[index_pop++].letter_type = 0;
				if(index_pop== mail_size){index_pop=0;}
				reply();
				break;

			case END_Thread:
				tmp_pcb-> prevp->nextp = tmp_pcb-> nextp;
				tmp_pcb-> nextp ->prevp = tmp_pcb-> prevp;
				free_page(pm_mail[index_pop].dst_task);	
				pm_mail[index_pop++].letter_type = 0;
				if(index_pop== mail_size){index_pop=0;}
				reply();				
				break;
			default:
				schedule();
			
		}

		
	}

	


}



/*type: mailbox or rendezvous or end_thread*/
void send_pm_daemon(unsigned int type, thread_t thread_id, int msg){
	/*push*/	
	if(pm_mail[index_push].letter_type != 0){
		printf("Push pm_mail error! Mailbox is Full");	
	}
	else{
		pm_mail[index_push].letter_type = type;/*0:empty*/
		pm_mail[index_push].dst_task = thread_id;/*pid?*/
		pm_mail[index_push++].msg = msg;
		if(index_push == mail_size){index_push=0;}
	}
	
}
