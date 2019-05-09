#include "sched.h"
#include "irq.h"
#include "printf.h"
#include "utils.h"
#include "mm.h"
#include "fork.h"
#include <stddef.h>
struct cpu_context init_cpu = {0,0,0,0,0,0,0,0,0,0,0,0,0};
long init_state = 0;
long init_counter = 0;
long init_priority = 15;
long init_preempt_count = 0;
unsigned long init_flags = SERVER_THREAD;
struct mm_struct init_mm = { 0, 0, {{0}}, 0, {0}};
struct pcb_struct init_task_0 = {&init_cpu,&init_state,&init_counter,&init_priority,&init_preempt_count,&init_flags,&init_mm};
struct pcb_struct init_task_1 = {&init_cpu,&init_state,&init_counter,&init_priority,&init_preempt_count,&init_flags,&init_mm};
struct pcb_struct init_task_2 = {&init_cpu,&init_state,&init_counter,&init_priority,&init_preempt_count,&init_flags,&init_mm};
struct pcb_struct init_task_3 = {&init_cpu,&init_state,&init_counter,&init_priority,&init_preempt_count,&init_flags,&init_mm};
struct pcb_struct *current = &(init_task_0);
/*linked list*/
struct pcb_struct *task_prio_table[4] = {&init_task_0, &init_task_1, &init_task_2, &init_task_3}; /*Hardware*/ /*Server*/ /*Change task*/ /*User*/
struct pcb_struct *head[4]={&init_task_0, &init_task_1, &init_task_2, &init_task_3};
/*linked list*/




int nr_tasks = 1;

void preempt_disable(void)
{
	*(current->preempt_count) = *(current->preempt_count) +1;
}

void preempt_enable(void)
{
	*(current->preempt_count) = *(current->preempt_count) -1;
}

/*
struct task_struct {
	struct cpu_context cpu_context;
	long state;	
	long counter;
	long priority;
	long preempt_count;
	unsigned long flags;
	struct mm_struct mm;
	int *thread_id;
}
struct pcb_struct {
	struct cpu_context *cpu_context;
	long *state;	
	long *counter;
	long *priority;
	long *preempt_count;
	unsigned long *flags;
	struct mm_struct *mm;
	int *thread_id;
	//struct task_struct *thread;

};
*/



int next = 0;
void _schedule(void)
{
	preempt_disable();
	
	int c;
	struct pcb_struct* pcb;
	//struct pcb_struct pcb;
	//printf("scheduler\n\r");
	static int index = 0;/*3 priority highest*/
	*(task_prio_table[index]->counter) = (*(task_prio_table[index]->counter) >> 1) + *(task_prio_table[index]->priority);
	while(1){

		while (task_prio_table[index]->nextp == NULL){	
			task_prio_table[index] = head[index];
			index++;			
			if(index == 4){ index = 0; }
			
		}
		//printf("%x\n\r",index);
		if(*(task_prio_table[index]->nextp->state) == TASK_RUNNING||*(task_prio_table[index]->nextp->state) == THREAD_JOINABLE||*(task_prio_table[index]->nextp->state) == THREAD_DETACHED){
			break;

		}else{
			task_prio_table[index] = task_prio_table[index]->nextp;
					
		}
		
	}
	
	task_prio_table[index] = task_prio_table[index]->nextp;

        
/*
	while (1) {
		c = -1;
		next = 0;
	
		for (int i = 0; i < NR_TASKS; i++){
			pcb = task[i];
			if (pcb && *(pcb->state) == TASK_RUNNING && *(pcb->counter) > c) {

				c = *(pcb->counter);
				next = i;
			}
		}
		if (c) {
			break;
		}


		for (int i = 0; i < NR_TASKS; i++) {
			pcb = task[i];
			if (pcb) {
				*(pcb->counter) = (*(pcb->counter) >> 1) + *(pcb->priority);

			}

		}
	}
*/
	//printf("task_prio_table:%d\n\r",index);
	switch_to(task_prio_table[index]);
	preempt_enable();
}

void schedule(void)
{

	*(current->counter) = 0;
	_schedule();
}

unsigned long  init_page; 
struct pt_regs *prev_childregs;
struct pt_regs *prev_real_regs;
struct pt_regs *next_childregs;
struct pt_regs *next_real_regs;
unsigned long  page_next;
unsigned long  page_prev;
struct task_struct *prev_real;
struct task_struct *next_real;
struct pcb_struct *prev;
void switch_to(struct pcb_struct * next) 
{	

	static int flag = 0;
	if (current == next) 
		return;
	

	if(flag ==0){
		page_prev = allocate_kernel_page();
		page_next = allocate_kernel_page();
		prev_real = (struct task_struct *) page_prev;
		next_real =(struct task_struct *) page_next;
		flag = 1;
	} 
	else{
		/*save old data*/
		//printf("prev:%x\n\r",prev);
		prev_real-> cpu_context = *(prev -> cpu_context);
	}

	prev = current;
	current = next;
	set_pgd(next->mm->pgd);

	*(next -> cpu_context) = next_real-> cpu_context;

	cpu_switch_to(prev_real, next_real);

	
}

void schedule_tail(void) {
	preempt_enable();
}


void timer_tick()
{
	--*(current->counter);

	if (*(current->counter)>0 || *(current->preempt_count) >0) {
		return;
	}
	*(current->counter)=0;

	enable_irq();
        //printf("interrupt schedule\n\r");
	_schedule();
	disable_irq();
}


void timer3_tick()
{

}



void exit_process(){
	preempt_disable();
	for (int i = 0; i < NR_TASKS; i++){
/*
		if (task[i] == current) {
			*(task[i]->state) = TASK_ZOMBIE;
			break;
		}
*/
	}
	preempt_enable();
	schedule();
}
