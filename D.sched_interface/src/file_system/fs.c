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
		bpb=(bpb_t*)(&_end + partition[pnum].dbr);
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
		/*here*/
		openfile* addr = bl_init( &_start_+(return_fs->addr_directory-(unsigned int)&_start_),dev_param);
	        //data_dump((char *)(&_start_ + (unsigned int)addr->log_addr),512);
		partition[pnum].directory_addr.log_addr = addr->log_addr;
		partition[pnum].directory_addr.phy_addr = addr->phy_addr;		
		//build_root();
   	 	pa_name =  _search_file(&partition[pnum].directory_addr,partition[pnum].op_dir,partition[pnum]); /*here*/
		pa_in = 0;
		/*scanf empty cluster*/
		//scanf_empty_cluster(partition[pnum].type,(unsigned char*)(memory.start));	
		if(pnum == 0){
			user_dir(partition[0].op_dir);
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
extern int sect;
char* _search_file(openfile* addr, char* page, struct dev dev_num){
        int index = 0;
	int parse_i = 4,file_i =0;
	//printf("save_dir_head:%x %x %x\n\r",(unsigned int)addr->log_addr,addr,page);
	struct file* file = (struct file*)(&_start_ + (unsigned int)page);
	fatdir_t * tmp_origin= &_start_ + (unsigned int)addr->log_addr;/*global!!!*/
	//data_dump((char *)(&_start_ + (unsigned int)addr.log_addr),256);
	struct fs_unit* return_fs = fs_type_support(dev_num.type);
	if(return_fs){
		for(int offset = 0; tmp_origin->name[0]!=0;tmp_origin++, offset++) {
			// is it a valid entry?
			if(tmp_origin->name[0]==0xE5 || tmp_origin->attr[0]==0xF ||tmp_origin->name[0]==0x2E|| !memcmp("TRASH-~1",tmp_origin -> name,8)) continue;
			// decode attributes
			memcpy(tmp_origin -> name, (file+index)->dir_record.name,11);	
		        memcpy(tmp_origin -> attr, (file+index)->dir_record.attr,9);	
			
			(file+index)->dir_record.size = tmp_origin -> size;
			(file+index)->dir_record.ch = tmp_origin -> ch;		
			(file+index)->dir_record.cl = tmp_origin -> cl;
			unsigned int clu = ((unsigned int)tmp_origin -> ch)<<16|tmp_origin -> cl;
		
		 
			if((file+index)->dir_record.attr[0]&16){/*directory*/
				if(clu == 0){
					printf("Not know this file!\n\r");
				}else{
					// read into memory
					parse_i = 0;
					file_i =0;
					int a = bl_init( &_start_+(return_fs->addr_getcluster-(unsigned int)&_start_), (file+index)->dir_record.name, dev_num);
					
					openfile* save_dir = bl_init( &_start_+(return_fs->addr_readfile -(unsigned int)&_start_), clu, dev_num);				/*new page*/
 					openfile tmp_dir;
					tmp_dir.phy_addr = save_dir->phy_addr;
					tmp_dir.log_addr = save_dir->log_addr;
			
					struct mm_info dir_page =  allocate_kernel_page(4096);					
					(file+index)->num_fatdir= offset;/*offset*/
					(file+index)->directory = dir_page.start;/*next page*/
					(file+index)->addr.phy_addr = addr->phy_addr;/*phy dir start*/
					(file+index)->addr.log_addr = addr->log_addr;/*log dir start*/	
					_search_file(&tmp_dir, dir_page.start,dev_num);				
					
					//fat_listdirectory(&_end+(adr-(unsigned int)&_end));
					
				}	
			}else{
				if(clu == 0){
					printf("Not know this file!\n\r");
				}else{
					// read into memory
					parse_i = 0;
					file_i =0;
					
					(file+index)->num_fatdir= offset;/*offset*/
					(file+index)->directory = NULL;
					(file+index)->addr.phy_addr = addr->phy_addr;/*phy dir start*/
					(file+index)->addr.log_addr = addr->log_addr;/*log dir start*/
				}	


			}
		
			index++;
			
		
		}
        }else{
		return NULL;
	}
	return file->dir_record.name;

}


void user_dir(char* page){
        int index = 0;
        memzero(file_dir,sizeof(struct user_fs)*20);
	struct file* file = (struct file*)(&_start_ + (unsigned int)page);
	//data_dump((unsigned int*)(origin),256);
        for(;file->dir_record.name[0]!=0;file++) {
		// is it a valid entry?
		if(file->dir_record.name[0]==0xE5 || file->dir_record.attr[0]==0xF) continue;
		// decode attributes

		memcpy(file->dir_record.name,file_dir[index].name,11);	
                memcpy(file->dir_record.attr,file_dir[index].attr,9);	
		file_dir[index].size = file->dir_record.size;
		file_dir[index].ch = file->dir_record.ch;		
		file_dir[index].cl = file->dir_record.cl;
		index++;
		
	}
        

}



