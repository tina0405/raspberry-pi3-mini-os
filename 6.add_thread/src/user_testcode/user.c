#include "user_sys.h"
#include "user.h"
#include "printf.h"
#include "mini_uart.h"
#include "utils.h"
#include "sched.h"
#include "sys.h"
#include <stddef.h>
int t=0;
thread_t thread=0;
thread_t thread_2=0;
void loop(char* str)
{
	char buf[2] = {""};
	while (1){

		for (int i = 0; i < 5; i++){
			buf[0] = str[i];
			call_sys_write(buf);
			call_sys_write_int(t);
			user_delay(1000000);
		}
	}
}

void add(int a)
{
   call_sys_write("I'm a thread, ID is ");
   call_sys_write_int(thread_self());
   call_sys_write("\n\r");
   t = t + a;
   call_sys_write("Finish to add 1!\n\r");	
}

void user_process() 
{	
	call_sys_write("User start\n\r");
#ifdef SHELL
	char command[2] = {""};
	call_sys_write("Shell start\n\r");
	call_sys_write("tkernel@user_name:$");
	
	while(1){
	
		command[0] = call_sys_read();
		if(command[0] == '\r'){	
			call_sys_write("\n\rtkernel@user_name:$");
		}
		else
		{
			call_sys_write(command);
		}
	}
#endif
	call_sys_write_int(t);
	call_sys_write("Create Thread\n\r");
	
	//const struct thread_attr_t* attr = NULL;
	thread_create(&thread, NULL, &add, 5) ;

        //call_sys_led();
	call_sys_write("Fork\n\r");
        int pid = call_sys_fork();
	
        if (pid < 0) {
		call_sys_write("Error during fork\n\r");
		call_sys_exit();
		return;
	}

		
	if (pid == 0){

		thread_create(&thread_2, NULL, &add, 2) ;
		//call_sys_write("JOIN!\n\r");
		//thread_join(thread_2,5);
		
		if(thread_equal(thread_2,thread)){
			call_sys_write("Equel!\n\r");
  		} else {
			call_sys_write("Not equel!\n\r");
   		}		
		
		loop("abcde");
		
	} else {

		loop("12345");
	}
	
	
}

