#ifndef	_USER_SYS_H
#define	_USER_SYS_H
typedef int __thread_t;
typedef __thread_t thread_t;

void call_sys_list(void);
void call_sys_write(char * buf);
void call_sys_root(void);
void call_sys_cd(char* file_name);
void call_sys_run(char* file_name);
void call_sys_dump(char* file_name);
void call_sys_root(void);
int call_sys_fork(void);
void call_sys_exit(void);
void call_sys_led(void);
char call_sys_read(void);
void call_sys_write_int(int a);
void call_sys_com(char* file_name);

int thread_create(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* b);
int thread_equal (thread_t t1, thread_t t2);
int thread_join ( thread_t thread, void **value_ptr);
thread_t sys_thread_self(void);

struct thread_attr_t {
	int a;
	int b;
};




extern void user_delay ( unsigned long);
//extern unsigned long get_sp ( void );
//extern unsigned long get_pc ( void );

#endif  /*_USER_SYS_H */
