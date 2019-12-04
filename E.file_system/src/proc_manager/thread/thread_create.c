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
#include "pm.h"

extern struct pcb_struct *thread_id_table[4096];/*tid and pcb map*/
/*thread table Pointer thread_id-> pointer*/
extern int next;
extern int tid;
void thread_end(void) /*Normal end*/
{
   /*thread id to kernel*/
   //call_sys_write("this thread have ended!\n\r");
   /*kernel mode cannot schedule*/
   int a = 0;
   call_sys_exit(a);
}

void error(void){
    printf("Fail!");
    *(current -> state) = TASK_ZOMBIE;
    while(1){}
}

int _thread_create(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg){
	struct pcb_struct *tmp_pcb;
	preempt_disable();
      	
	
	struct pcb_struct *pcb; /*interface*/
	struct task_struct *p; /*new space, just need pt_regs*/
	struct task_struct *now = &(current->cpu_context->x19);	/*now task*/
	struct mm_info page_mm;
	struct mm_info page_1_mm;
		
	page_mm =  allocate_kernel_page(4096);
	page_1_mm = allocate_kernel_page(4096);
	unsigned long page = page_mm.start;
	unsigned long page_1 = page_1_mm.start;
	
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
	p->state = THREAD_JOINABLE;/*default*/
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
	pcb -> pid = current->pid;
	pcb -> main_thread = current;
	pcb -> h_count = 0;
	pcb -> hardware = NULL;
	/*need initial next thread*/
	while(thread_id_table[tid]!=0){		
		tid = tid + 1;	
		if(tid == 4096){tid=0;}
	}; 
        *thread = tid;
	pcb -> tid = tid;
	thread_id_table[tid] = pcb;

        /*schedule*/      
	tmp_pcb = head[APP_THREAD];
	if(tmp_pcb -> nextp != NULL){
		pcb -> nextp = tmp_pcb->nextp;
		pcb -> prevp = tmp_pcb;
		tmp_pcb -> nextp -> prevp = pcb;
		tmp_pcb -> nextp =pcb;
	}else{
		tmp_pcb->nextp =  pcb;
		pcb -> prevp =	tmp_pcb;
		pcb -> nextp =	NULL;				
	}

	//task[pid] = pcb;
    
	/*child thread*/
        struct pcb_struct * thread_mark = current;
	while(thread_mark->thread_n!=NULL){
		thread_mark = thread_mark->thread_n;
	}
	thread_mark->thread_n = pcb;
	pcb -> thread_p = thread_mark;
        pcb -> thread_n = NULL;
	preempt_enable();
	return 0;/*succeed*/
}



