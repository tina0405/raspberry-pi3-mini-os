#include "user_sys.h"
#include "user.h"
#include "printf.h"
#include "mini_uart.h"

extern unsigned long _end;
void shell_user_process() 
{	
	call_sys_write("Shell start\n\r");
	static int index = 0;
	char output[15] = {""};	
	char command[2] = {""};
	char directory[20] = {"root"};
	char file_name[11] = {""};	
	call_sys_write("tkernel@user_name:");
	call_sys_write(directory);
	call_sys_write("$");
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
				call_sys_write("\n\rtkernel@user_name:");
				call_sys_write(directory);
				call_sys_write("$");
			}
			else if(output[0]=='d'&& output[1]=='u' && output[2]=='m'&& output[3]=='p'){ /*to_do_list*/
				if(output[4]=='\r'){
					
					/*go to root*/
				}
				else if(output[4]==' '){
					for(int f = 5,n=0;f < 17;f++,n++){
					   if(output[f]=='\r'){
						break;
					   }
					   file_name[n] = output[f];
					   
		                           
					}
		    			
					call_sys_dump(file_name);
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
				}
				else
				{
					call_sys_write("\n\rNot support this command!");
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
				}
			
			}
			else if(output[0]=='c'&& output[1]=='d'){ /*to_do_list*/
			        if(output[2]=='\r'){
					
					/*go to root*/
				}
				else if(output[2]==' '){
					for(int f = 3,n=0;f < 15;f++,n++){
					   if(output[f]=='\r'){
						break;
					   }
					   file_name[n] = output[f];
					   
		                           
					}
		    			
					call_sys_cd(file_name);
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
				}
				else
				{
					call_sys_write("\n\rNot support this command!");
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
				}
			}
			else if(output[0]=='m'&& output[1]=='o' && output[2]=='d' && output[3]=='\r'){ 
				/*to_do_list*/			
				call_sys_write("\n\rmod file!");
				call_sys_write("\n\rtkernel@user_name:");
				call_sys_write(directory);
				call_sys_write("$");
			}else if(output[0] != '\r'){
                                call_sys_write("\n\rNot support this command!");
				call_sys_write("\n\rtkernel@user_name:");
				call_sys_write(directory);
				call_sys_write("$");
				
			}else{
				call_sys_write("\n\rtkernel@user_name:");
				call_sys_write(directory);
				call_sys_write("$");
			}
			index = 0;	
		}
		else
		{	
			
			call_sys_write(command);
		}
	}

}
