#ifndef	_SYS_H
#define	_SYS_H

#define __NR_syscalls	 26

#ifndef __ASSEMBLER__
 /*pthreadtypes.h*/
#include "sched.h"

void sys_write(char * buf);
int sys_fork();
void sys_write_int(int a);
void sys_exit();
void sys_led(void);
char sys_read(void);


#endif

#endif  /*_SYS_H */
