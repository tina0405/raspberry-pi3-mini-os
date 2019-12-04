#include <printf.h>
#include <stddef.h>
#include <stdint.h>
#include "fat.h"
#include "fs.h"
#include "pm.h"
#include "mm.h"
#include "cm.h"
#include "sched.h"
#include "thread.h"
#include "utils.h"
#include "fat16.h"
#include "fat32.h"
#include "mini_uart.h"

int opera_addr = 0;
int rmcom_addr = 0;
char* comp_start=0;
extern int cd_rem;
extern unsigned char _start_;


struct text_func{
	char name[16];
	int size;
};


struct text_func text_function[16];
extern unsigned char _end;
extern void * const sys_call_table[];
struct com_file cfile[64];/*Component Table*/
struct symbol_struct ksym[128];/*Kernel Table*/
struct hard_struct hardware_table[40];


int com_index=0;


struct mod_section move_sec[7];/*0.text 1.rodata 2.data 3.bss 4.rela.text 5.symtab 6.strtab*/

char compt_filename[8];
int compt_file(char* file_name){/*incom*/
	unsigned int clust = 0;
	volatile openfile* basefile;
        unsigned long base = 0;
        struct dev* dev_param;
	struct fs_unit* return_fs;
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,file_name,8)){
		clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;
		if(clust){

			printf("\n\r");
			com_index = 0;
			while(com_index<64){
				if(cfile[com_index].filename[0]=='\0')
				{
					memcpy(file_name, &cfile[com_index].filename[0],8);
					
					break;
				}
				
				
				com_index++;
				if(com_index==32){
					printf("\n\rComponent Table is Full!");
					return 0;
				}
			}

			return_fs = fs_type_support(partition[cd_rem].type);
                        dev_param = &partition[cd_rem];
               		if(return_fs){
				basefile = bl_init( &_start_+ (unsigned int)return_fs->addr_readfile, clust, dev_param);
				base = (unsigned long)basefile->log_addr;
			}else{
				printf("Not support %x type in File system",partition[cd_rem].type);
				return 0;
			}

			
			unsigned long size_u = file_dir[k].size;
			printf("Component: read file OK!\n\r");
			printf("----------------------Component initial----------------------\n\r");
			
			//base = (unsigned long)basefile->log_addr;	
			unsigned long section_table_start;
			unsigned long section_num;
			unsigned long section_size;
			unsigned long name_index;
			
			int check = check_file_format(base ,&section_table_start,&section_num,&section_size,&name_index);
			if(!check){printf("\n\rNot ELF format!"); return 0;} 
			for(int num_sec = 0 ; num_sec < section_num ; num_sec++){
                        	find_sec_addr(section_table_start + base + num_sec * section_size);
			}
			struct mm_info section_mm;
			section_mm = allocate_kernel_page(4096);
			unsigned long section = section_mm.start;
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
				opera_addr = use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"oprt_compt");
				rmcom_addr = use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"exit_compt");
				/*clear component table*/
				cfile[com_index].rmcom = rmcom_addr + comp_start;
				cfile[com_index].sym = NULL;			
				if(init_addr<0){
					printf("Without init_comp function!");
				}else if(rmcom_addr<0){
					printf("Without exit_comp function!");
				}else{
					bl_init(comp_start+init_addr,0);
					/*without*/
				}
			}
			
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

int compt_sched_file(char* file_name){/*incom*/

	unsigned int clust = 0;
	openfile* basefile;
        unsigned long base = 0;
        struct dev* dev_param;
	struct fs_unit* return_fs;
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	   if(!memcmp(file_dir[k].name,file_name,8)){
		clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;
		if(clust){

			printf("\n\r");
			return_fs = fs_type_support(partition[cd_rem].type);
                        dev_param = &partition[cd_rem];
               		if(return_fs){
				basefile = bl_init( &_start_+(return_fs->addr_readfile -(unsigned int)&_start_), clust, dev_param);
			}else{
				printf("Not support %x type in File system",partition[cd_rem].type);
				return 0;
			}
			
			unsigned long size_u = file_dir[k].size;
			printf("Component: read file OK!\n\r");
			printf("----------------------Change Schedule----------------------\n\r");
			
			base = basefile->log_addr;
			unsigned long section_table_start;
			unsigned long section_num;
			unsigned long section_size;
			unsigned long name_index;
			
			int check = check_file_format(base ,&section_table_start,&section_num,&section_size,&name_index);
			if(!check){printf("\n\rNot ELF format!"); return 0;} 
			
			for(int num_sec = 0 ; num_sec < section_num ; num_sec++){
                        	find_sec_addr(section_table_start + base + num_sec * section_size);
			}
			
			struct mm_info section_mm;
			section_mm = allocate_kernel_page(4096);
			unsigned long section = section_mm.start;
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
				base = use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"sched_policy");
				if(base<0){
					printf("Without sched_policy function!");
					sched_type = &round_robin;
				}
				sched_type = comp_start+base;
			}
				
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
        
	
	 
	   
		for(int num = 0; num<64; num++){
			if(!memcmp(cfile[num].filename,file_name,8)){ 
					printf("\n\r----------------------Component exit----------------------\n\r");
					bl_init(cfile[num].rmcom,0); 
					/*check unreg and release hardware*/
					for(int n = 0; n<32; n++){/*unreg*/
						if(!memcmp(cfile[n].filename,file_name,8)){
							printf("\n\rThere are apps and compoenets using %s\n\r",cfile[n].filename);
							printf("System advices adding unreg_compt(char* compt_name) in exit_compt() function \n\ror forcing to remove component:[rmcom -f compt_name]");
							return 0; 
						}
					}
					memzero(cfile[num].filename,8);
					free_page(cfile[num].rmcom,1);
					memzero(&(cfile[num].sym->sym_name[0]),32);
					free_page(cfile[num].sym->sym_addr,1);
						
					/*free k*/
					return 1;
			}
			
		}
	   
	   

        

	printf("\n\rNot file A");
	return 0;

}

int unreg_compt(char* compt_name){/*remove app*/
	int length = strlength(compt_name);
	for(int num = kapi_count; num<128; num++){
			if(!memcmp(&ksym[num].sym_name[9],compt_name,length)){
				ksym[num].sym_name[0] = '\0' ;
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

int reg_compt(char* compt_name){/*return num*/
	int ksym_i = 9,compt_i=0;
	int length = strlength(compt_name);
	for(int num = kapi_count; num<128; num++){
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
			//memcpy(compt_name, &cfile[com_index].filename[0],8);
			cfile[com_index].sym = &ksym[num];	
			return 0; /*succeed*/
		}
	}
	printf("Cannot register! Symbol table is full!");
	return 1;/**/
}

extern struct pcb_struct *thread_id_table[4096];/*tid and pcb map*/
int hardware_request(unsigned int address){
	

	if(hardware_table[address].app_count != 0){
		
		if(hardware_table[address].app_count == 1 && *(hardware_table[address].app_page) == thread_id_self())
		{return 0;}
		printf("GPIO %x is used by applications:",address);
		for(int i = 0; i<hardware_table[address].app_count; i++){
			printf("pcb number: %x\n\r",*(hardware_table[address].app_page+i));
		}
		return 1;/*fail*/
	}
	if(hardware_table[address].app_page==NULL){	
		struct mm_info app_page = allocate_kernel_page(4096);	
		hardware_table[address].app_page = app_page.start;
	}
	*(hardware_table[address].app_page + hardware_table[address].app_count) = thread_id_table[thread_id_self()];
	
        if(thread_id_table[thread_id_self()]-> hardware==NULL){	
		struct mm_info h_page = allocate_kernel_page(4096);	
		thread_id_table[thread_id_self()]-> hardware = h_page.start;
	}
	*(thread_id_table[thread_id_self()]-> hardware + thread_id_table[thread_id_self()]->h_count) = address;
	thread_id_table[thread_id_self()]->h_count++;
	
	printf("\n\rTid:%x pcb:%x\n\r",thread_id_self(),thread_id_table[thread_id_self()]);
        hardware_table[address].app_count++;
	return 0;/*succeed*/
}

int get_ndx(Elf64_Sym* sym){

	//printf("st_name:%x st_other:%x\n\r",(sym)->st_name,(sym)->st_other);
	//printf("bind:%x type:%x \n\r",(sym)->st_info >> 4,(sym)->st_info & 0xf);
	//printf("st_shndx:%x st_value:%x st_size:%x\n\r",(sym)->st_shndx,(sym)->st_value,(sym)->st_size);

	return sym->st_shndx;

}

int get_st_value(Elf64_Sym* sym){

	return sym->st_value;

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
			//printf("%s\n\r",&text_function[num].name[0]);
			if(!memcmp(&text_function[num].name[0],fun_name,10)){
				return  initial;
			}
			initial =  initial + (sym+init)->st_size;
			num++;
		}

	}
	printf("ERROR: without %s\n\r", fun_name);
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
				unsigned int value = 0;
				if(((int)comp_start + (rela+init)->r_offset) > (ksym[ksym_i-1].sym_addr & 0xffffffff)){
					value = 0x3ffffff -((((int)comp_start + (rela+init)->r_offset) - ksym[ksym_i-1].sym_addr)/4)+1;
				}else{
					value =((ksym[ksym_i-1].sym_addr - ((int)comp_start + (rela+init)->r_offset))/4);
				}
				
				unsigned int* bl_test = (comp_start + (rela+init)->r_offset);
				*bl_test = value + 0x94000000;
			}else{
				printf("Not componets support this function: %s\n\r",str_name);
				return -1;
			}

		   }else if(ndx==1){
			int st_value = get_st_value(base + move_sec[5].addr + 24*((rela+init)->r_info >> 32));
			unsigned int value=0;
			if(((int)comp_start + (rela+init)->r_offset) > ((int)comp_start + st_value)){
				value = 0x3ffffff -((((int)comp_start + (rela+init)->r_offset) - ((int)comp_start + st_value))/4)+1;
			}else{
				value = ((((int)comp_start + st_value) - ((int)comp_start + (rela+init)->r_offset))/4);

			}
			
			unsigned int* bl_test = (comp_start + (rela+init)->r_offset);
			*bl_test = value + 0x94000000;
		   }else{
			printf("%d\n\r",ndx);
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
	
	return header ->sh_name;
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

int config_compt(int* para){
	if(cfile[com_index].sym == NULL){
		printf("Error: Without registering.Could not config parameter.");
		return -1;
	}
	struct mm_info para_page = allocate_kernel_page(4096);
	memcpy((int*)para,(int*)para_page.start,4*((*para)+1));
	cfile[com_index].sym-> config_para = para_page.start;

	int* a = cfile[com_index].sym-> config_para;	
	return *para;
}

void ls_compt(void){
	printf("\n\r***************Components List***************\n\r");
	for(int compt_i = kapi_count; compt_i<128 && ksym[compt_i].sym_name[0]!='\0'; compt_i++){
		printf("%s\n\r",ksym[compt_i].sym_name);
	}
}

int exe_com(char* compt_name,void* para){
	int length = strlength(compt_name);
	for(int num = kapi_count; num<128; num++){
		if(!memcmp(&ksym[num].sym_name[0],compt_name,length)){
			ksym[num].sym_name[0] = '\0' ;
			bl_init( &_start_+ (unsigned int)ksym[num].sym_addr, para);
			//printf("Succeed to execute component!\n\r");				
			return 0;/*succeed*/
		}
	}
	printf("Fail to execute component:%s\n\r",compt_name);
	return 1;
}

int read_ksymbol(){
	unsigned int clust =0;
        char* base;
	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	   if(!strcmp(file_dir[k].name,"SYMBOL  TXT")){
	  	clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;	
		if(clust){			
			openfile* base = fat32_readfile(0, clust, &partition[0]);/*Putting ksymbol in first partition is regulation*/
			/*save kernel symbol*/
			unsigned int name_word=0,aa=0,base_index=0;
			char* name_addr =(char*)(&_start_+(unsigned int)base->log_addr);
			while(*(name_addr + base_index)!= 0x00){	
				if(*(name_addr + base_index) != 0xA){
					ksym[name_word].sym_name[aa++] = *(name_addr + base_index);
				}else{
					ksym[name_word].sym_addr = sys_call_table[name_word];
					//printf("%s %x\n\r", ksym[name_word].sym_name, ksym[name_word].sym_addr);
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

