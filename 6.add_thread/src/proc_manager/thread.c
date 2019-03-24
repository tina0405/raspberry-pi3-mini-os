#include <stddef.h>
#include "sched.h"
#include "fork.h"
#include "printf.h"
#include "utils.h"
#include "mm.h"
#include "led_blink.h"
#include "user_sys.h"
#include "sys.h"
#include "entry.h"
#include "threadtype.h"
struct pcb_struct *thread_id_table[4096] ={0};/*tid and pid map*/
/*thread table Pointer thread_id-> pointer*/
extern int next;


void thread_end(void) /*Normal end*/
{
   /*thread id to kernel*/
   call_sys_write("this thread have ended!\n\r");
   int msg = 1;
   //send(*(current -> mailbox),&msg);
   while(1){}
   /**/


}

void pthread_exit (void *status){ /*force to end*/

}



void error(void){
    printf("Fail!");
    *(current -> state) = TASK_ZOMBIE;
    while(1){}
}

int add_thread(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg){
	struct pcb_struct *tmp_pcb;
	preempt_disable();
	static int tid = 0; 
	int pid = nr_tasks++;
		
	*thread = tid++;	
	
	struct pcb_struct *pcb; /*interface*/
	struct task_struct *p; /*new space, just need pt_regs*/
	struct task_struct *now = &(current->cpu_context->x19);	/*now task*/
	printf("now:%x\n\r",now);
	unsigned long page = allocate_kernel_page();
	unsigned long page_1 = allocate_kernel_page();
	
	p = (struct task_struct *) page;
	pcb = (struct pcb_struct *) page_1;
	
	/*new space for childregs*/
	struct pt_regs *childregs = task_pt_regs(p);

	if (!p)
		return -1;

	/*copy*/
	struct pt_regs * cur_regs = task_pt_regs(&(current->cpu_context->x19));
	//*cur_regs = *childregs;
	childregs->regs[0] = arg;
	childregs->regs[30] = (unsigned long)&thread_end;
	childregs->pc =	start_routine;
	childregs->sp = PAGE_SIZE*1;
	childregs->pstate = cur_regs->pstate;
	
	copy_virt_memory(p);/*here*/
	
	p->flags = 0;
	p->priority = 2;
	p->state = TASK_RUNNING;
	p->counter = p->priority;
	p->preempt_count = 1; //disable preemtion until schedule_tail
	p->cpu_context.pc = (unsigned long)ret_from_fork;
	p->cpu_context.sp = (unsigned long)childregs;
	

	/* interface */
	pcb -> cpu_context = &(p->cpu_context);
	pcb -> state = &(p->state);
	pcb -> counter = &(p->counter);
	pcb -> priority = &(p->priority);
	pcb -> preempt_count = &(p->preempt_count);
	pcb -> flags = &(p->flags);
	pcb -> mm = &(now->mm);
	pcb -> pid = pid;
	pcb -> tid = *thread;
	//pcb -> thread_id = &(p->thread_id);
	/* interface */

	tmp_pcb = task_prio_table[0];
	while (tmp_pcb ->nextp != NULL){
		tmp_pcb = tmp_pcb -> nextp;
	}
	tmp_pcb->nextp = pcb;
	//task[pid] = pcb;
	thread_id_table[tid]=pcb;
	printf("pid:%x\n\r",pid);
	printf("pcb:%x\n\r",pcb);
	preempt_enable();
	return pid;
}

/* Return the thread ID of the calling thread.  */
thread_t thread_id_self ( void ){	
	return current -> tid;	/*now task*/
}

/*temperate*/
void recieve(struct pcb_struct *src,int* msg){
	static int recv_index = 0;
	while(1){	
		while(current -> mail[recv_index].time){
			recv_index++;
			if(recv_index==15){
			    recv_index=0;
			}

                        		
		}

		if(src == current -> mail[recv_index].from_where)
		{
		    *msg = current->mail[recv_index].status;
		    //current->mail[recv_index] = NULL;
		    break;
		} else {
			recv_index++;
			if(recv_index==15){
			    recv_index=0;
			}

		}
	}
}


/*temperate*/

void send(struct pcb_struct *dst,int* msg){
	
}
void hi(void){
   printf("hihihihi\n\r");
}

void signal(thread_t thread){
	
	struct pcb_struct *pcb_thread; /*interface*/	
/*	int pid_map;
	printf("t");
	if(thread_id_table[thread]>=0){
		printf("i");
		pid_map = thread_id_table[thread];/*wait for which thread*/
//	}else{

//		error();
//	}
	
	//pcb_thread = task[3];
	//printf("%x\n\r",pcb_thread);
        if (pcb_thread == NULL)
		error();

	//pcb_thread->cpu_context->pc = (unsigned long)ret_from_fork;
	
	//unsigned int *addr = (pcb_thread->cpu_context->sp+8);
	//*addr = &hi;
	*(pcb_thread -> state) = TASK_RUNNING;
}



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





/* Lock MUTEX, block if we can't get it.  */
int __pthread_mutex_lock (struct thread_mutex *mutex)
{
  int err;
  int drain;
  struct  pcb_struct *self;
  const struct __pthread_mutexattr *attr = mutex->__attr;

  if (attr == &__pthread_errorcheck_mutexattr)
    attr = &__pthread_errorcheck_mutexattr;
  if (attr == &__pthread_recursive_mutexattr)
    attr = &__pthread_recursive_mutexattr;
  //__pthread_spin_lock (&mutex->__lock);
  
/*
   if (__pthread_spin_trylock (&mutex->__held) == 0)
    //Successfully acquired the lock.
    {

      if (attr != NULL)
	switch (attr->__mutex_type)
	{
	  case __PTHREAD_MUTEX_NORMAL:
	    break;

	  case __PTHREAD_MUTEX_RECURSIVE:
	    mutex->__locks = 1;
	  case __PTHREAD_MUTEX_ERRORCHECK:
	    mutex->__owner = thread_self ();
	    break;

	  default:
	    error();
	}

      //__pthread_spin_unlock (&mutex->__lock);
      return 0;
    }
*/
  /* The lock is busy.  */

  self = thread_self ();
  //assert (self);

  if (attr == NULL || attr->__mutex_type == __PTHREAD_MUTEX_NORMAL){}
  else
  {
      switch (attr->__mutex_type)
      {
	case __PTHREAD_MUTEX_ERRORCHECK:
	  if (mutex->__owner == self)
	    {
	      //__pthread_spin_unlock (&mutex->__lock);
	      error();
	    }
	  break;

	case __PTHREAD_MUTEX_RECURSIVE:
	  if (mutex->__owner == self)
	    {
	      mutex->__locks++;
	      //__pthread_spin_unlock (&mutex->__lock);
	      return 0;
	    }
	  break;

	default:
	  error();
	}
    }


    //assert (mutex->__owner);



  /* Add ourselves to the queue.  */
  //__pthread_enqueue (&mutex->__queue, self);
  //__pthread_spin_unlock (&mutex->__lock);
//
  /* Block the thread.  */
  err = 0;
  thread_block (self);
   

  //__pthread_spin_lock (&mutex->__lock);
  if (self->prevp == NULL)
    /* Another thread removed us from the queue, which means a wakeup message
       has been sent.  It was either consumed while we were blocking, or
       queued after we timed out and before we acquired the mutex lock, in
       which case the message queue must be drained.  */
    drain = err ? 1 : 0;
  else
    {
      /* We're still in the queue.  Noone attempted to wake us up, i.e. we
         timed out.  */
      //__pthread_dequeue (self);
      drain = 0;
    }
  //__pthread_spin_unlock (&mutex->__lock);

  if (drain)
    //__pthread_block (self);

  if (err)
  {
      error();/*timeout*/
      return err;
   }

      //assert (mutex->__owner == self);


  if (attr != NULL)
      switch (attr->__mutex_type)
      {
      	case __PTHREAD_MUTEX_NORMAL:
		break;

      	case __PTHREAD_MUTEX_RECURSIVE:
		//assert (mutex->__locks == 0);
		mutex->__locks = 1;
     	 case __PTHREAD_MUTEX_ERRORCHECK:
		mutex->__owner = self;
		break;

      	default:
		error();
      }

  return 0;
}

int thread_cond_timedwait_internal (struct thread_cond *cond,struct thread_mutex *mutex,unsigned int abstime)
{

	if(abstime==0){
	      thread_block();/*wait for signal*/
	}else{
		wait_msec_st(abstime);/*wait for abstime*/
	}

}

void thread_block (void){
	*(current -> state) = TASK_ZOMBIE;
	schedule();
}

int thread_cond_wait (struct thread_cond *cond,struct thread_mutex * mutex){
	return thread_cond_timedwait_internal(cond, mutex, 0);
}

