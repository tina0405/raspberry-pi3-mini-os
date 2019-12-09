/*type: mailbox or rendezvous or end_thread*/
#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>
#include <stddef.h>
#include <ipc.h>
int pm_index_push = 0;
int fs_index_push = 0;
int compt_index_push = 0;

extern struct mailbox pm_mail[mail_size];
extern struct mailbox fs_mail[mail_size];
extern struct mailbox compt_mail[mail_size];

int send_msg(unsigned int type, int tid, int addr, void* msg,int size){/*without size*/
	/*push*/
	if(addr == PROCESS_MANAGER){/*process manager*/	
		if(pm_mail[pm_index_push].letter_type != 0){
			printf("Push pm_mail error! Mailbox is Full\n\r");
			return 1;	
		}
		else{
			pm_mail[pm_index_push].letter_type = type;/*0:empty*/
			pm_mail[pm_index_push].dst_task = tid;/*exit_thread*/
			pm_mail[pm_index_push].from = current;
			if(size){
				struct mm_info msg_mm = allocate_kernel_page(size);	
				pm_mail[pm_index_push].msg = msg_mm.start;
				memcpy(msg, msg_mm.start,11);
			}else{
				pm_mail[pm_index_push].msg = NULL;
			}		
			pm_index_push++;
			if(pm_index_push == mail_size){pm_index_push = 0;}
			return 0;
			
		}
	}else if(addr == FILESYS_MANAGER){/*1:file system manager*/
		if(fs_mail[fs_index_push].letter_type != 0){
			printf("Push fs_mail error! Mailbox is Full\n\r");
			return 1;	
		}
		else{
			fs_mail[fs_index_push].letter_type = type;/*0:empty*/
			fs_mail[fs_index_push].dst_task = tid;/*exit_thread*/
			fs_mail[fs_index_push].from = current;
			if(size){
				//struct mm_info msg_mm = allocate_kernel_page(size);	
				fs_mail[fs_index_push].msg = msg;
				//memcpy(msg, msg_mm.start,13);
			}else{
				fs_mail[fs_index_push].msg = NULL;
			}		
			fs_index_push++;
			if(fs_index_push == mail_size){fs_index_push = 0;}
			return 0;
		}
 

	}else if(addr == COMPONENT_MANAGER){/*2:component manager*/
		if(compt_mail[compt_index_push].letter_type != 0){
			printf("Push compt_mail error! Mailbox is Full\n\r");	
			return 1;
		}
		else{
			compt_mail[compt_index_push].letter_type = type;/*0:empty*/
			compt_mail[compt_index_push].dst_task = tid;/*exit_thread*/
			compt_mail[compt_index_push].from = current;
			if(size){
				//struct mm_info msg_mm = allocate_kernel_page(size);	
				compt_mail[compt_index_push].msg = msg;
				//memcpy(msg, msg_mm.start,40);
			}else{
				compt_mail[compt_index_push].msg = NULL;
			}		
			compt_index_push++;
			if(compt_index_push == mail_size){compt_index_push = 0;}
			return 0;
		}
	}
	
}
