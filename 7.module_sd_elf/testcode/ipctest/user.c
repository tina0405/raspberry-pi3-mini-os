#include <stddef.h>
#define Rendezvous 1
typedef int __thread_t;
typedef __thread_t thread_t;


void user_ipc_test() 
{
	
		call_sys_write("Send a msg from user application\n\r");
		call_send_msg(Rendezvous, 2, 3);
		call_sys_write("Test is over!\n\r");
	while(1){}
}

