#include <fs.h>
#include <mm.h>
#include <printf.h>
#include <stddef.h>
extern unsigned char _start_;
extern unsigned char _end;
extern int cd_rem;
struct File* fopen(char* file_name, char* type){
	 



	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	   if(!memcmp(file_dir[k].name,file_name,8)){
		
		/*get cluster for*/
		unsigned int clust = ((unsigned int)file_dir[k].ch)<<16|(unsigned int)file_dir[k].cl;
		if(clust){
			
                        printf("\n\r");
			struct fs_unit* return_fs = fs_type_support(partition[cd_rem].type);
               		if(return_fs){
				struct mm_info page = allocate_kernel_page(file_dir[k].size);
				struct File* fp = (struct File*)page.start; 
				bpb_t *bpb=(bpb_t*)(&_end+partition[cd_rem].dbr);				
				fp->_base = bl_init( &_start_+(return_fs->addr_readfile -(unsigned int)&_start_), clust, &partition[cd_rem]);
				fp->_bufsize = bpb->spc*(bpb->bps0 + (bpb->bps1 << 8));/*clust size*/
				bl_init( &_start_+(return_fs->addr_getpos -(unsigned int)&_start_), clust, &partition
[cd_rem],fp);
				//memcpy(&(tmp_pos->real_addr),&(fp->_real.real_addr),sizeof(struct pos));	

				printf("cluster %x %x!!\n\r",fp->_real.next_cluster,fp->_real.real_addr);		
				return fp;
			}else{
				printf("Not support %x type in File system",partition[cd_rem].type);
				return NULL;
			}
			
		}
		else{
			printf("\n\rNot file");		
		}
		return NULL;
	   }
	   

        }
	printf("\n\rNot file");
	return NULL;
}
