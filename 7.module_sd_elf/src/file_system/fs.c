#include <printf.h>
#include "fat.h"
#include "fs.h"
#include "pm.h"
#include "sched.h"
int fs_mail[64] = {0};
extern unsigned char _end;
extern fatdir_t *dir;
extern char directory[20];
extern unsigned long mod_process;
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
    unsigned short  ch;
    unsigned short  cl;
    struct user_fs* folder;
};
void build_root(void){
	origin = dir;	
}
struct user_fs file_dir[20];

void search_file(void){
        int index = 0;
	memzero(file_dir,33*20);
	
        for(;origin->name[0]!=0;origin++) {
		// is it a valid entry?
		if(origin->name[0]==0xE5 || origin->attr[0]==0xF) continue;
		// decode attributes
		
		memcpy(origin -> name,file_dir[index].name,11);
		memcpy(origin -> attr,file_dir[index].attr,9);	
		file_dir[index].size = origin -> size;
		file_dir[index].ch = origin -> ch;		
		file_dir[index].cl = origin -> cl;
		
		index++;
		
	}

}

void list(){

    uart_puts("\nAttrib Size     Cluster  Name\n");
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

int dump(char* file_name){
unsigned int clust =0;
        unsigned long addr=0;
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,file_name,8)){
		clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;
		if(clust){
			printf("\n\r");
			addr = fat_readfile(clust);
			unsigned long a,b,d,stop;
                        unsigned char c;
			
			for(a = addr,stop =0;a<addr+file_dir[k].size;a+=16,stop+=16) {
				uart_hex(a); uart_puts(": ");
				for(b=0;b<16;b++) {
				    c=*((unsigned char*)(a+b));
				    d=(unsigned int)c;d>>=4;d&=0xF;d+=d>9?0x37:0x30;
				    uart_send(d);
				    d=(unsigned int)c;d&=0xF;d+=d>9?0x37:0x30;
				    uart_send(d);
				    uart_send(' ');
				    if(b%4==3)
				    uart_send(' ');
				}
				for(b=0;b<16;b++) {
				    c=*((unsigned char*)(a+b));
				    uart_send(c<32||c>=127?'.':c);
				}
				uart_send('\r');
				uart_send('\n');
			
				if(stop==512){
					stop = 0;
					while(!uart_recv()){}
				}
   			}

		}
		else{
			printf("\n\rNot file");		
		}
		return 0;
	   }
	   

        }
	printf("\n\rNot file");

}

void cd(char* file_name){
		//char directory[20]
		unsigned int clu = fat_getcluster(file_name);
		if(clu ==0){
		       	printf("\n\rNot know this file!");
		}else{
		       				 // read into memory
		       	unsigned int adr = fat_readfile(clu);
			fat_listdirectory(&_end+(adr-(unsigned int)&_end));
		}
		build_root();
		search_file();

}

void cd_root(void){
		
		unsigned int adr = fat_readfile(2);
	        fat_listdirectory(&_end+(adr-(unsigned int)&_end));
		build_root();
		search_file();

}
struct mod_section move_sec[4];/*0.text 1.data 2.bss 3.rodata*/
char map_array[4096];/*improve*/
int mod_file(char* file_name){
	unsigned int clust =0;
        unsigned long base =0;
        
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,file_name,8)){
		clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;
		if(clust){

			printf("\n\r");

			base = fat_readfile(clust);
			
			unsigned long size_u = file_dir[k].size;
			memcpy(base , &map_array[0],size_u);
			copy_process(SERVER_THREAD, (unsigned long)&mod_process, &map_array[0], size_u);
			printf("module OK\n\r");
			
/*
			unsigned long section_table_start;
			unsigned long section_num;
			unsigned long section_size;
			unsigned long name_index;
			
			int check = check_file_format(base ,&section_table_start,&section_num,&section_size,&name_index);
			
			if(!check){printf("\n\rNot ELF format!"); return 0;} 
			
			for(int num_sec = 0 ; num_sec < section_num ; num_sec++){
                        	find_sec_addr(section_table_start + base + num_sec * section_size);
			}
			
			//unsigned long section = allocate_kernel_page();
			char map_array[move_sec[0].size + move_sec[3].size + move_sec[1].size + move_sec[2].size];

	 		char * A = (char *)(base + move_sec[0].addr);
			memcpy((char *)(base + move_sec[0].addr), &map_array[0],move_sec[0].size);
			memcpy((char *)(base + move_sec[3].addr), &map_array[move_sec[0].size-1], move_sec[3].size);
			memcpy((char *)(base + move_sec[1].addr), &map_array[move_sec[0].size + move_sec[3].size -1],move_sec[1].size);
			memzero((char *)(&map_array[move_sec[0].size + move_sec[3].size + move_sec[1].size -1]),move_sec[2].size);
			
			
			unsigned long load_size = move_sec[0].size + move_sec[3].size + move_sec[1].size + move_sec[2].size;
			
			copy_process(SERVER_THREAD, (unsigned long)&mod_process, &map_array[0], load_size);
			
			//mod_process(section,move_sec[0]->size+ move_sec[3]->size + move_sec[1]->size);			
*/
		}
		else{
			printf("\n\rNot file");		
		}
		return 1;
	   }
	   

        }
	printf("\n\rNot file");

}

int find_sec_addr(Elf64_Shdr *header){
	if(move_sec[0].num == header ->sh_name ){
	   move_sec[0].addr = header -> sh_offset;
	   move_sec[0].size =	header -> sh_size;
	
	}else if(move_sec[1].num == header ->sh_name ){
	   move_sec[1].addr = header -> sh_offset;
	   move_sec[1].size = header -> sh_size;

	}else if(move_sec[2].num == header ->sh_name ){
	   move_sec[2].addr = header -> sh_offset;
	   move_sec[2].size =	header -> sh_size;

	}else if(move_sec[3].num == header ->sh_name ){
	   move_sec[3].addr = header -> sh_offset;
	   move_sec[3].size =	header -> sh_size;
	}	
	
	return 0;
}

int check_file_format(Elf64_Ehdr *header,unsigned long *section_table_start ,unsigned long *section_num, unsigned long *section_size,unsigned long *name_index)
{
	if(!(header->e_ident[EI_MAG0] == 0x7f && header->e_ident[EI_MAG1] == 'E' && header->e_ident[EI_MAG2] == 'L' && header->e_ident[EI_MAG3] == 'F'))return 0;
	*section_table_start = 0;
	*section_num = header->e_shnum;
	*section_size = header->e_shentsize;
	*name_index = header->e_shstrndx;
	if((header->e_shoff)%8==0){
	     *section_table_start = header->e_shoff;	
	}	
	else{
	     *section_table_start = header->e_shoff+(8 - ((header->e_shoff) % 8));
	}
	string_table(*section_table_start+ (unsigned long) header +  header->e_shentsize * header->e_shstrndx, header);	
  	return 1; 
}

int string_table(Elf64_Shdr *section,Elf64_Ehdr * header){

	unsigned long section_size = section->sh_size;
	get_string(section->sh_offset+(unsigned long)header,section_size);
	return 0;


}

int get_string(char* addr,unsigned long size){
	unsigned long b;
        char c;

	for(b=0;b<size;b++) {
		c=*((char*)(addr+b));
		
		if(c=='.'){

		      if(*((char*)(addr+b+1))=='t'&&*((char*)(addr+b+2))=='e'&&*((char*)(addr+b+3))=='x'&&*((char*)(addr+b+4))=='t')
		      {
				move_sec[0].num = b;
		      }
		      
		      else if(*((char*)(addr+b+1))=='d'&&*((char*)(addr+b+2))=='a'&&*((char*)(addr+b+3))=='t'&&*((char*)(addr+b+4))=='a')
		      {
				move_sec[1].num = b;
		      }
			
		      else if(*((char*)(addr+b+1))=='b'&&*((char*)(addr+b+2))=='s'&&*((char*)(addr+b+3))=='s')
		      {
				move_sec[2].num = b;
		      }
		      
		      else if(*((char*)(addr+b+1))=='r'&&*((char*)(addr+b+2))=='o'&&*((char*)(addr+b+3))=='d'&&*((char*)(addr+b+4))=='a'&&*((char*)(addr+b+5))=='t'&&*((char*)(addr+b+6))=='a')
		      {
				move_sec[3].num = b;
		      }
		
		}
	}
	return 0;
}






void fs_daemon(void)
{
	
	static int read_mail_index = 0;
	
	
	printf("File System send a message (int 3) to ipc_test (use pid as an address)\n\r");
	send_msg(Rendezvous, 2, 3);
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



