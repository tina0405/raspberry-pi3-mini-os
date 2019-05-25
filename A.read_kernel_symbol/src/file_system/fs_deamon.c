#include <printf.h>
#include <pm.h>
#include <mm.h>
#include <sched.h>

int fs_mail[64] = {0};

void fs_daemon(void)
{	
	static int read_mail_index = 0;
	printf("File System send a message (int 3) to ipc_test (use pid as an address)\n\r");
	//send_msg(Rendezvous, 2, 3);
	printf("File System Starts running....\n\r");
	printf("File System Starts receiving messages....\n\r");

	read_ksymbol();
	
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
	while(1){
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
			
		}

		
	}
	


}




