#include <printf.h>
#include <stddef.h>
#include <stdint.h>
#include "fat.h"
#include "fs.h"
#include "pm.h"
#include <mm.h>
#include "sched.h"
#include "utils.h"
#include "fat.h"

extern unsigned char _end;
extern fatdir_t *dir;
extern char directory[20];
extern unsigned long mod_process;
fatdir_t *origin;
int opera_addr = 0;
int rmcom_addr = 0;
void bl_init(unsigned long a);
char* comp_start=0;
struct symbol_struct{
	unsigned char sym_name[32];
	unsigned long sym_addr;
};
 
struct text_func{
	char name[16];
	int size;
};

struct com_file{
	char filename[8];
	int ksym_index;
	unsigned long rmcom;
};


struct text_func text_function[16];

extern void * const sys_call_table[];
struct com_file cfile[32];
struct symbol_struct ksym[64];
int ksym_index = 26;
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
char sd_p[4][11]; 
extern char* fat16_read_directory(struct dev sd_num);
extern char* fat32_read_directory(struct dev sd_num);
void build_kernel_directory(void){
    char* tmp;
    for(int pnum=1; pnum<4; pnum++){
	 switch(partition[pnum].type){
		case 0x0:
			break;
		case 0x0e:/*fat16*/
			memcpy( fat16_read_directory(partition[pnum]), (&sd_p[pnum][0]), 11);
			printf("A:%s ",(&sd_p[pnum][0]));
			break;	
		case 0x0c:/*fat32*/
			memcpy(fat32_read_directory(partition[pnum]), (&sd_p[pnum][0]), 11);
			break;	
		default:
		     	printf("Not support %x type in File system",partition[pnum].type);
			break;

       	 }
    }
    memcpy(fat32_read_directory(partition[0]), (&sd_p[0][0]), 11);
    printf("B: %s ",(&sd_p[0][0]));
}

void build_root(void){
	origin = dir;
	

}

struct user_fs file_dir[20];
char* search_file(void){
        int index = 0;
        memzero(file_dir,sizeof(struct user_fs)*20);
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
	return file_dir[0].name;

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
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,file_name,8)){

		/*get cluster for*/
		unsigned int clust = ((unsigned int)file_dir[k].ch)<<16|(unsigned int)file_dir[k].cl;
		if(clust){
			unsigned long addr = fat16_readfile(clust, partition[1]);	
			unsigned long a,b,d,stop;
                        unsigned char c;
		
			for(a = &_end+(addr-(unsigned int)&_end),stop =0;a < &_end+(addr-(unsigned int)&_end)+file_dir[k].size;a+=16,stop+=16) {
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
			
			//sdTransferBlocks (sect, 1, &_end, 1);

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
		/*relative*/
		unsigned int clu = fat16_getcluster(file_name, partition[1]);
		if(clu == 0){
		       	printf("\n\rNot know this file!");
		}else{
		     // read into memory
		     unsigned int adr = fat16_readfile(clu, partition[1]);
		     fat_listdirectory(&_end+(adr-(unsigned int)&_end));
		}
		     build_root();
		     search_file();
}

extern int support_type[4];
void ls_dev(void){
	/*sd_card*/
	printf("\n\rdevice     type     support     mounted\n\r");
	for(int dev_num=0;dev_num < 4 ;dev_num++){
		if(partition[dev_num].partitionlba > 0){
			printf("sda%d       ",dev_num+1);
			switch(partition[dev_num].type){
				case 0xC:					
					printf("FAT32    yes         root/boot/(reserved by system)\n\r");
					
					break;
				case 0xE:
					printf("FAT16    ");
					if(support_type[0]==1){printf("yes");
					}else{printf("no ");}
					printf("         no\n\r");	
					break;
			        default:
					printf("UNKNOWN  no          no\n\r");
					break;
				
			}			
			
		}else{
			break;
		
		}
	}
}
void ls_compt(void){


}
void cd_root(void){
		
		unsigned int adr = fat32_readfile(2, partition[0]);
	        fat_listdirectory(&_end+(adr-(unsigned int)&_end));
		build_root();
		search_file();

}
struct mod_section move_sec[7];/*0.text 1.rodata 2.data 3.bss 4.rela.text 5.symtab 6.strtab*/
char map_array[4096];/*improve*/
unsigned int map_index= 0;
int run_file(char* file_name){
	unsigned int clust =0;
        unsigned long base =0;
        
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,file_name,8)){
		clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;
		if(clust){

			printf("\n\r");
			base = fat32_readfile(clust, partition[0]);
			//printf("%x\n\r",base);
			//printf("%x\n\r",&_end+(base-(unsigned int)&_end));
			unsigned long size_u = file_dir[k].size;
			memcpy(&_end+(base-(unsigned char)&_end) , &map_array[map_index],size_u);
			copy_process(APP_THREAD, (unsigned long)&mod_process, &map_array[map_index], size_u);
			map_index = map_index + size_u;
			printf("User application: read file OK!\n\r");
			
		}
		else{
			printf("\n\rNot file");	
			return 0;	
		}
		return 1;
	   }
	   

        }
	printf("\n\rNot file");
	return 0;

}



char compt_filename[8];
int compt_file(char* file_name){/*incom*/

	unsigned int clust = 0;
        unsigned long base = 0;
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,file_name,8)){
		clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;
		if(clust){

			printf("\n\r");
			int for_i=0;
			while(for_i<32){
				if(cfile[for_i].filename[0]=='\0')
				{
					memcpy(file_name, &cfile[for_i].filename[0],8);
					break;
				}
				
				/*full not solve*/
				for_i++;
			}
			base = fat32_readfile(clust, partition[0]);
			
			unsigned long size_u = file_dir[k].size;
			printf("Component: read file OK!\n\r");
			printf("----------------------Component initial----------------------\n\r");
			

			unsigned long section_table_start;
			unsigned long section_num;
			unsigned long section_size;
			unsigned long name_index;
			
			int check = check_file_format(base ,&section_table_start,&section_num,&section_size,&name_index);
			
			if(!check){printf("\n\rNot ELF format!"); return 0;} 
			
			for(int num_sec = 0 ; num_sec < section_num ; num_sec++){
                        	find_sec_addr(section_table_start + base + num_sec * section_size);
			}
			
			unsigned long section = allocate_kernel_page();
			comp_start = section;			


			memcpy((char *)(base + move_sec[0].addr),(char *) section,move_sec[0].size);/*.text*/
			section = section + move_sec[0].size;
			memcpy((char *)(base + move_sec[1].addr),(char *) section, move_sec[1].size);/*.rodata*/
			section = section + move_sec[1].size;
			memcpy((char *)(base + move_sec[2].addr), (char *)section,move_sec[2].size);/*.data*/
			section = section + move_sec[2].size;
			memzero((char *)section,move_sec[3].size);/*.bss*/
			section = section + move_sec[3].size;
			unsigned long load_size = move_sec[0].size + move_sec[1].size + move_sec[2].size + move_sec[3].size;
			

			/*relocate*/
			int rela_err = relocate(comp_start,section_table_start,section_size,(char *)base,(char *)(base + move_sec[4].addr),move_sec[4].size);
			if(!rela_err){
				int init_addr = use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"init_compt");/*find initial*/
				opera_addr = use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"oper_compt");
				rmcom_addr = use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"exit_compt");
				cfile[for_i].rmcom = rmcom_addr + comp_start;
								
				if(init_addr<0){
					printf("Without init_comp function!");
				}else if(rmcom_addr<0){
					printf("Without exit_comp function!");
				}else{
					bl_init(comp_start+init_addr);
				}
			}
			
	/*		
			unsigned long a,b,d,stop;
                        unsigned char c;
			for(a = (char *)comp_start,stop = 0;a < (char *)comp_start +load_size;a+=16,stop+=16) {
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

		*/	
			
			//copy_process(SERVER_THREAD, (char *)comp_start, 0, 0);
			
						

		}
		else{
			printf("\n\rNot file");
			return 0;		
		}
		return 1;
	   }
	   

        }

	printf("\n\rNot file");
	return 0;

}



int rm_compt_file(char* file_name){

	unsigned int clust =0;
        unsigned long base =0;
        
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,file_name,8)){
		for(int num = 0; num<32; num++){
			if(!memcmp(cfile[num].filename,file_name,8)){ 
					printf("\n\r----------------------Component exit----------------------\n\r");
					bl_init(cfile[num].rmcom); 
					memzero(cfile[num].filename,8);
					free_page(cfile[num].rmcom);
					int page_num = ((cfile[num].rmcom - LOW_MEMORY) / PAGE_SIZE);	
							
					for(int rm_i = 26;rm_i<64;rm_i++){
						if(ksym[rm_i].sym_name[0] != '\0'){
							int page_num_1 = ((ksym[rm_i].sym_addr - LOW_MEMORY) / PAGE_SIZE);
							if(page_num == page_num_1){
								memzero(&ksym[rm_i].sym_name[0],32);/*force rmcom*/
								break;
							}
						}						
					}
									
					/*free k*/
					return 1;
			}
			
		}
	   }
	   

        }

	printf("\n\rNot file");
	return 0;

}

int unreg_compt(char* compt_name){
	int length = strlength(compt_name);
	int ksym_i = 9,compt_i=0;
	for(int num = 26; num<64; num++){
			if(!memcmp(&ksym[num].sym_name[9],compt_name,length)){
				printf("Succeed to unregister symbol!\n\r");				
				return 0;/*succeed*/
			}
	}
	printf("kservice_%s was not in symbol table!",compt_name);
	return 1;/**/

}

int strlength(char* string){
	int length =0;
	while(*(string + length) !='\0'){length++;}
	return length;
}

int reg_compt(char* compt_name){/*回傳 num*/
	int ksym_i = 9,compt_i=0;
	int length = strlength(compt_name);
	for(int num = 0; num<64; num++){
		if(!memcmp(&ksym[num].sym_name[9],compt_name,length)){
			printf("Cannot register! kservice_%s has existed!",compt_name);
			return 1;/*fail*/
		}

		if(ksym[num].sym_name[0] == '\0'){
			memcpy("kservice_",&ksym[num].sym_name[0],9);
			while(*(compt_name + compt_i)!= '\0'){
				ksym[num].sym_name[ksym_i++] = *(compt_name + (compt_i++));		
			}
			ksym[num].sym_addr = comp_start + opera_addr;
			printf("Register component function: %s\n\r",ksym[num].sym_name);
			return 0; /*succeed*/
		}
	}
	printf("Cannot register! Symbol table is full!");
	return 1;/**/
}

int get_ndx(Elf64_Sym* sym){

	//printf("st_name:%x st_other:%x\n\r",(sym+init)->st_name,(sym+init)->st_other);
	//printf("bind:%x type:%x \n\r",(sym+init)->st_info >> 4,(sym+init)->st_info & 0xf);
	//printf("st_shndx:%x st_value:%x st_size:%x\n\r",(sym+init)->st_shndx,(sym+init)->st_value,(sym+init)->st_size);

	return sym->st_shndx;

}

/*init*//*cleanup*//*operation*/
int use_compt_func(char* base,Elf64_Sym* sym,int size,char* fun_name){
	int num =0;
	unsigned int initial = 0;
	for(int init=0; init < size/24 ;init++){
		if((unsigned int)(sym+init)->st_shndx == 1 && (unsigned int)(sym+init)->st_size != 0){
                        
			int i = 0;	
			char* chara = base + move_sec[6].addr+ (unsigned int)(sym+init)->st_name;
			while(*(chara+i)!='\0'){
				text_function[num].name[i] = *(chara+i);
				i++;
			}
		
			text_function[num].size = (sym+init)->st_size;
			if(!memcmp(&text_function[num].name[0],fun_name,10)){
				return  initial;
			}
			
			initial =  initial + (sym+init)->st_size;
			num++;
		}

	}
	return -1;
	//printf("st_name:%x st_other:%x\n\r",(sym+init)->st_name,(sym+init)->st_other);
	//printf("bind:%x type:%x \n\r",(sym+init)->st_info >> 4,(sym+init)->st_info & 0xf);
	//printf("st_shndx:%x st_value:%x st_size:%x\n\r",(sym+init)->st_shndx,(sym+init)->st_value,(sym+init)->st_size);
}

int get_strname(Elf64_Sym* sym){
	return sym->st_name;

}

int relocate(char* comp_start,unsigned long section_table_start,unsigned long section_size,char* base,Elf64_Rela* rela,unsigned long size){

	for(int init=0; init < size/24 ;init++){

		if((unsigned int)(rela+init)->r_info==0x113){}
		else if((unsigned int)(rela+init)->r_info == 0x115){
		  
		  int ndx = get_ndx(base + move_sec[5].addr + 24*((rela+init)->r_info >> 32));
	          int rel_num =	find_sec_addr(base + section_table_start + ndx*(section_size));
		  unsigned int* ch_test = (comp_start + (rela+init)->r_offset);
		
		  if(rel_num == 1){ /*rodata*/
			 *ch_test = (((move_sec[0].size + (rela+init)->r_addend)*4)<<8) + (0x91000000);
			 
		  }else if(rel_num==2){ /*data*/
			 *ch_test = (((move_sec[0].size + move_sec[1].size + (rela+init)->r_addend)*4)<<8)+(0x91000000);
		  }else if(rel_num==3){ /*bss*/
			 *ch_test = (((move_sec[0].size + move_sec[1].size + move_sec[2].size + (rela+init)->r_addend)*4)<<8)+(0x91000000);
                  }else{
			  printf("Not data section!");
		  }
		}else if((unsigned int)(rela+init)->r_info==0x11b){
		   int ndx = get_ndx(base + move_sec[5].addr + 24*((rela+init)->r_info >> 32));/*hi->symbol name*/
			/*0.text 1.rodata 2.data 3.bss 4.rela.text 5.symtab 6.strtab*/
		   if(ndx==0){
			int strname = get_strname(base + move_sec[5].addr + 24*((rela+init)->r_info >> 32));
			char str_name[30]={'\0'};
			int i = 0,ksym_i = 0,flag =0;
			
			
			char* chara =base + move_sec[6].addr+ (int)strname;
			while(*(chara+i)!='\0'){
				str_name[i] = *(chara+i);
				i++;
			}
			
			while(ksym[ksym_i].sym_name[0]!='\0')
				if(!memcmp(&ksym[ksym_i++] , &str_name[0] ,i-1)){flag = 1; break;}

			if(flag==1){
				unsigned int value = 0x3ffffff -((((int)comp_start + (rela+init)->r_offset) - ksym[ksym_i-1].sym_addr)/4)+1;
				unsigned int* bl_test = (comp_start + (rela+init)->r_offset);
				*bl_test = value + 0x94000000;
			}else{
				printf("Not componets support this function: %s\n\r",str_name);
				return -1;
			}

		   }else{

			printf("To do list\n\r");
		   }
		}
		//printf("off:%x\n\r",(rela+init)->r_offset);
		//printf("sym:%x\n\r",(rela+init)->r_info >> 32);
		//printf("info:%x\n\r",(rela+init)->r_info);
		//printf("addend:%x\n\r",(rela+init)->r_addend);
	}
	return 0;

}

int find_sec_addr(Elf64_Shdr *header){
	if(move_sec[0].num == header ->sh_name ){
	   move_sec[0].addr = header -> sh_offset;
	   move_sec[0].size =	header -> sh_size;
	   return 0;
	
	}else if(move_sec[1].num == header ->sh_name){
	   move_sec[1].addr = header -> sh_offset;
	   move_sec[1].size = header -> sh_size;
	   return 1;

	}else if(move_sec[2].num == header ->sh_name){
	   move_sec[2].addr = header -> sh_offset;
	   move_sec[2].size =	header -> sh_size;
	   return 2;
	}else if(move_sec[3].num == header ->sh_name){

	   move_sec[3].addr = header -> sh_offset;
	   move_sec[3].size =	header -> sh_size;
	   return 3;
	}else if(move_sec[4].num == header ->sh_name){

	   move_sec[4].addr = header -> sh_offset;
	   move_sec[4].size =	header -> sh_size;
	   return 4;
	}else if(move_sec[5].num == header ->sh_name){

	   move_sec[5].addr = header -> sh_offset;
	   move_sec[5].size =	header -> sh_size;
	   return 5;
	}else if(move_sec[6].num == header ->sh_name){

	   move_sec[6].addr = header -> sh_offset;
	   move_sec[6].size =	header -> sh_size;
	   return 6;
	}				
	
	return -1;
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

void string_table(Elf64_Shdr *section,Elf64_Ehdr * header){

	unsigned long section_size = section->sh_size;
	get_string(section->sh_offset+(unsigned long)header,section_size);
}

void get_string(char* addr,unsigned long size){
	unsigned long b;
        char c;

	for(b=0;b<size;b++) {
		c=*((char*)(addr+b));
		
		if(c=='.'){

		      if(*((char*)(addr+b+1))=='t'&&*((char*)(addr+b+2))=='e'&&*((char*)(addr+b+3))=='x'&&*((char*)(addr+b+4))=='t')
		      {
				move_sec[0].num = b;
			
		      }
		      else if(*((char*)(addr+b+1))=='r'&&*((char*)(addr+b+2))=='o'&&*((char*)(addr+b+3))=='d'&&*((char*)(addr+b+4))=='a'&&*((char*)(addr+b+5))=='t'&&*((char*)(addr+b+6))=='a')
		      {
				move_sec[1].num = b;
				
		      }
		      else if(*((char*)(addr+b+1))=='d'&&*((char*)(addr+b+2))=='a'&&*((char*)(addr+b+3))=='t'&&*((char*)(addr+b+4))=='a')
		      {
				move_sec[2].num = b;
				
		      }
			
		      else if(*((char*)(addr+b+1))=='b'&&*((char*)(addr+b+2))=='s'&&*((char*)(addr+b+3))=='s')
		      {
				move_sec[3].num = b;
			
		      }
		       else if(*((char*)(addr+b+1))=='r'&&*((char*)(addr+b+2))=='e'&&*((char*)(addr+b+3))=='l'&&*((char*)(addr+b+4))=='a'&&*((char*)(addr+b+5))=='.'&&*((char*)(addr+b+6))=='t'&&*((char*)(addr+b+7))=='e'&&*((char*)(addr+b+8))=='x'&&*((char*)(addr+b+9))=='t')/*rela.text*/
		      {
				move_sec[4].num = b;
			
		      }
		       else if(*((char*)(addr+b+1))=='s'&&*((char*)(addr+b+2))=='y'&&*((char*)(addr+b+3))=='m'&&*((char*)(addr+b+4))=='t'&&*((char*)(addr+b+5))=='a'&& *((char*)(addr+b+6))=='b'/*symtab*/)
		      {
				move_sec[5].num = b;
			
		      }
		      else if(*((char*)(addr+b+1))=='s'&&*((char*)(addr+b+2))=='t'&&*((char*)(addr+b+3))=='r'&&*((char*)(addr+b+4))=='t'&&*((char*)(addr+b+5))=='a'&& *((char*)(addr+b+6))=='b'/*strtab*/)
		      {
				move_sec[6].num = b;
			
		      }
		
		      
		      
		
		}
	}
}


int read_ksymbol(){
	unsigned int clust =0;
        unsigned long base =0;
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,"SYMBOL  TXT",8)){
		clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;
		
		if(clust){

			
			base = fat32_readfile(clust, partition[0]);
			/*save kernel symbol*/
			unsigned int name_word=0,aa=0,base_index=0;
			while(*((unsigned char*)(&_end+(base-(unsigned int)&_end) + base_index))!= 0x00){	
	
				if(*((unsigned char*)(&_end+(base-(unsigned int)&_end) + base_index)) != 0xA){
					ksym[name_word].sym_name[aa++] = *((unsigned char*)(&_end+(base-(unsigned int)&_end) + base_index));
				}else{
					ksym[name_word].sym_addr = sys_call_table[name_word];
					name_word++;
					aa = 0;			
				}
				base_index++;
			}

			return 0;
			
		}
		else{
			printf("Cannot read kernel symbol.\n\r");
			return 1;
	   	}
	   }
	   
	}
	
	printf("Cannot find kernel symbol.\n\r");

}




