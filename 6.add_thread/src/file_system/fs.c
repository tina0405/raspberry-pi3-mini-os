#include <printf.h>
#include "fat.h"
#include "fs.h"
int fs_mail[64] = {0};
extern unsigned char _end;
extern fatdir_t *dir;

void list(void){
	uart_puts("\nAttrib Cluster  Size     Name\n");
	/*need origin*/
	for(;dir->name[0]!=0;dir++) {
        // is it a valid entry?
        if(dir->name[0]==0xE5 || dir->attr[0]==0xF) continue;
        // decode attributes
        uart_send(dir->attr[0]& 1?'R':'.');  // read-only
        uart_send(dir->attr[0]& 2?'H':'.');  // hidden
        uart_send(dir->attr[0]& 4?'S':'.');  // system
        uart_send(dir->attr[0]& 8?'L':'.');  // volume label
        uart_send(dir->attr[0]&16?'D':'.');  // directory
        uart_send(dir->attr[0]&32?'A':'.');  // archive
        uart_send(' ');
        // staring cluster
        uart_hex(((unsigned int)dir->ch)<<16|dir->cl);
        uart_send(' ');
        // size
        uart_hex(dir->size);
        uart_send(' ');
        // filename
        dir->attr[0]=0;
        uart_puts(dir->name);
        uart_puts("\n");
     }
}

void fs_daemon(void)
{
	
	static int read_mail_index = 0;
	printf("File System Starts running....\n\r");
	printf("File System Starts receiving messages....\n\r");
	
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



