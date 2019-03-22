#ifndef	_USER_SYS_H
#define	_USER_SYS_H
typedef int __thread_t;
typedef __thread_t thread_t;




void call_sys_write(char * buf);
int call_sys_fork();
void call_sys_exit();
void call_sys_led(void);
char call_sys_read(void);
void call_sys_write_int(int a);

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
