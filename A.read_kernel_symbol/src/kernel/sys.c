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


void kernel_sevice_write(char * buf){	
	printf(buf);
}

int  kernel_sevice_fork(){
	return copy_process(3, 0, 0, 0);
}

void  kernel_sevice_exit(){
	exit_process();
}

void  kernel_sevice_led(void){
	led_blink();
}

char  kernel_sevice_read(void){
	return uart_recv();
}

int  kernel_sevice_create_thread(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg){
	return _thread_create(thread, attr, start_routine, arg);
}

thread_t  kernel_sevice_thread_self(void){
	return thread_id_self();
}

int  kernel_sevice_thread_join (thread_t thread, void **value_ptr ){
	_thread_join(thread,value_ptr);
	return 0;
}

void  kernel_sevice_thread_exit (thread_t thread){
	
}

void  kernel_sevice_thread_signal(thread_t thread){
	_thread_cond_signal(thread);
}

void  kernel_sevice_list_file(void){
	list();
}

void  kernel_sevice_cd_folder(char* file_name){
	cd(file_name);
}

void  kernel_sevice_dump_file(char* file_name){
	dump(file_name);
}

void  kernel_sevice_root_file(char* file_name){
	cd_root();
}

void  kernel_sevice_com_file(char* file_name){
	com_file(file_name);
}

void  kernel_sevice_run_file(char* file_name){
	run_file(file_name);
}


void  kernel_sevice_mutex_trylock(struct thread_mutex *mutex){
	thread_mutex_trylock(mutex);
}

void  kernel_sevice_mutex_lock(struct thread_mutex *mutex){
	thread_mutex_lock(mutex);
}

void  kernel_sevice_mutex_unlock(struct thread_mutex *mutex){
	thread_mutex_unlock(mutex);
}


/*ipc_*/
int ipc_index_push =0;
void kernel_sevice_send_msg(unsigned int type, int pid, int msg){
	user_ipc_mail[ipc_index_push].dst_task = pid;/*pid?*/
	user_ipc_mail[ipc_index_push].from = current;
	user_ipc_mail[ipc_index_push].msg = msg;
	user_ipc_mail[ipc_index_push++].letter_type = type;/*0:empty*/
	if(ipc_index_push == mail_size){ipc_index_push=0;}
	accept_reply();	
}

struct mailbox kernel_sevice_recieve_msg(unsigned int ipc_type){
	recieve_msg(ipc_type);
}
void * const sys_call_table[] = {kernel_sevice_write, kernel_sevice_fork, kernel_sevice_exit, kernel_sevice_led, kernel_sevice_read,  /*0-4*/ 
				 kernel_sevice_create_thread, kernel_sevice_thread_self,kernel_sevice_thread_join,kernel_sevice_thread_exit,kernel_sevice_thread_signal,/*5-9*/
				 kernel_sevice_list_file,kernel_sevice_cd_folder,kernel_sevice_dump_file,kernel_sevice_root_file,kernel_sevice_run_file,kernel_sevice_send_msg,/*10-15*/
				kernel_sevice_recieve_msg,kernel_sevice_mutex_trylock,kernel_sevice_mutex_lock,kernel_sevice_mutex_unlock,kernel_sevice_com_file};/*16-20*/
