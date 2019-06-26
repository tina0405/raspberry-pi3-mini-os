extern int application(void);
void sys_need(void){
	int ret = 0;	
	ret = application();
	call_sys_exit(ret);
}
