#include <stddef.h>
#include "sched.h"
#include "printf.h"
int _thread_mutex_unlock (struct thread_mutex *mutex)
{
	int ret = unlock(&mutex->__lock);
	//printf("unlock:%d", ret);
	if(ret != 0 ){
		return 1; 
	}

		
	thread_dequeue(&mutex->__queue,mutex);
	return 0;
}
/*dequeue*/

void thread_dequeue(struct pcb_struct ** head,struct thread_mutex *mutex){
	
	if(*head == NULL){
		
	}
	else{
		while(try_lock(&mutex->__lock)!=0){}
		mutex->__owner = (**head).tid;
		*head =	(**head).prevp;
	}

}

