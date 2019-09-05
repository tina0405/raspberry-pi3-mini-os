#include <printf.h>
#include <fs.h>
#include "fat16.h"
#include "fat32.h"
#include <sched.h>
extern unsigned char _end;
extern unsigned long mod_process;
char map_array[4096];/*improve*/
unsigned int map_index= 0;
extern int cd_rem;
int run_file(char* file_name){
	unsigned int clust =0;
        unsigned long base =0;
        
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,file_name,8)){
		clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;
		if(clust){

			printf("\n\r");
			switch(partition[cd_rem].type){
				case 0xc:
					base = fat32_readfile(0, clust, &partition[cd_rem]);
					break;
#ifdef FAT16
				case 0xe:
					base = fat16_readfile(0, clust, &partition[cd_rem]);
					break;
#endif
				default:
					printf("Not support %x type in File system",partition[cd_rem].type);
					return 0;			
			}
			unsigned long size_u = file_dir[k].size;
			memcpy((char *)base , &map_array[map_index],size_u);
			copy_process(APP_THREAD, (unsigned long)&mod_process, &map_array[map_index], size_u);
			map_index = map_index + size_u;
			printf("User application: read file OK!\n\r");
			
		}
		else{
			printf("\n\rNot file");	
			return 0;	
		}
		return 1;
	   }
	   

        }
	printf("\n\rNot file");
	return 0;

}
