#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>
#include <fs.h>
#include "fat16.h"
#include "fat32.h"

int fs_mail[64] = {0};
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
		//printf("FS");
		if(read_mail_index == 64){
			read_mail_index = 0;
		}
  
		switch(fs_mail[read_mail_index]){
			case 1: /*ls*/

				
				read_mail_index++;
				break;

			case 2:
				//send();
				read_mail_index++;
				break;

			default:
				schedule();
				break;
			
		}

		
	}
	


}




