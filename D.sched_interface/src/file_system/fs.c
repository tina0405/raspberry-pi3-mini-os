#include <printf.h>
#include <stddef.h>
#include <stdint.h>
#include "fat.h"
#include "fs.h"
#include "pm.h"
#include <mm.h>
#include "sched.h"
#include "utils.h"
#include "fat.h"
#include "fat16.h"
#include "fat32.h"
#include "mini_uart.h"
extern unsigned char _end;
extern fatdir_t *dir;
extern char directory[20];
fatdir_t *origin;
extern unsigned char _start_;

struct fs_unit* fs_type_support(int type){
	for(int type_index=0; fs_support[type_index].type != 0 && type_index<32;type_index++){
		if(fs_support[type_index].type == type){
			return &fs_support[type_index];}
	}
	return (struct fs_unit*)0;
}


void build_kernel_directory(void){
    char* tmp;
    char* pa_name; 
    int pa_in = 0;
    bpb_t *bpb;
    unsigned int s = 0;
    unsigned char* ptr;
    struct dev* dev_param;
    struct fs_unit* return_fs;
    struct mm_info memory;
    memzero(&(sd_p[0][0]),44);
    for(int pnum=0; pnum<4; pnum++){
	
	 return_fs = fs_type_support(partition[pnum].type);
	 if(return_fs){
		
                /*malloc*/
		bpb=(bpb_t*)(&_end + partition[pnum].record);
		dev_param = &partition[pnum];	
		/*save the FAT table*/
		switch(partition[pnum].type){
			case 0xc:
				memory = allocate_kernel_page((bpb->spf32+bpb->rsc)*512);
				s = kservice_dev_read(1, partition[pnum].partitionlba+1,(unsigned char*)(memory.start),bpb->spf32+bpb->rsc);
				/*clean empty[16]*/
				unsigned int *fat32 = (unsigned int*)(memory.start - 512 + bpb->rsc*512);
				for(int i = 0, k = 0; i < (bpb->spf32 - bpb->rsc + 1)*(512/32) ;i++){
					if(fat32[i]==0){
						partition[pnum].empty[k++] = i;
						if(k == 16){break;}	
					}
				}
				break;
#ifdef FAT16
			case 0xe:
				memory = allocate_kernel_page((bpb->spf16+bpb->rsc)*512);
				s = kservice_dev_read(1, partition[pnum].partitionlba+1,(unsigned char*)memory.start,bpb->spf16+bpb->rsc);
				/*clean empty[16]*/				
				unsigned short *fat16 = (unsigned short*)(memory.start - 512 + bpb->rsc*512);
				for(int a = 0, b = 0; a < (bpb->spf16 - bpb->rsc + 1)*(512/16);a++){
					if(fat16[a]==0){
						partition[pnum].empty[b++] = a;
						if(b==16){break;}	
					}
		
				}		
				break;
#endif	
		}
		
                partition[pnum].fat_table_start = memory.start;
		bl_init( &_start_+(return_fs->addr_directory-(unsigned int)&_start_),dev_param);		
		build_root();
   	 	pa_name =  search_file(); 
		pa_in = 0;
		/*scanf empty cluster*/
		//scanf_empty_cluster(partition[pnum].type,(unsigned char*)(memory.start));	
		if(pnum == 0){
			read_ksymbol();
		}
		while(*(pa_name+pa_in)!=(char)8 && *(pa_name+pa_in)!='\0' && *(pa_name+pa_in)!= (char)32){
			sd_p[pnum][pa_in] = *(pa_name+pa_in);
			pa_in++;
		}
		sd_p[pnum][pa_in] = '\0';
	 }
    }
    

    memzero(file_dir,sizeof(struct user_fs)*20);/*file_dir index */
    for(int build_d = 0,file_dir_i = 0; build_d<4; build_d++){
	 if(sd_p[build_d][0] != (char)8 && sd_p[build_d][0] != '\0'){
		memcpy((&sd_p[build_d][0]),&(file_dir[file_dir_i].name[0]), 8);
	 	file_dir[build_d].attr[0] = (char) 16;	
		file_dir_i++;
	 }
    }

    /*save FAT table*/

   
    // end of FAT in memory
    //memcpy(fat32_read_directory(partition[0]), (&sd_p[0][0]), 11);
    //printf("A:%s\n\r",&sd_p[0][0]);
}



void build_root(void){
	origin = dir;
}

char* search_file(void){
        int index = 0;
        memzero(file_dir,sizeof(struct user_fs)*20);
	
        for(;origin->name[0]!=0;origin++) {
		// is it a valid entry?
		if(origin->name[0]==0xE5 || origin->attr[0]==0xF) continue;
		// decode attributes
		//printf("%s\n\r",origin -> name);
		memcpy(origin -> name,file_dir[index].name,11);	
                memcpy(origin -> attr,file_dir[index].attr,9);	
		file_dir[index].size = origin -> size;
		file_dir[index].ch = origin -> ch;		
		file_dir[index].cl = origin -> cl;
		index++;
		
	}
	return file_dir[0].name;

}






