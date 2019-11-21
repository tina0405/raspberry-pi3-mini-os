#ifndef	_SYS_H
#define	_SYS_H

#define __NR_syscalls	 32

#ifndef __ASSEMBLER__
 /*pthreadtypes.h*/
#include "sched.h"

extern void kservice_uart_write(char *fmt, ...);
extern int  kservice_fork(void);
extern void kservice_exit(void *status);
extern void kservice_led_blink(void);
extern char kservice_uart_read(void);


#endif

#endif  /*_SYS_H */
