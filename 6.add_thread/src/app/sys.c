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

int sys_create_thread(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg){
	return add_thread(thread, attr, start_routine, arg);
}

thread_t sys_thread_self(void){
	return thread_id_self();
}

void * const sys_call_table[] = {sys_write, sys_fork, sys_exit, sys_led, sys_read, sys_write_int,  sys_create_thread, sys_thread_self};
