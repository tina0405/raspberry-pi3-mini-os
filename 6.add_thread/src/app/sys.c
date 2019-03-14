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

#define	EAGAIN 11	/* No more processes */
#define	ENOMEM 12	/* Not enough core */
int sys_create_thread(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg){
	
	int err;
	struct task_struct *new_thread;//struct __pthread *pthread;
	err = __thread_create_internal (&new_thread, attr, start_routine, arg);
	if (!err)
		*thread = new_thread->thread_id;
	else if (err == ENOMEM)
		err = EAGAIN;
	return err;

	
}

int __thread_create_internal (struct task_struct **thread, const struct thread_attr_t * attr, void *(*start_routine) (void *), void *arg)
{

	int err;
	static int tid;
  	struct task_struct *pthread;

 	/* Allocate a new thread structure. */

  	unsigned long page = allocate_kernel_page();
	//pthread = (struct task_struct *)page;
	printf("current:%d\n\r",current);
	pthread = current;
        struct pt_regs *childregs = task_pt_regs(pthread);

	/* Use the default attributes if ATTR is NULL.  */
/*
        pthread -> state = TASK_RUNNING;
	pthread -> counter = current -> priority;
	pthread -> priority = current -> priority;
	pthread -> preempt_count = 1;
	pthread -> flags = 0;
	pthread -> mm = current -> mm;
	pthread -> thread_id = tid++;


	struct pt_regs * cur_regs = task_pt_regs(current);
        *cur_regs = *childregs;
	printf("childreg.pc:%d\n\r",childregs->pc); 
	childregs->regs[0] = 0;
*/	


	childregs->pstate = PSR_MODE_EL0t;
	childregs->pc = (unsigned long)start_routine + user_page_start;
  
	pthread->cpu_context.pc = (unsigned long)ret_from_fork;
	pthread->cpu_context.sp = (unsigned long)childregs;

	

	int ppid = nr_tasks++;
	task[ppid] = pthread;	
	printf("ppid:%d\n\r",ppid); 
	*thread = pthread;

	return ppid;

	
}

void * const sys_call_table[] = {sys_write, sys_fork, sys_exit, sys_led, sys_read, sys_write_int,  sys_create_thread};
