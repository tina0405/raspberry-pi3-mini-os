#include <printf.h>
#include <fs.h>
#include <strcmp.h>
#include "utils.h"
char kdirectory[30] = {"root"};
int cd_rem = -1;/*remember where you are*/
extern unsigned char _end;
extern unsigned char _start_;
unsigned int current_page=0;
int cd(char* file_name){
		//char directory[20]
		/*relative*/
		int parse_i = 4,file_i =0;
		struct dev* dev_param;
		struct fs_unit* return_fs;
		if(cd_rem == -1){/*root*/			
			for(int cd_i = 0; cd_i<4 ; cd_i++){
				if(!strcmp(file_name,&(sd_p[cd_i][0]))){
					return_fs = fs_type_support(partition[cd_i].type);
					if(return_fs){
						dev_param = &partition[cd_i];
						current_page = dev_param->op_dir;
   	 					user_dir((char*)dev_param->op_dir); 
						while(file_name[file_i]!='\0'){
							kdirectory[parse_i+file_i] = file_name[file_i];
							file_i++;
						}
						cd_rem = cd_i;
						return 1;
					}else{  return 0;}
				}
			}
			return 0;
		}
		unsigned int clu = 0, adr = 0;
                return_fs = fs_type_support(partition[cd_rem].type);
                if(return_fs){
			dev_param = &partition[cd_rem];
			int succe = find_folder((char*)dev_param->op_dir,file_name);
			if(succe==1){
				parse_i = 0;
				file_i =0;
				while(kdirectory[parse_i] !='\0' && kdirectory[parse_i] != (char)8){
					parse_i++;
				}
                                kdirectory[parse_i++] = '/';
				while(file_name[file_i]!='\0'){
					kdirectory[parse_i+file_i] = file_name[file_i];
					file_i++;
				}
				//printf("dir:%s\n\r",kdirectory);
				return 1;

			}else{
				return 0;
			}
	
		}else{  
			printf("Not support %x type in File system",partition[cd_rem].type);
			return 0;
		}
				
}

void cd_root(void){
		
		 memzero(file_dir,sizeof(struct user_fs)*20);
   		 for(int build_d = 0; build_d<4 && sd_p[build_d][0] != (char)8; build_d++){
       	 	 	memcpy((&sd_p[build_d][0]),&(file_dir[build_d].name[0]), 8);
	 		file_dir[build_d].attr[0] = (char) 16;
    		 }
		 memzero(&kdirectory[0],30);
		 memcpy("root",&kdirectory[0],4);
		 cd_rem = -1;

}


int find_folder(char* page,char* file_name){
	int index = 0;
        memzero(file_dir,sizeof(struct user_fs)*20);
	struct file* file = (struct file*)(&_start_ + (unsigned int)page);
	//data_dump((unsigned int*)(origin),256);
        for(;file->dir_record.name[0]!=0;file++) {
		// is it a valid entry?
		if(file->dir_record.name[0]==0xE5 || file->dir_record.attr[0]==0xF) continue;
		// decode attributes
		if(!memcmp(file->dir_record.name,file_name,8)){
			if(file->dir_record.attr[0]&16){
				current_page = file->directory;
				user_dir((char*)current_page);
				return 1;
			}else{
				printf("Not a directory:%s\n\r",file_name);
				return 2;
			}
		}
		
		index++;
		
	}
	printf("\n\runknown:%s\n\r",file_name);
	return 0;
        
}
