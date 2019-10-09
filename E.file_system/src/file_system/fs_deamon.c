#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>
#include <fs.h>
#include "fat16.h"
#include "fat32.h"
#include <stddef.h>
#define OPENF 1
struct mailbox fs_mail[mail_size]={NULL};
static int index_push = 0;
static int index_pop = 0;
//int support_type[4] = {0}; /*0:fat16*/

void fs_daemon(void)
{	
	static int read_mail_index = 0;
	//printf("File System send a message (int 3) to ipc_test (use pid as an address)\n\r");
	//send_msg(Rendezvous, 2, 3);

	
	//register fat32
	fs_support[0].type = 0x0c;
	fs_support[0].addr_directory = &fat32_read_directory;
 	fs_support[0].addr_getcluster = &fat32_getcluster;
 	fs_support[0].addr_readfile = &fat32_readfile;
	//fs_support[0].addr_getpos = &fat32_getpos;
#ifdef FAT16
	//register fat16
	fs_support[1].type = 0x0e;
	fs_support[1].addr_directory = &fat16_read_directory;
 	fs_support[1].addr_getcluster = &fat16_getcluster;
 	fs_support[1].addr_readfile = &fat16_readfile;
	//support_type[0] = 1;
#endif
	
        build_kernel_directory();//root directory
    
	printf("\n\rFile System Starts running....\n\r");
	printf("File System Starts receiving messages....\n\r");

	
	
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
	while(1){
	
		while(fs_mail[index_pop].letter_type){	
			switch(fs_mail[index_pop].letter_type){
				case OPENF:
					//tmp_pcb = task[fs_mail[index_pop].dst_task];
					//tmp_pcb->Rdv = fs_mail[index_pop];
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

void send_fs_msg(unsigned int type, int tid, int addr, void* msg,int size){/*without size*/
	/*push*/	
	if(fs_mail[index_push].letter_type != 0){
		printf("Push pm_mail error! Mailbox is Full\n\r");	
	}
	else{
		fs_mail[index_push].letter_type = type;/*0:empty*/
		fs_mail[index_push].dst_task = tid;/*exit_thread*/
		fs_mail[index_push].from = current;
		if(size){
			struct mm_info msg_mm = allocate_kernel_page(size);	
			fs_mail[index_push].msg = msg_mm.start;
			memcpy(msg, msg_mm.start,11);
		}else{
			fs_mail[index_push].msg = NULL;
		}		
		index_push++;
		if(index_push == mail_size){index_push=0;}
		//accept_reply();
	}
	
}





