#include <printf.h>
#include <fs.h>
#include <strcmp.h>
#include "utils.h"
char kdirectory[30] = {"root"};
int cd_rem = -1;/*remember where you are*/
extern unsigned char _end;
extern unsigned char _start_;

int cd(char* file_name){
		//char directory[20]
		/*relative*/
		int parse_i = 4,file_i =0;
		struct dev* dev_param;
		struct fs_unit* return_fs;
		if(cd_rem == -1){			
			for(int cd_i = 0; cd_i<4 ; cd_i++){
				if(!strcmp(file_name,&(sd_p[cd_i][0]))){
					return_fs = fs_type_support(partition[cd_i].type);
					if(return_fs){
						dev_param = &partition[cd_i];
						bl_init( &_start_+(return_fs->addr_directory-(unsigned int)&_start_),dev_param);
						build_root();
   	 					search_file(); 
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
			clu = bl_init( &_start_+(return_fs->addr_getcluster-(unsigned int)&_start_), file_name, dev_param);
			if(clu == 0){
				printf("\n\rNot know this file!");
				return 0;
			}else{
				// read into memory
				parse_i = 0;
				file_i =0;
				adr = bl_init( &_start_+(return_fs->addr_readfile -(unsigned int)&_start_), clu, dev_param);
				fat_listdirectory(&_end+(adr-(unsigned int)&_end));
				build_root();
				search_file();
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
