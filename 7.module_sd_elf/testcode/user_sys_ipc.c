void call_sys_send(unsigned int type, int pid, int msg);
void call_send_msg(unsigned int type, int pid, int msg){
	//mutex_lock;
	call_sys_send(type, pid, msg);
	//mutex_unlock;
}
