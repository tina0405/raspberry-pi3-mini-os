#ifndef	_USER_SYS_H
#define	_USER_SYS_H

void call_sys_list(void);
void call_sys_write(char *fmt, ...);
void call_sys_root(void);
void call_sys_cd(char* file_name);
void call_sys_run(char* file_name);
void call_sys_dump(char* file_name);
void call_sys_root(void);
int call_sys_fork(void);
void call_sys_exit(void *status);
void call_sys_led(void);
char call_sys_read(void);
void call_sys_write_int(int a);
void call_sys_com(char* file_name);
void call_sys_rmcom(char* compt_name);
void call_sys_lscom(void);
void call_sys_lsdev(void);
void call_sys_debug(void);
extern void user_delay ( unsigned long);
//extern unsigned long get_sp ( void );
//extern unsigned long get_pc ( void );

#endif  /*_USER_SYS_H */
