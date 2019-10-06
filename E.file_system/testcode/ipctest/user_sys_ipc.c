#include <mutex.h>
void call_send_msg(unsigned int type, int pid, int msg){
	unsigned int tmp, tmp2;
	//thread_mutex send_lock;
	int send_lock = 0;
	int var = 1;


	call_sys_write_int(tmp);
	call_sys_send(type, pid, msg);
	//mutex_unlock;
}
