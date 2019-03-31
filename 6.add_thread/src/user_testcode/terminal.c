#include "user_sys.h"
#include "user.h"
#include "printf.h"
#include "mini_uart.h"

extern unsigned long _end;
void shell_user_process() 
{	
	call_sys_write("Shell start\n\r");
	static int index = 0;
	char output[10] = {""};	
	char command[2] = {""};
	call_sys_write("tkernel@user_name:$");
	for(int nope = 0;nope<8;nope++){
		call_sys_read();
	};
	/*Nope*/

	unsigned long a = &_end;
	while(1){
		
		command[0] = call_sys_read();		
		output[index++] = command[0];
                /* index */
		/*over 10*/
                if(command[0] == '\r'){ /*enter*/	
			
		
			if(output[0]=='l'&& output[1]=='s' && output[2]=='\r'){ /*to_do_list*/	
				call_sys_list();				
				call_sys_write("\n\rtkernel@user_name:$");
			}
			else if(output[0]=='c'&& output[1]=='a' && output[2]=='t' && output[3]=='\r'){ 
				/*to_do_list*/
				call_sys_write("\n\rcat file!"); 
				call_sys_write("\n\rtkernel@user_name:$");
			}
			else if(output[0]=='c'&& output[1]=='d' && output[2]=='\r'){ /*to_do_list*/
				call_sys_write("\n\rcd file!");
				call_sys_write("\n\rtkernel@user_name:$");
			}
			else if(output[0]=='m'&& output[1]=='o' && output[2]=='d' && output[3]=='\r'){ 
				/*to_do_list*/			
				call_sys_write("\n\rmod file!");
				call_sys_write("\n\rtkernel@user_name:$");
			}else if(output[0] != '\r'){
                                call_sys_write("\n\rNot support this command!");
				call_sys_write("\n\rtkernel@user_name:$");
				
			}else{
				call_sys_write("\n\rtkernel@user_name:$");
			}
			index = 0;	
		}
		else
		{	
			
			call_sys_write(command);
		}
	}

}
