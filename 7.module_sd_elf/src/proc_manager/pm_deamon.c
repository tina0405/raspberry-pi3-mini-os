#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>



struct mailbox pm_mail[mail_size]; /*Mailbox*/
struct mailbox rendezvous; /*Rendezvous*/
extern struct pcb_struct *thread_id_table[4096];
extern unsigned long mod_process;
static int stack_index = 0;
/*FIFO*/
void pm_daemon(void)
{
	

	printf("Process Manager Starts running....\n\r");
	printf("Process Manager Starts receiving messages....\n\r");
	
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
	
        while(1){
		if(stack_index == 0){
			schedule();
		}
                struct pcb_struct *tmp_pcb = thread_id_table[pm_mail[stack_index-1].dst_task];
		//listening
				
		switch(pm_mail[stack_index-1].letter_type){
			case Rendezvous:
				//send();
				printf("recieve a  Rendezvous letter!");
				stack_index--;
				reply();
				break;

			case Mailbox:
				//send();
				printf("recieve a  Rendezvous letter!");
				stack_index--;
				reply();
				break;

			case END_Thread:
				tmp_pcb-> prevp->nextp = tmp_pcb-> nextp;
				tmp_pcb-> nextp ->prevp = tmp_pcb-> prevp;
				free_page(pm_mail[stack_index-1].dst_task);				
				stack_index--;
				reply();				
				break;
			default:
				schedule();
			
		}

		
	}

	


}

void listen(struct pcb_struct *dst,int* msg){
	if(*msg){

		


	}
}


/*type: mailbox or rendezvous or end_thread*/
void send_pm_daemon(unsigned int type, thread_t thread_id, int msg){
	pm_mail[stack_index].letter_type = type;
	pm_mail[stack_index].dst_task = thread_id;/*pid?*/
	pm_mail[stack_index++].msg = msg;
}
