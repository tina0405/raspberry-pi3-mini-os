#ifndef	_USER_SYS_H
#define	_USER_SYS_H

void call_sys_write(char * buf);
int call_sys_fork();
void call_sys_exit();
void call_sys_led(void);
char call_sys_read(void);
void sys_write_int(int a);
int create_thread(struct task_struct *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* b);

struct thread_attr_t {
	int a;
	int b;
};

extern void user_delay ( unsigned long);
//extern unsigned long get_sp ( void );
//extern unsigned long get_pc ( void );

#endif  /*_USER_SYS_H */
