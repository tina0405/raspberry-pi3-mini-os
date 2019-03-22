#include "sched.h"
#include "irq.h"
#include "printf.h"
#include "utils.h"
#include "mm.h"
#include "fork.h"

struct cpu_context init_cpu = {0,0,0,0,0,0,0,0,0,0,0,0,0};
long init_state = 0;
long init_counter = 0;
long init_priority = 15;
long init_preempt_count = 0;
unsigned long init_flags = PF_KTHREAD;
struct mm_struct init_mm = { 0, 0, {{0}}, 0, {0}};
struct pcb_struct init_task = {&init_cpu,&init_state,&init_counter,&init_priority,&init_preempt_count,&init_flags,&init_mm};
struct pcb_struct *current = &(init_task);


struct pcb_struct *task_IO[NR_TASKS]={&init_task,}; /*Hardware*/
struct pcb_struct *task_SERVER[NR_TASKS]={&init_task,}; /*Server*/
struct pcb_struct *task_Change[NR_TASKS]={&init_task,}; /*change task*/
struct pcb_struct *task[NR_TASKS]={&init_task,}; /*user*/ 

struct pcb_struct **Task_Priority[4]={&task_IO[0],&task_SERVER[0],&task_Change[0],&task[0]};


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
	printf("scheduler\n\r");
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

/*add bill*/
		for (int i = 0; i < NR_TASKS; i++) {
			pcb = task[i];
			if (pcb) {
				*(pcb->counter) = (*(pcb->counter) >> 1) + *(pcb->priority);

			}

		}
	}
	printf("choose task:%d\n\r",next);
	switch_to(task[next]);
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
		prev_real = (struct task_struct *)page_prev;
		next_real =(struct task_struct *) page_next;
		flag = 1;
	}
	else{
		/*save old data*/
		printf("prev:%x\n\r",prev);
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
		if (task[i] == current) {
			*(task[i]->state) = TASK_ZOMBIE;
			break;
		}
	}
	preempt_enable();
	schedule();
}
