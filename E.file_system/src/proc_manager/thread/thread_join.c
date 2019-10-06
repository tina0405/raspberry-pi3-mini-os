#include "printf.h"
#include "sched.h"
#include <stddef.h>
#include <errno.h> 
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

	/*mutex*/
	_thread_mutex_lock (&pcb_thread->state_lock);
	
	/*__pthread_cond_wait*/
	while(*(pcb_thread->state) == THREAD_JOINABLE){
	    schedule(); /*block*/
	}

	
	switch (*(pcb_thread->state))
	{
		case THREAD_EXITED:
		//THREAD has already exited.  Salvage its exit status.			
			if (status != NULL)
				*status = *(pcb_thread->state);
			_thread_mutex_unlock (&pcb_thread->state_lock);
			//__pthread_dealloc (pthread);
			break;

		default:
			//Thou shalt not join non-joinable threads! 
			thread_mutex_unlock (&pcb_thread->state_lock);
			int err = EINVAL;
			printf("Error number: ",EINVAL);
			break;
      }

	return 0;
}


