#include "user_sys.h"
#include "user.h"
#include "printf.h"
#include "sched.h"
//#include "terminal_cmd.h"
#include "mini_uart.h"
#include "fs.h"
extern unsigned long _end;
char directory[30] = {"root"};
void invalid(void){
	call_sys_write("\n\rNot support this command!\n\rtkernel@user_name:%s$",directory);
}
void shell_user_process() 
{	
	call_sys_write("Shell start\n\r");
	static int index = 0;
	char output[40] = {""};	
	char command[2] = {""};
	
	char file_name[40] = {""};	
	call_sys_write("tkernel@user_name:");
	call_sys_write("%s",directory);
	call_sys_write("$");
	for(int nope = 0;nope<8;nope++){
		call_sys_read();
	};
	/*Nope*/

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
					call_sys_write("\n\rWithout input file!");
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
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
				else{invalid();}
			
			}
			else if(output[0]=='c'&& output[1]=='d'){ /*to_do_list*/
			        if(output[2]=='\r' || (output[2]==' ' && output[3]=='r' && output[4]=='o' && output[5]=='o' && output[6]=='t' && output[7]=='\r') ){
					call_sys_root();
					/*go to root*/
					/*here clear directory*/
					directory[0] = 'r';directory[1] = 'o';directory[2] = 'o';directory[3] = 't';directory[4] = '\0';
					for(int cdr=5;cdr<30;cdr++){ directory[cdr] = '\0'; }
					
				}
				else if(output[2]==' '){
					for(int f = 3,n=0;f < 15;f++,n++){
					   if(output[f]=='\r'){
						file_name[n] = '\0';
						break;
					   }
					   file_name[n] = output[f];
					}
		    			int parse_i = 0,file_i =0;
					if(call_sys_cd(file_name)){
						while(directory[parse_i] !='\0' && directory[parse_i] != (char)8){
							parse_i++;
						}
                                                directory[parse_i++] = '/';
						while(file_name[file_i]!='\0'&& file_name[file_i]!=' '){
							directory[parse_i+file_i] = file_name[file_i];
							file_i++;
						}
						file_name[file_i] = '\0';
					}
		
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
				}
				else{invalid();}
			}
			else if(output[0]=='r'&& output[1]=='u' && output[2]=='n'){ 
				/*to_do_list*/			
				if(output[3]=='\r'){
					call_sys_write("\n\rWithout input file!");
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
					
				}
				else if(output[3]==' '){
					for(int f = 4,n=0;f < 14;f++,n++){
					   if(output[f]=='\r'){
						break;
					   }
					   file_name[n] = output[f];
					   
		                           
					}
		    			
					call_sys_run(file_name);
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
				}
				else
				{
					invalid();
				}

			}
			else if(output[0]=='s'&& output[1]=='w' && output[2]=='a' && output[3]=='p'){/*swap*/
				/*to_do_list*/			
				if(output[4]=='\r'){
					call_sys_write("\n\rWithout input file!");
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
					
				}
				else if(output[4]==' '){
					for(int f = 5,n=0;f < 44 ;f++,n++){
					   if(output[f]=='\r'){
						break;
					   }
					   file_name[n] = output[f];
					   
		                           
					}
		    			
					call_sys_swap(file_name);
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
				}
				else
				{
					invalid();
				}

			}
			else if(output[0]=='l'&& output[1]=='s' && output[2]=='c' && output[3]=='o' && output[4]=='m'){ 
				if(output[5]=='\r'){
					call_sys_lscom();
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
					
				}
				else{invalid();}				
			}
			else if(output[0]=='l'&& output[1]=='s' && output[2]=='d' && output[3]=='e' && output[4]=='v'){ 
				if(output[5]=='\r'){
					call_sys_lsdev();
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
					
				}
				else{invalid();}			
			}
			else if(output[0]=='i'&& output[1]=='n' && output[2]=='c' && output[3]=='o' && output[4]=='m'){ 
			
				if(output[5]=='\r'){
					call_sys_write("\n\rWithout input file!");
					call_sys_write("\n\rtkernel@user_name:");
					
				}
				else if(output[5]==' '){
					for(int f = 6,n=0;f < 20;f++,n++){
					   if(output[f]=='\r'){
						break;
					   }
					   file_name[n] = output[f];		                           
					}
					call_sys_com(file_name);				
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
				}
				else{invalid();}
			}else if(output[0]=='r'&& output[1]=='m' && output[2]=='c' && output[3]=='o' && output[4]=='m'){ 
				/*to_do_list*/			
				if(output[5]=='\r'){
					call_sys_write("\n\rWithout input file!");
					call_sys_write("\n\rtkernel@user_name:");
					
				}
				else if(output[5]==' '){
					for(int f = 6,n=0;f < 45;f++,n++){
					   if(output[f]=='\r'){
						break;
					   }
					   file_name[n] = output[f];
					   
		                           
					}
		    			
					call_sys_rmcom(file_name);
					call_sys_write("\n\rtkernel@user_name:");
					call_sys_write(directory);
					call_sys_write("$");
				}
				else{invalid();}
			}else if(output[0]=='m'&& output[1]=='o' && output[2]=='u' && output[3]=='n' && output[4]=='t' && output[5]==' ' && output[6]=='s' && output[7]=='d'){ 
					call_sys_mount();
			}
			else if(output[0] != '\r'){
                                invalid();
				
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


