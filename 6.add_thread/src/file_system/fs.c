#include <printf.h>
#include "fat.h"
#include "fs.h"
int fs_mail[64] = {0};
extern unsigned char _end;
extern fatdir_t *dir;
extern char directory[20];
fatdir_t *origin;
struct user_fs;
/*
struct user_fs{
    
    char           name[8];
    char            attr[9];
    unsigned int    size;
    struct user_fs* folder;
};
*/
struct user_fs{
    
    char           name[11];
    char            attr[9];
    unsigned int    size;
    struct user_fs* folder;
};
void build_root(void){
	origin = dir;	
}
struct user_fs file_dir[20];

void search_file(void){
        int index = 0;
	memzero(file_dir,29*20);
        for(;origin->name[0]!=0;origin++) {
		// is it a valid entry?
		if(origin->name[0]==0xE5 || origin->attr[0]==0xF) continue;
		// decode attributes

		file_dir[index].size = origin -> size;
		memcpy(origin -> attr,file_dir[index].attr,9);	
		memcpy(origin -> name,file_dir[index].name,11);
		index++;
		
	}

}

void list(){

    uart_puts("\nAttrib Size     Name    Type\n");
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
	   uart_puts(file_dir[k].name);
           uart_puts("\n");

     }

}

void cd(char* file_name){
		//char directory[20]
		unsigned int clu = fat_getcluster(file_name);
		if(clu ==0){
		       	printf("\n\rNot know this file!");;
		}else{
		       				 // read into memory
		       	unsigned int adr = fat_readfile(clu);
			fat_listdirectory(&_end+(adr-(unsigned int)&_end));
		}
		build_root();
		search_file();

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



