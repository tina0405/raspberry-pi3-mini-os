#include <printf.h>
#include <fs.h>
#include "fat16.h"
#include "fat32.h"
#include <sched.h>
extern unsigned char _end;
extern unsigned long mod_process;
char map_array[4096];/*improve*/
unsigned int map_index= 0;
int run_file(char* file_name){
	unsigned int clust =0;
        unsigned long base =0;
        
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,file_name,8)){
		clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;
		if(clust){

			printf("\n\r");
			base = fat32_readfile(clust, &partition[0]);
			//printf("%x\n\r",base);
			//printf("%x\n\r",&_end+(base-(unsigned int)&_end));
			unsigned long size_u = file_dir[k].size;
			memcpy(&_end+(base-(unsigned char)&_end) , &map_array[map_index],size_u);
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
