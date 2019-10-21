#include <printf.h>
#include <sched.h>
extern thread_t thread_id_self(void);
void _thread_exit (void *status){	
	/*status*/
	struct pcb_struct *now = current;
	if(now->state_cond.__queue){
		struct pcb_struct *next = now->state_cond.__queue;
		next -> state = TASK_RUNNING;
	}
	while(send_msg(END_Thread,thread_id_self(),0,0,0)){
		schedule();
	}
	printf("EXIT:%x\n\r",now);		
	*(now->state) = THREAD_EXITED;
	schedule();	
}
 
