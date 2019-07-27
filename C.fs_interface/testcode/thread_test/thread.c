#include <stddef.h>
#include <threadlib.h>

int t=0;
thread_t thread=0;
thread_t thread_2=0;
void add(int a);
void loop(char* str);

void application(void) 
{	
	call_sys_write("User start\n\r");
	call_sys_write("Create 2 Threads\n\r");
	
        const struct thread_attr_t* attr = NULL;
	thread_create(&thread, NULL, &add, 5) ;
	thread_create(&thread_2, NULL, &add, 2) ;
	
	call_sys_write("Join Threads !\n\r");
	thread_join(thread,0);
	thread_join(thread_2,0);
		
	call_sys_write("Compare Threads !\n\r");
	if(thread_equal(thread_2,thread)){
		call_sys_write("Equel!\n\r");
  	} else {
		call_sys_write("Not equel!\n\r");
   	}		
	call_sys_write("Sum:%d\n\r",t);
}
void add(int a){
   t = t + a;
   call_sys_write("Finish to add %d!\n\r",a);	
}

