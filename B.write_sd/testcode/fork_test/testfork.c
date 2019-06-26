#include <stddef.h>

void data(char* str,int a);
int t = 0;
void application(void) 
{	
	call_sys_write("Fork test start!\n\r");
	call_sys_write("Fork\n\r");
        int pid = call_sys_fork();
	
        if (pid < 0) {
		call_sys_write("Error during fork\n\r");
		int a =0;
		call_sys_exit(a);

	}


	if (pid == 0){
		data("abcde",2);
	}else{
		data("12345",5);
	}
	call_sys_write("\n\rt is %d\n\r",t);
	
}

void data(char* str,int a)
{
	t=t+a;
	char buf[2] = {""};	
	for (int i = 0; i < 5; i++){
		buf[0] = str[i];
		call_sys_write(buf);
		user_delay(1000000);
	}
}

