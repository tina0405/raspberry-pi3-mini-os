extern void application(void);
void sys_need(void){
	int a = 0;	
	application();
	call_sys_exit(a);
}
