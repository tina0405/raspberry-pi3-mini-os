#include<sched.h>
#include <stddef.h>
#include <kservice.h>

struct sched_interface{
	struct pcb_struct * current_task;
	struct pcb_struct * head;
};
struct sched_interface input;
struct para_config drv_config ={
    op_func: 1,
    /*1 operation*/
    name: "LIFO",
    interface: SCH_DEF,
    pnum: 2,
    para_1: sizeof(input.current_task),
    para_2: sizeof(input.head),
};

void init_compt(void){ /*initial*/

	kservice_uart_write("Initial LIFO component!\n\r");
	if(!kservice_reg_compt("scheduler", SCH_COM ,&drv_config)){
		kservice_uart_write("Register is successful!");
	}
	
}

struct pcb_struct* LIFO(struct sched_interface* sched){	
	struct pcb_struct * tmp_task = sched->head;
	while(1){
		if(sched->head->nextp == NULL){	
			return NULL;
		}else{
			if(*(sched->head->nextp->counter)==0){
				*(sched->head->nextp->counter) = 1;
				return NULL;
			}
			if(*(tmp_task->nextp->state)== TASK_RUNNING||*(tmp_task->nextp->state) == THREAD_JOINABLE||*(tmp_task->nextp->state) == THREAD_DETACHED){
				return tmp_task->nextp;
			}else{
				tmp_task = tmp_task->nextp;			
			}
		}
	}	
}

void exit_compt(void){ /*exit*/
	kservice_unreg_compt("scheduler");
	kservice_uart_write("Clean up GPIO component!\n\r");
}


