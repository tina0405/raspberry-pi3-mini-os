#include<sched.h>
#include <stddef.h>
struct pcb_struct* sched_policy(struct pcb_struct * current_task, struct pcb_struct * head){	
	struct pcb_struct * tmp_task = head;
	while(1){
		if(head->nextp == NULL){	
			return NULL;
		}else{
			if(*(head->nextp->counter)==0){
				*(head->nextp->counter) = 1;
				return NULL;
			}
			if(*(tmp_task->nextp->state)== TASK_RUNNING||*(tmp_task->nextp->state) == THREAD_JOINABLE||*(tmp_task->nextp->state) == THREAD_DETACHED){
				return tmp_task->nextp;
			}else{
				tmp_task = tmp_task->nextp;			
			}
		}
	}	
}


