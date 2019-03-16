#include <stddef.h>
#include "fork.h"
#include "printf.h"
#include "utils.h"
#include "sched.h"
#include "mm.h"
#include "led_blink.h"
#include "mini_uart.h"
#include "user_sys.h"
#include "sys.h"
#include "entry.h"
extern unsigned long user_page_start;
extern unsigned long hi;
void sys_write(char * buf){	
	printf(buf);
}

void sys_write_int(int a){	
	pr_int(a);
}

int sys_fork(){
	return copy_process(0, 0, 0);
}

void sys_exit(){
	exit_process();
}

void sys_led(void){
	led_blink();
}

char sys_read(void){
	return uart_recv();
}

void thread_end(void)
{
   call_sys_write("this thread have ended!\n\r");
   while(1){}
}

int sys_create_thread(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg){
	
	preempt_disable();
	struct pcb_struct *pcb; /*interface*/
	struct task_struct *p; /*new space, just need pt_regs*/
	struct task_struct *now = &(current->cpu_context->x19);	/*now task*/
	
	unsigned long page = allocate_kernel_page();
	unsigned long page_1 = allocate_kernel_page();
	unsigned long page_2 = allocate_kernel_page();
	
	p = (struct task_struct *) page;
	pcb = (struct pcb_struct *) page_1;

	/*new space for childregs*/
	struct pt_regs *childregs = task_pt_regs(p);

	if (!p)
		return -1;

	/*copy*/
	struct pt_regs * cur_regs = task_pt_regs(&(current->cpu_context->x19));
	//*cur_regs = *childregs;
	childregs->regs[0] = 0;
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
	//pcb -> thread_id = &(p->thread_id);
	/* interface */
	task[pid] = pcb;
	
	printf("pid:%x\n\r",pid);
	printf("pcb:%x\n\r",pcb);
	preempt_enable();
	return pid;
}


void * const sys_call_table[] = {sys_write, sys_fork, sys_exit, sys_led, sys_read, sys_write_int,  sys_create_thread};
