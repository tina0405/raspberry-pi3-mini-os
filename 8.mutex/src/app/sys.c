#include "fork.h"
#include "printf.h"
#include "utils.h"
#include "sched.h"
#include "mm.h"
#include "mini_uart.h"
#include "led_blink.h"
#include "user_sys.h"
#include "sys.h"
#include "fs.h"
extern struct mailbox user_ipc_mail[mail_size];
int add_thread(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg);
thread_t thread_id_self(void);
int _thread_join (thread_t thread, void **status);
void signal(thread_t thread);


extern unsigned long user_page_start;


void sys_write(char * buf){	
	printf(buf);
}

void sys_write_int(int a){	
	pr_int(a);
}

int sys_fork(){
	return copy_process(3, 0, 0, 0);
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

int sys_thread_join (thread_t thread, void **value_ptr ){
	_thread_join(thread,value_ptr);
	return 0;
}

void sys_thread_exit (thread_t thread){
	
}

void sys_thread_signal(thread_t thread){
	signal(thread);
}

void sys_list_file(void){
	list();
}

void sys_cd_folder(char* file_name){
	cd(file_name);
}

void sys_dump_file(char* file_name){
	dump(file_name);
}

void sys_root_file(char* file_name){
	cd_root();
}

void sys_mod_file(char* file_name){
	mod_file(file_name);
}
/*ipc_*/
int ipc_index_push =0;
void sys_send_msg(unsigned int type, int pid, int msg){
	user_ipc_mail[ipc_index_push].dst_task = pid;/*pid?*/
	user_ipc_mail[ipc_index_push].from = current;
	user_ipc_mail[ipc_index_push].msg = msg;
	user_ipc_mail[ipc_index_push++].letter_type = type;/*0:empty*/
	if(ipc_index_push == mail_size){ipc_index_push=0;}
	accept_reply();	
}

struct mailbox sys_recieve_msg(unsigned int ipc_type){
	recieve_msg(ipc_type);
}
void * const sys_call_table[] = {sys_write, sys_fork, sys_exit, sys_led, sys_read, sys_write_int, /*0-5*/ 
				 sys_create_thread, sys_thread_self,sys_thread_join,sys_thread_exit,sys_thread_signal,/*6-10*/
				 sys_list_file,sys_cd_folder,sys_dump_file,sys_root_file,sys_mod_file,sys_send_msg,/*11-16*/
				sys_recieve_msg};
