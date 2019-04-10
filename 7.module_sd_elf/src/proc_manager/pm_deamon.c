#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>



struct mailbox mail[mail_size];
extern struct pcb_struct *thread_id_table[4096];
extern unsigned long mod_process;
/*FIFO*/
void pm_daemon(void)
{
	
	static int read_mail_index = 0;
	printf("Process Manager Starts running....\n\r");
	printf("Process Manager Starts receiving messages....\n\r");
	
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
	
        while(1){
		if(read_mail_index == 64){
			read_mail_index = 0;
		}
                struct pcb_struct *tmp_pcb = thread_id_table[mail[read_mail_index].dst_task];
		switch(mail[read_mail_index].letter_type){
			case Rendezvous:
				//send();

				read_mail_index++;
				break;

			case Mailbox:
				//send();
				read_mail_index++;
				break;

			case END_Thread:
				tmp_pcb-> prevp->nextp = tmp_pcb-> nextp;
				tmp_pcb-> nextp ->prevp = tmp_pcb-> prevp;
				free_page(mail[read_mail_index].dst_task);				
				read_mail_index++;				
				break;
			default:
				schedule();
			
		}

		
	}
}




void send_pm_daemon(unsigned int type, thread_t thread_id, unsigned int msg){
	static int put_mail_index = 0;
	mail[put_mail_index].letter_type = type;
	mail[put_mail_index].dst_task = thread_id;/*pid?*/
	mail[put_mail_index++].msg = msg;
}
