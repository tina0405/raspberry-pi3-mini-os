#include "sched.h"
int thread_cond_wait (struct thread_cond *cond,struct thread_mutex * mutex){
	return thread_cond_timedwait_internal(cond, mutex, 0);
}

int thread_cond_timedwait_internal (struct thread_cond *cond,struct thread_mutex *mutex,unsigned int abstime)
{

	if(abstime==0){
	      thread_block();/*yield CPU*/
	}else{
		wait_msec_st(abstime);/*wait for abstime*/
	}

}

void thread_block (void){
	*(current -> state) = TASK_ZOMBIE;
	schedule();
}

