#include <fs.h>
#include <mm.h>
#include <printf.h>
#include <stddef.h>
extern unsigned char _start_;
extern unsigned char _end;
extern int cd_rem;
extern char kdirectory[32];
struct File* fopen(char* file_name, char* type){


	/*ask opened*/

	if(!memcmp(file_name,"root/",5)){/*direct*/
		sizeof(file_name);


	}else{
		int parse_i =0,num=0;
		
		while(kdirectory[parse_i] !='\0' && kdirectory[parse_i] != (char)8){
			parse_i++;
		}
		
		char tmp_dir[32];
		memcpy(&kdirectory[0], &tmp_dir[0],parse_i);
		tmp_dir[parse_i++] = '/';
		memcpy(&file_name[0], &tmp_dir[parse_i],11);
		tmp_dir[parse_i + 12] = '\0';
		while(strcmp(tmp_dir, symbolic_fs_array[num].tmp_name)){
			num++;
			if(symbolic_fs_array[num].tmp_name[0]==NULL){
				return NULL;
			}
		}
		unsigned int clust = ((unsigned int)symbolic_fs_array[num].file_info->dir_record.ch)<<16|(unsigned int)symbolic_fs_array[num].file_info->dir_record.cl;

		if(clust){
			struct fs_unit* return_fs = fs_type_support(partition[cd_rem].type);
               		if(return_fs){
				struct mm_info tmp_page = allocate_kernel_page(4096);		
				struct File *fp = tmp_page.start;
				bpb_t *bpb=(bpb_t*)(&_end+partition[cd_rem].dbr);
				openfile* tmp_addr = bl_init( &_start_+ (unsigned int)return_fs->addr_readfile, clust, &partition[cd_rem]);
				openfile* real_addr = (&_start_ + (unsigned int)tmp_addr);		
				fp->_base = real_addr->log_addr;
				fp->_bufsize = bpb->spc*(bpb->bps0 + (bpb->bps1 << 8));
				fp->_tmpname = num;
				symbolic_fs_array[num].open = real_addr->log_addr;/*soft symbolic*/ 
				symbolic_fs_array[num].file_info->directory = real_addr->phy_addr;/*phy addr*/		
				return fp;
			}else{
				printf("Not support %x type in File system",partition[cd_rem].type);
				return NULL;
			}
			
		}
		else{
			printf("\n\rNot file");		
		}
		
	} 

	
	return NULL;
}
