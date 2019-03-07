#include <stddef.h>
#include "fork.h"
#include "printf.h"
#include "utils.h"
#include "sched.h"
#include "mm.h"
#include "led_blink.h"
#include "mini_uart.h"
#include "user_sys.h"


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

int sys_create_thread(struct task_struct *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* b){
	return add_thread(start_routine,b);

}

void * const sys_call_table[] = {sys_write, sys_fork, sys_exit, sys_led, sys_read, sys_write_int,  sys_create_thread};
