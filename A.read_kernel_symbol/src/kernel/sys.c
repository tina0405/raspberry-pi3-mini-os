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


void kservice_uart_write(char * buf){	
	printf(buf);
}

int  kservice_fork(){
	return copy_process(3, 0, 0, 0);
}

void  kservice_exit(){
	exit_process();
}

void  kservice_led_blink(void){
#ifdef GPIO
	led_blink();
#endif
}

char  kservice_uart_read(void){
	return uart_recv();
}

int  kservice_create_thread(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg){
	return _thread_create(thread, attr, start_routine, arg);
}

thread_t  kservice_thread_self(void){
	return thread_id_self();
}

int  kservice_thread_join (thread_t thread, void **value_ptr ){
	_thread_join(thread,value_ptr);
	return 0;
}

void  kservice_thread_exit (thread_t thread){
	
}

void  kservice_thread_signal(thread_t thread){
	_thread_cond_signal(thread);
}

void  kservice_list_file(void){
	list();
}

void  kservice_cd_folder(char* file_name){
	cd(file_name);
}

void  kservice_dump_file(char* file_name){
	dump(file_name);
}

void  kservice_root_file(char* file_name){
	cd_root();
}

void  kservice_run_file(char* file_name){
	run_file(file_name);
}

/*ipc_*/
int ipc_index_push =0;
void kservice_send_msg(unsigned int type, int pid, int msg){
	user_ipc_mail[ipc_index_push].dst_task = pid;/*pid?*/
	user_ipc_mail[ipc_index_push].from = current;
	user_ipc_mail[ipc_index_push].msg = msg;
	user_ipc_mail[ipc_index_push++].letter_type = type;/*0:empty*/
	if(ipc_index_push == mail_size){ipc_index_push=0;}
	accept_reply();	
}

struct mailbox kservice_recieve_msg(unsigned int ipc_type){
	recieve_msg(ipc_type);
}

void  kservice_mutex_trylock(struct thread_mutex *mutex){
	thread_mutex_trylock(mutex);
}

void  kservice_mutex_lock(struct thread_mutex *mutex){
	thread_mutex_lock(mutex);
}

void  kservice_mutex_unlock(struct thread_mutex *mutex){
	thread_mutex_unlock(mutex);
}

void  kservice_com_file(char* file_name){
	com_file(file_name);
}

unsigned long  kservice_allocate_upage(){
	return allocate_user_page(&(current->cpu_context->x19),0);
}

void kservice_free_page(unsigned long p){
	free_page(p);
}

unsigned long  kservice_allocate_kpage(){
	return allocate_kernel_page();
}

void  kservice_schedule(){
	schedule();
}

void * const sys_call_table[] = {kservice_uart_write, kservice_fork, kservice_exit, 
kservice_led_blink, 
kservice_uart_read,  /*0-4*/ 
kservice_create_thread, kservice_thread_self,kservice_thread_join,kservice_thread_exit,kservice_thread_signal,/*5-9*/
kservice_list_file,kservice_cd_folder,kservice_dump_file,kservice_root_file,kservice_run_file,kservice_send_msg,/*10-15*/
kservice_recieve_msg,kservice_mutex_trylock,kservice_mutex_lock,kservice_mutex_unlock,kservice_com_file,/*16-20*/
kservice_allocate_upage, kservice_free_page/*21-22*//*below for symbol table*/,kservice_allocate_kpage,kservice_schedule
};
