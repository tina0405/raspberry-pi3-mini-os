#include "threadtype.h"
#include <stddef.h>
#include "sched.h"
/* Lock MUTEX, block if we can't get it.  */
extern struct pcb_struct *thread_id_table[4096]; 
int _thread_mutex_lock (struct thread_mutex *mutex)
{
  /*owner*/
  while(try_lock(&(mutex->__lock))!=0)
  {
	
	
	if(mutex->__attr == 0){
		schedule();
	}
	else
	{
		thread_t self = thread_self();
		if(mutex->__owner == self)
		    break;	

		thread_enqueue (&mutex->__queue, self);
		thread_block (self);
		
	}
  
  }
  mutex->__owner = thread_self ();
  return 0;//Successfully acquired the lock  	

}

void thread_enqueue(struct pcb_struct ** head,thread_t thread){
	

	while(*head != NULL){
		head = (**head).prevp;
	}
	
	*head = thread_id_table[thread];
		
}

