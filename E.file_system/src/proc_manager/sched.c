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
long init_extra_priority = 10;
long init_app_priority = 1;
long init_preempt_count = 0;
unsigned long init_flags = SERVER_THREAD;
unsigned long init_extra_flags = EXTRA_SERVER_THREAD;
unsigned long init_app_flags = APP_THREAD;
struct mm_struct init_mm = { 0, 0, {{0}}, 0, {0}};
struct pcb_struct init_task_0 = {&init_cpu,&init_state,&init_counter,&init_priority,&init_preempt_count,&init_flags,&init_mm,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
struct pcb_struct init_task_1 = {&init_cpu,&init_state,&init_counter,&init_extra_priority,&init_preempt_count,&init_extra_flags,&init_mm,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
struct pcb_struct init_task_2 = {&init_cpu,&init_state,&init_counter,&init_app_priority,&init_preempt_count,&init_app_flags,&init_mm,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
//struct pcb_struct init_task_2 = {&init_cpu,&init_state,&init_counter,&init_priority,&init_preempt_count,&init_flags,&init_mm};
//struct pcb_struct init_task_3 = {&init_cpu,&init_state,&init_counter,&init_priority,&init_preempt_count,&init_flags,&init_mm};
struct pcb_struct *current = &(init_task_0);
/*linked list*/
struct pcb_struct *task_prio_table[3] = {&init_task_0, &init_task_1, &init_task_2}; /*Hardware*/ /*Server*/ /*Change task*/ /*User*/
struct pcb_struct *head[3]={&init_task_0, &init_task_1, &init_task_2};
extern unsigned char _start_;



int nr_tasks = 0;

void preempt_disable(void)
{
	//printf("preempt_disable\n\r");
	*(current->preempt_count) = *(current->preempt_count) +1;
}

void preempt_enable(void)
{	
	//printf("preempt_enable\n\r");
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
int service_index = 0;
int is_running(struct pcb_struct * task){
	return (*(task->state)== TASK_RUNNING||*(task->state) == THREAD_JOINABLE||*(task->state) == THREAD_DETACHED);
}
struct pcb_struct* round_robin(struct sched_interface* sched){
	*(sched->current_task->counter) = 1;	

	struct pcb_struct * tmp_task = sched->current_task;
	while(1){	
		if(tmp_task->nextp == NULL){	
			return NULL;			
		}else{
			if(is_running(tmp_task->nextp)){
				return tmp_task->nextp;
			}else{
				tmp_task = tmp_task->nextp;			
			}

		}
	}
}



struct sched_interface* sched;
void _schedule(void)
{
	preempt_disable();
	
	while(1){
		if(service_index == SERVER_THREAD){
			if(task_prio_table[0]->nextp == NULL){	
				task_prio_table[0] = head[0];
				service_index = EXTRA_SERVER_THREAD;			
			}else{
				task_prio_table[0] = task_prio_table[0]->nextp;
				if(is_running(task_prio_table[0])){
					break;
				}
			}

		}else if(service_index == EXTRA_SERVER_THREAD){
			*(task_prio_table[1]->counter) = 10;	
			if(task_prio_table[1]->nextp == NULL){	
				task_prio_table[1] = head[1];
				service_index = APP_THREAD;			
			}else{
				task_prio_table[1] = task_prio_table[1]->nextp;
				if(is_running(task_prio_table[1])){
					break;
				}
			}
		
		}
		else{
			sched->current_task = task_prio_table[2];
			sched->head = head[2];
			struct pcb_struct* result = bl_sched_wrap(sched, &_start_+(sched_type-(unsigned int)&_start_));
		
			if(result == NULL){
				task_prio_table[2] = head[2]; 
				service_index = 0;
			}else{
				task_prio_table[2] = result; 
				break;
			}

		}

	}

	switch_to(task_prio_table[service_index]);
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
struct mm_info page_next_mm;
struct mm_info page_prev_mm;
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
		page_prev_mm = allocate_kernel_page(4096);
		page_next_mm = allocate_kernel_page(4096);
		page_prev = page_prev_mm.start;
		page_next = page_next_mm.start;
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
	//printf("tick");
	if(service_index == SERVER_THREAD){return;}/*kernel service*/
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
