#include "printf.h"
#include "sched.h"
#include <stddef.h>
extern struct pcb_struct *thread_id_table[4096];
int _thread_join (thread_t thread, void **status){
		
	
	struct pcb_struct *pcb_thread; /*interface*/	
	int *msg;
	int pid_map;
	/*without thread id*/
	if(thread_id_table[thread]==0){
		error();
	}
	pcb_thread = thread_id_table[thread];
	printf("%x\n\r",pcb_thread);

	/*mutex*/

	/*__pthread_cond_wait*/
	while (*(pcb_thread->state) == THREAD_JOINABLE)
    		thread_cond_wait (&pcb_thread->state_cond, &pcb_thread->state_lock);

	
		
	*(current -> state) = TASK_ZOMBIE;
	while(1){}/*tmp*/
	
	

	switch (*(pcb_thread->state))
	{
		case THREAD_EXITED:
		//THREAD has already exited.  Salvage its exit status.
			if (status != NULL)
				*status = pcb_thread-> status;
			//__pthread_mutex_unlock (&pthread->state_lock);
			//__pthread_dealloc (pthread);
			break;

		case THREAD_TERMINATED:
			//Pretend THREAD wasn't there in the first place.
			//__pthread_mutex_unlock (&pthread->state_lock);
			//err = ESRCH;
			break;

		default:
			//Thou shalt not join non-joinable threads! 
			//__pthread_mutex_unlock (&pthread->state_lock);
			//err = EINVAL;
			break;
      }

	return 0;
}


