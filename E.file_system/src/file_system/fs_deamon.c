#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>
#include <fs.h>
#include "fat16.h"
#include "fat32.h"
#include <stddef.h>

struct mailbox fs_mail[mail_size]={NULL};
static int index_pop = 0;
//int support_type[4] = {0}; /*0:fat16*/
extern unsigned char _start_;
extern int cd_rem;
void fs_daemon(void)
{	
	//printf("File System send a message (int 3) to ipc_test (use pid as an address)\n\r");
	//send_msg(Rendezvous, 2, 3);

	
	fs_support[0].type = 0x0c;
	fs_support[0].addr_directory = &fat32_read_directory;
 	fs_support[0].addr_getcluster = &fat32_getcluster;
 	fs_support[0].addr_readfile = &fat32_readfile;
	fs_support[0].addr_readbuf = &fat32_readbuf;
	fs_support[0].addr_writebuf = &fat32_writebuf;
	fs_support[0].addr_writedir = &fat32_writedir;
#ifdef FAT16
	//register fat16
	fs_support[1].type = 0x0e;
	fs_support[1].addr_directory = &fat16_read_directory;
 	fs_support[1].addr_getcluster = &fat16_getcluster;
	fs_support[1].addr_readfile = &fat16_readfile;
 	fs_support[1].addr_readbuf = &fat16_readbuf;
	fs_support[1].addr_writebuf = &fat16_writebuf;
	fs_support[1].addr_writedir = &fat16_writedir;
#endif


        build_kernel_directory();//root directory
    
	printf("\n\rFile System Starts running....\n\r");
	printf("File System Starts receiving messages....\n\r");


	struct File* tmp;
	int num;
	void *a;
	while(1){
	
		while(fs_mail[index_pop].letter_type){	
			switch(fs_mail[index_pop].letter_type){
				case FOPEN:
					tmp = fopen(&(((char*)fs_mail[index_pop].msg)[2]), &(((char*)fs_mail[index_pop].msg)[0]));
					((unsigned long*)fs_mail[index_pop].msg)[0]=(unsigned long)tmp;					
					((char*)fs_mail[index_pop].msg)[14]='Y';					
					fs_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}
					break;
				case FCLOSE:
					num = fclose((unsigned long)(((unsigned long*)fs_mail[index_pop].msg)[0]));
					((int*)fs_mail[index_pop].msg)[0]=(unsigned long)num;					
					((char*)fs_mail[index_pop].msg)[9]='Y';					
					fs_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}
					break;
				case FFLUSH:
					num = fflush((unsigned long)(((unsigned long*)fs_mail[index_pop].msg)[0]));
					((int*)fs_mail[index_pop].msg)[0]=(unsigned long)num;					
					((char*)fs_mail[index_pop].msg)[9]='Y';					
					fs_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}
					break;
				case LOAD_RBUF:
					if(fs_mail[index_pop].msg!=NULL){
						struct File* next_buff = (struct File*) fs_mail[index_pop].msg;
						
						struct fs_unit* return_fs = fs_type_support(partition[cd_rem].type);
						int index = next_buff->_cnt+1;
						openfile* tmp_addr = bl_init( &_start_+ (unsigned int)return_fs->addr_readbuf, ((unsigned long*)symbolic_fs_array[next_buff->_tmpname].file_info->director)[index], &partition[cd_rem], next_buff->_base);
						
						//openfile* real_addr = (&_start_ + (unsigned int)tmp_addr);		
						next_buff->_ptr = next_buff->_base;
						next_buff->_cnt = index;
					}
					else{
						printf("Load buf without message");
					}
					fs_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}					
					break;

				case LOAD_WBUF:
					if(fs_mail[index_pop].msg!=NULL){
						struct File* next_buff = (struct File*) fs_mail[index_pop].msg;
						
						struct fs_unit* return_fs = fs_type_support(partition[cd_rem].type);
						int index = next_buff->_cnt+1;
						if(!((unsigned long*)symbolic_fs_array[next_buff->_tmpname].file_info->director)[index]){
							/*assigned new page*/
						}
						openfile* tmp_addr = bl_init( &_start_+ (unsigned int)return_fs->addr_readbuf, ((unsigned long*)symbolic_fs_array[next_buff->_tmpname].file_info->director)[index], &partition[cd_rem], next_buff->_base);
						
						//openfile* real_addr = (&_start_ + (unsigned int)tmp_addr);		
						next_buff->_ptr = next_buff->_base;
						next_buff->_cnt = index;
					}
					else{
						printf("Load buf without message");
					}
					fs_mail[index_pop].letter_type = 0;	
					index_pop++;			
					if(index_pop == mail_size){index_pop=0;}					
					break;
				
				default:
					break;
			}
		}
		
		schedule();
	}
	


}





