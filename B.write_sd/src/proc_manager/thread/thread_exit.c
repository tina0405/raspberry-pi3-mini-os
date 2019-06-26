#include <printf.h>
#include <sched.h>


void _thread_exit (void *status){	
	/*status*/
	struct pcb_struct *now = current;
	if(now->state_cond.__queue){
		struct pcb_struct *next = now->state_cond.__queue;
		next -> state = TASK_RUNNING;
	}
	send_msg(END_Thread,thread_id_self(),0);
	//printf("EXIT:%x\n\r",now);		
	*(now->state) = THREAD_EXITED;
	schedule();	
}
 
