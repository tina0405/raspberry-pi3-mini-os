#ifndef	_SYS_H
#define	_SYS_H

#define __NR_syscalls	 23

#ifndef __ASSEMBLER__
 /*pthreadtypes.h*/

void sys_write(char * buf);
int sys_fork();
void sys_write_int(int a);
void sys_exit();
void sys_led(void);
char sys_read(void);
//int sys_create_thread(struct task_struct *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* b);

#endif

#endif  /*_SYS_H */
