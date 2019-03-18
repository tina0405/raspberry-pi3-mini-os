#include <stddef.h>
#include "fork.h"
#include "printf.h"
#include "utils.h"
#include "sched.h"
#include "mm.h"
#include "led_blink.h"
#include "user_sys.h"
#include "sys.h"
#include "entry.h"

void thread_end(void)
{
   /*thread id to kernel*/
   call_sys_write("this thread have ended!\n\r");
   while(1){}
}

int add_thread(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg){
	
	preempt_disable();
	static int tid = 0; 	
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
	childregs->sp = PAGE_SIZE*2;
	childregs->pstate = cur_regs->pstate;
	
	copy_virt_memory(p);/*here*/
	
	p->flags = 0;
	p->priority = 2;
	p->state = TASK_RUNNING;
	p->counter = p->priority;
	p->preempt_count = 1; //disable preemtion until schedule_tail
	p->cpu_context.pc = (unsigned long)ret_from_fork;
	p->cpu_context.sp = (unsigned long)childregs;
	int pid = nr_tasks++;

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
	task[pid] = pcb;
	
	printf("pid:%x\n\r",pid);
	printf("pcb:%x\n\r",pcb);
	preempt_enable();
	return pid;
}

/* Return the thread ID of the calling thread.  */
thread_t thread_id_self ( void ){	
	return current -> tid;	/*now task*/
}

int pthread_join ( thread_t thread, void **value_ptr ){

return 0;
}
void pthread_exit (void *value_ptr){

}




