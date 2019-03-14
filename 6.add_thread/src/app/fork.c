#include "mm.h"
#include "sched.h"
#include "fork.h"
#include "utils.h"
#include "entry.h"
#include "printf.h"
#include "user_sys.h"
extern unsigned long user_page_start;
extern unsigned long uart_send;
extern unsigned long add;
unsigned long  page_next;
unsigned long  page_prev;
struct task_struct *prev_real;
struct task_struct *next_real;
int copy_process(unsigned long clone_flags, unsigned long fn,unsigned long arg)
{
	
	preempt_disable();
	struct pcb_struct *pcb;
	struct task_struct *p;
	
    	
	unsigned long page = allocate_kernel_page();
	unsigned long page_1 = allocate_kernel_page();
	
	p = (struct task_struct *) page;
	pcb = (struct pcb_struct *) page_1;



	struct pt_regs *childregs = task_pt_regs(p);

	if (!p)
		return -1;

	if (clone_flags==PF_KTHREAD) {
		p->cpu_context.x19 = fn;
		p->cpu_context.x20 = arg;
	} else if(clone_flags==0){
		struct pt_regs * cur_regs = task_pt_regs(&(current->cpu_context->x19));
		*cur_regs = *childregs;
		childregs->regs[0] = 0;
		copy_virt_memory(p);
	}
	p->flags = clone_flags;
	p->priority = *(current->priority);
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
	pcb -> mm = &(p->mm);
	//pcb -> thread_id = &(p->thread_id);
	/* interface */
	task[pid] = pcb;
	
	printf("pid:%x\n\r",pid);
	printf("pcb:%x\n\r",pcb);
	preempt_enable();
	return pid;
}


int add_thread(void * (*addr)(void *),void* arg){
	
	/*improve*/        

	/*improve*/ 
	jump_thread((unsigned long)addr+user_page_start,arg);
        return 1; 
}

unsigned long move_to_user_mode(unsigned long start, unsigned long size, unsigned long pc)
{

	struct pt_regs *regs = task_pt_regs(&(current->cpu_context->x19));

	regs->pstate = PSR_MODE_EL0t;
	regs->pc = pc;
	regs->sp = 2 *  PAGE_SIZE; 

	unsigned long code_page = allocate_user_page(&(current->cpu_context->x19), 0);
	if (code_page == 0)	{
		return -1;
	}
	printf("page_user:%x\n\r",code_page);
	memcpy(start, code_page, size);
	struct task_struct *now = &(current->cpu_context->x19);
	set_pgd(now->mm.pgd);

	return code_page;
}

struct pt_regs * task_pt_regs(struct task_struct *tsk)
{
	unsigned long p = (unsigned long)tsk + THREAD_SIZE - sizeof(struct pt_regs);
	return (struct pt_regs *)p;
}
