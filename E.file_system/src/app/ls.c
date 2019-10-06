#include "fs.h"
#include "mini_uart.h"
extern int cd_rem;
void list(){
    uart_puts("\nAttrib Size     Cluster  Name\n");
    if(cd_rem==-1){/*root*/
	for(int k = 0;k<4;k++){
		if(file_dir[k].name[0]!='\0'){
		   uart_send(file_dir[k].attr[0]& 1?'R':'.');  // read-only
		   uart_send(file_dir[k].attr[0]& 2?'H':'.');  // hidden
		   uart_send(file_dir[k].attr[0]& 4?'S':'.');  // system
		   uart_send(file_dir[k].attr[0]& 8?'L':'.');  // volume label
		   uart_send(file_dir[k].attr[0]&16?'D':'.');  // directory
		   uart_send(file_dir[k].attr[0]&32?'A':'.');  // archive
		   uart_send(' ');
		   uart_hex(file_dir[k].size);
		   uart_send(' ');
		   uart_hex(((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl);
		   uart_send(' ');
		   uart_puts(file_dir[k].name);
		   uart_send(' ');
		   uart_puts("\n");
                 }
	}


    }else{
	    for(int k = 0;file_dir[k].name[0]!='\0';k++){
		   uart_send(file_dir[k].attr[0]& 1?'R':'.');  // read-only
		   uart_send(file_dir[k].attr[0]& 2?'H':'.');  // hidden
		   uart_send(file_dir[k].attr[0]& 4?'S':'.');  // system
		   uart_send(file_dir[k].attr[0]& 8?'L':'.');  // volume label
		   uart_send(file_dir[k].attr[0]&16?'D':'.');  // directory
		   uart_send(file_dir[k].attr[0]&32?'A':'.');  // archive
		   uart_send(' ');
		   uart_hex(file_dir[k].size);
		   uart_send(' ');
		   uart_hex(((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl);
		   uart_send(' ');
		   uart_puts(file_dir[k].name);
		   uart_send(' ');
		   uart_puts("\n");
	     }
     }
}
