//#include "threadtype.h"
#include <stddef.h>
#include "sched.h"
void _thread_cond_signal(thread_t thread){
	
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

