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
#include "str.h"
#include "mini_uart.h"
#include "reset_hardware.h"
int opera_addr = 0;
int rmcom_addr = 0;
char* comp_start=0;
extern int cd_rem;
extern unsigned char _start_;


struct text_func{
	char name[16];
	int size;
};


struct text_func text_function[32];
extern unsigned char _end;
struct com_file cfile[64];/*Component Table*/
struct symbol_struct ksym[128];/*Kernel Table*/
struct hard_struct hardware_table[40];


int com_index=0;
struct com_file* current_file;


struct mod_section move_sec[7];/*0.text 1.rodata 2.data 3.bss 4.rela.text 5.symtab 6.strtab*/
unsigned long base = 0;
char compt_filename[8];
int compt_file(char* file_name){/*incom*/
	unsigned int clust = 0;
	volatile openfile* basefile;
        base = 0;
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
			current_file = &cfile[com_index];
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
				//opera_addr = use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"oprt_compt");
				//rmcom_addr = use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"exit_compt");
				/*clear component table*/
				//cfile[com_index].rmcom = rmcom_addr + comp_start;
				current_file->sym = NULL;			
				if(init_addr<0){
					printf("Without init_comp function!");
				}else if(rmcom_addr<0){
					printf("Without exit_comp function!");
				}else{
					printf("%x\n\r",comp_start+init_addr);
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


int rm_compt_file(char* compt_name){/*improve: component name*/ /*remove*/


        int length = strlength(compt_name);
	for(int num = kapi_count; num<128; num++){
		if(!memcmp(&ksym[num].ksym_name[0],compt_name,length)){
			printf("\n\r----------------------Component exit----------------------\n\r");
			bl_init( &_start_+ (unsigned int)ksym[num].rm_addr, 0);
			/*check unreg*/
			if(ksym[num].ksym_name[0]!='\0'){
				printf("\n\rThere are apps and compoenets using %s\n\r",compt_name);
				printf("System advices adding unreg_compt(char* compt_name) in exit_compt() function \n\ror forcing to remove component:[rmcom -f compt_name]");
				return 1; 
			}
			
			memzero(ksym[num].file->filename,8);
			free_page(ksym[num].rm_addr,1);
			memzero(&(ksym[num].ksym_name[0]),32);
			free_page(ksym[num].sym_addr,1);
			return 0;/*succeed*/
		}
	}
	printf("\n\rComponent is not exist:%s", compt_name);
	return 1;

}
int check_config = 0;
int unreg_compt(char* compt_name){/*remove app*/
	int length = strlength(compt_name);
	if(check_config){
		for(int num = kapi_count; num<128; num++){
			if(!memcmp(&ksym[num].ksym_name[9],compt_name,length)){

				printf("Swap operation:Succeed to unregister!\n\r");				
				
				int hard_count = ((int*)ksym[num].file->hardware)[0];
				printf("h:%d %d %x\n\r",hard_count, ((int*)ksym[num].file->hardware)[1],(int*)ksym[num].file->hardware);
				for(int i=0;i<hard_count;i++){
					int addr = ((int*)ksym[num].file->hardware)[1+i*2];
					printf("htable:%d\n\r",addr);
					clear_gpio(addr);
					
		
				}
				return 0;/*succeed*/
			}
		}
		printf("kservice_%s was not in symbol table!",compt_name);
		return 1;/**/
	}
	else{
		for(int num = kapi_count; num<128; num++){
			if(!memcmp(&ksym[num].ksym_name[9],compt_name,length)){
				ksym[num].ksym_name[0] = '\0' ;
				printf("Succeed to unregister symbol!\n\r");				
				
				int hard_count = ((int*)ksym[num].file->hardware)[0];
				printf("h:%d\n\r",hard_count);
				for(int i=0;i<hard_count;i++){
					int addr = ((int*)ksym[num].file->hardware)[1+hard_count*2];
					printf("htable:%d\n\r",addr);
					clear_gpio(addr);
					
		
				}
				return 0;/*succeed*/
			}
		}
		printf("kservice_%s was not in symbol table!",compt_name);
		return 1;/**/
	}

}


struct symbol_struct sched_sym;
int reg_compt(char* compt_name, int type, void* para){/*return num*/
	int ksym_i = 9,compt_i= 0, fun_type = 0;
	unsigned long fun_base = 0, tmp_sched = 0;
	switch(type){
		case DRV_COM:
			if(check_config){/*swap*/
				if(compt_name != NULL){
					printf("Without SWAP Operation!");
					check_config = 2;/*not compare*/
					return -1;
				}
				void* old_para = current_file->config_para;				
				if(*((int*)old_para) != *((int*)para)){
					check_config = 2;/*not compare*/
					return -1;
				}
				for(int i=0; i<*((int*)old_para); i++){
					if(memcmp(((int*)old_para+1), ((int*)para+1) ,32)){
						printf("Function name do not compare:old->%s, new->%s",((int*)old_para+1) ,((int*)para+1));
						check_config = 2;
						return -1;
					}else{
						*((int*)old_para+1+8) = comp_start + use_compt_func(base, (char*)(base + move_sec[5].addr), move_sec[5].size, (char*)((int*)old_para+1));                       
					}
					if(*((int*)old_para+1+8+2) != *((int*)para+1+8+1)){
						printf("parameter's count is not compare:old->%d, new->%d",*((int*)old_para+1+8+2), *((int*)para+1+8+1));
						check_config = 2;/*not compare*/
						return -1;
					}		
					for(int a=0; a<*((int*)old_para+1+8+2+1); a++){
						if(*((int*)old_para+1+8+2+1+a) != *((int*)para+1+8+1+1+a)){
							printf("parameter's size is not compare");
							check_config = 2;/*not compare*/
							return -1;
						}
					}
					old_para = ((int*)old_para+1+8+2+1+*((int*)old_para+1+8+2+1));
					para = ((int*)para+1+8+1+1+*((int*)para+1+8+1+1));
				}

				return 0;
			}else{
				if(!compt_name){
					printf("Cannot register! Need_name!");
					return 1;/*fail*/
				}
				int length = strlength(compt_name);
				for(int num = kapi_count; num<128; num++){
					if(!memcmp(&ksym[num].ksym_name[9],compt_name,length)){
						printf("Cannot register! kservice_%s has existed!",compt_name);
						return 1;/*fail*/
					}			
				}
				for(int num = kapi_count; num<128; num++){
					if(ksym[num].ksym_name[0] == '\0'){
						
						void * ptr = para;
						int function_count = *((int*)ptr);
						
						printf("function count:%d\n\r",function_count);
						ptr = ((int*)ptr)+1;
						struct mm_info op_page = allocate_kernel_page(4096);/*operation table*/
						current_file-> config_para = op_page.start;
						*((int*)op_page.start) = function_count;	
						void* page_ptr = (char*)op_page.start+4;				
						/*
						struct para_config{
							int op_func;
							int interface;
							char name[32];
							int pnum;
							int para_1;
							int para_2;

							int interface2;
							char name2[32];
							int pnum2;
							int para2_1;
							...
						};

						*/
						/*operation table
						|++++++++++++|	----->  offset
						|function num|	-----> 	int -> 4 bytes
						|name	     |	----->	char*32 -> 32 bytes 
						|addr	     |	----->	unsigned long -> 8 bytes
						|para num    |	----->	int -> 4 byte
						|para1...    |	----->	int*para_num -> 4*para_num bytes
						...						
						|++++++++++++|
						*/

						for(int func_check = 0; func_check < function_count; func_check++){
							fun_type = *((int*)ptr);					
							ptr = ((int*)ptr) + 1;
							memcpy((char*)ptr, (char*)page_ptr, 32);
							page_ptr=(unsigned long*)page_ptr + 4;
							fun_base = use_compt_func(base, (char*)(base + move_sec[5].addr), move_sec[5].size, (char*)ptr);
							if(fun_base<=0){
								free_page(op_page.start,1);
								return 1;
							}
							*((unsigned long*)page_ptr) = comp_start + fun_base;
							if(fun_type != USER_DEF){					
								printf("Not user definition");
								return 1;
							}
				
							ptr = ((char*)ptr) + 32;
							page_ptr = ((char*)page_ptr) + 8;
							int para_num =*((int*)ptr);
							memcpy((char*)ptr, (char*)page_ptr, 4*(para_num+1));
							ptr = ((char*)ptr)+4*(para_num+1);
							page_ptr = ((char*)page_ptr)+4*(para_num+1);					
												
						}
						memcpy("kservice_",&ksym[num].ksym_name[0],9);
						while(*(compt_name + compt_i)!= '\0'){
							ksym[num].ksym_name[ksym_i++] = *(compt_name + (compt_i++));	
						}
						ksym[num].opera_sym_addr = op_page.start;
						ksym[num].rm_addr = comp_start + use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"exit_compt");
						printf("Register component function: %s\n\r",ksym[num].ksym_name);
						current_file->sym = &ksym[num];
						ksym[num].file = current_file;
			
						return 0;
				
					}

				}
				printf("Cannot register! Symbol table is full!");
				return 1;/**/
			}
			break;
		case SCH_COM:
			if(check_config){/*swap*/
				return 0;
			}else{
				void * ptr = para;
				int function_count = *((int*)ptr);	
				printf("function count:%d\n\r",function_count);
				ptr = ((int*)ptr)+1;
				struct mm_info op_page = allocate_kernel_page(4096);/*operation table*/
				current_file-> config_para = op_page.start;
				*((int*)op_page.start) = function_count;	
				void* page_ptr = (char*)op_page.start+4;				
				for(int func_check = 0; func_check < function_count; func_check++){
					fun_type = *((int*)ptr);					
					ptr = ((int*)ptr) + 1;
					memcpy((char*)ptr, (char*)page_ptr, 32);
					page_ptr=(unsigned long*)page_ptr + 4;
					fun_base = use_compt_func(base, (char*)(base + move_sec[5].addr), move_sec[5].size, (char*)ptr);
					if(fun_base<=0){
						free_page(op_page.start,1);
						return 1;
					}
					*((unsigned long*)page_ptr) = comp_start + fun_base;
					if(fun_type == USER_DEF){}
					else if(fun_type == SCH_DEF){

						sched_type =  fun_base + comp_start;
					}else{
						free_page(op_page.start,1);
						return 1;
					}			
					ptr = ((char*)ptr) + 32;
					page_ptr = ((char*)page_ptr) + 8;
					int para_num =*((int*)ptr);
					memcpy((char*)ptr, (char*)page_ptr, 4*(para_num+1));
					ptr = ((char*)ptr)+4*(para_num+1);
					page_ptr = ((char*)page_ptr)+4*(para_num+1);					
												
				}
				if(sched_sym.opera_sym_addr!=NULL){
					/*clear sched*/
				}
				sched_sym.opera_sym_addr = op_page.start;
				sched_sym.rm_addr = comp_start + use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"exit_compt");	
				int index_s = 0;
				while(*(compt_name + index_s)!= '\0'){
					sched_sym.ksym_name[index_s] = *(compt_name + (index_s));
					index_s++;	
				}
				sched_sym.ksym_name[index_s] = '\0';
				printf("Register component function: %s\n\r",sched_sym.ksym_name);
				current_file->sym = &sched_sym;
				sched_sym.file = current_file;
				return 0;
				
			}
			
			break;
		default:
			break;
	}
	
}

extern struct pcb_struct *thread_id_table[4096];/*tid and pcb map*/
int hardware_request(unsigned int address){
	
	/*address exist?*/
	if(hardware_table[address].app_count != 0){
		
		if(hardware_table[address].app_count == 1 && *(hardware_table[address].app_page) == thread_id_self())
		{return 0;}
		//printf("GPIO %x is used by applications:",address);
		for(int i = 0; i<hardware_table[address].app_count; i++){
			//printf("pcb number: %x\n\r",*(hardware_table[address].app_page+i));
		}
		return 1;/*fail*/
	}
	/*	component use hardware table
		|++++++++++++++++|
		| hardware num   | ---> int 
		| address        | ---> int
		| use address num| ---> int
		...

	*/
	if(current_file->hardware==NULL){
		struct mm_info h_page = allocate_kernel_page(4096);
		current_file->hardware = h_page.start;
		((int*)current_file->hardware)[0] = 1;
		((int*)current_file->hardware)[1] = address; 
		printf("ha:%x %d\n\r",current_file->hardware,((int*)current_file->hardware)[1]);
		((int*)current_file->hardware)[2] = 1; 	
	}else{
		printf("h:%d\n\r",((int*)current_file->hardware)[1]);
		int tmp_count = ((int*)current_file->hardware)[0];
		((int*)current_file->hardware)[0] = tmp_count+1;
		((int*)current_file->hardware)[1+tmp_count*2] = address; 
		((int*)current_file->hardware)[2+tmp_count*2] = 1; 
				
	}

	if(hardware_table[address].app_page==NULL){/*first be used*/	
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
	
	//printf("\n\rTid:%x pcb:%x\n\r",thread_id_self(),thread_id_table[thread_id_self()]);
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
			//printf("%s %s %d\n\r",&text_function[num].name[0],fun_name,strlength(fun_name));
			if(!memcmp(&text_function[num].name[0],fun_name,strlength(fun_name))){
				memzero(&text_function[num].name[0],text_function[num].size);
				return initial;
			}
			memzero(&text_function[num].name[0],text_function[num].size);
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

		if((unsigned int)(rela+init)->r_info==0x113){
	
		}
		else if((unsigned int)(rela+init)->r_info == 0x115){
		  
		  int ndx = get_ndx(base + move_sec[5].addr + 24*((rela+init)->r_info >> 32));
	          int rel_num =	find_sec_addr(base + section_table_start + ndx*(section_size));
		  unsigned int* ch_test = (comp_start + (rela+init)->r_offset);
		
		  if(rel_num == 1){ /*rodata*/
			 *ch_test = (((move_sec[0].size + (rela+init)->r_addend)*4)<<8) + (*ch_test);
		  }else if(rel_num==2){ /*data*/
			 *ch_test = (((move_sec[0].size + move_sec[1].size + (rela+init)->r_addend)*4)<<8)+(*ch_test);
			 
		  }else if(rel_num==3){ /*bss*/
			 *ch_test = (((move_sec[0].size + move_sec[1].size + move_sec[2].size + (rela+init)->r_addend)*4)<<8)+(*ch_test);
                  }else{
			  printf("Not data section!");
		  }
		}else if((unsigned int)(rela+init)->r_info==0x11b){
		   int ndx = get_ndx(base + move_sec[5].addr + 24*((rela+init)->r_info >> 32));/*hi->symbol name*/
			/*0.text 1.rodata 2.data 3.bss 4.rela.text 5.symtab 6.strtab*/
		   
		   if(ndx==0){
			int strname = get_strname(base + move_sec[5].addr + 24*((rela+init)->r_info >> 32));
			
			int i = 0, ksym_i = 0, flag =0;
			unsigned long rela_addr = 0;
		   	char str_name[30]={'\0'};
			char* chara =base + move_sec[6].addr+ (int)strname;
			while(*(chara+i)!='\0'){
				str_name[i] = *(chara+i);
				i++;
			}
			struct mm_info com_page;
			
			while(ksym[ksym_i].ksym_name[0]!='\0'&& ksym_i<128){
				/*bug*/
				int index_k=0;
				while(ksym[ksym_i].ksym_name[index_k]!='\0'){
					index_k++;
				}
				if(!memcmp(&ksym[ksym_i], &str_name[0] ,index_k-1)){
					if(ksym[ksym_i].opera_sym_addr==NULL){
						/*kapi*/
						/*successful*/
						if(index_k == i){
							rela_addr = ksym[ksym_i].sym_addr;
							flag = 1; 
							break;
						}
						
					}else{
						/*insert comp*/
						/*use table
						|------------|
						|use comp num|unsigned long ->8bytes
						|ksym pointer|unsigned long -> 8bytes 
						*/
						int f_count = *((int*)ksym[ksym_i].opera_sym_addr);
						void* ptr = (int*)ksym[ksym_i].opera_sym_addr+1;
						for(int tmp_count=0; tmp_count<f_count; tmp_count++){
							printf("%s %s",ptr,&str_name[index_k]);
							if(str_name[index_k] == '_' && !memcmp(ptr, &str_name[index_k+1] ,i-index_k-1)){								/*dependency*/
								/*successful ksym[ksym_i] -> system's com | str_name -> relocation */

								if(current_file->use_compt_page == NULL){
									struct mm_info com_p = allocate_kernel_page(4096);
									current_file->use_compt_page = com_p.start;
									((unsigned long*)current_file->use_compt_page)[0] = 1;
									((unsigned long*)current_file->use_compt_page)[1] = (unsigned long)ksym[ksym_i].file;							/*here*/
								}


								unsigned long tmp_k;
								unsigned long tmp_c = ((unsigned long*)current_file->use_compt_page)[0];
								if(ksym[ksym_i].file->use_compt_page == NULL){
									tmp_k = 0;						
								}else{
									tmp_k = ((unsigned long*)ksym[ksym_i].file->use_compt_page)[0];
								}
								((unsigned long*)current_file->use_compt_page)[0] = tmp_c + tmp_k;
								
								for(int t_count = 0; t_count < tmp_k; t_count++){
									((unsigned long*)current_file->use_compt_page)[tmp_c+1+t_count] = ((unsigned long*)ksym[ksym_i].file->use_compt_page)[t_count+1];
									struct com_file* tmp_file = (struct com_file*)((unsigned long*)ksym[ksym_i].file->use_compt_page)[t_count+1];


									if(tmp_file->used_compt_page==NULL){
										com_page = allocate_kernel_page(4096);
										((unsigned long*)com_page.start)[0] = 1; 
										((unsigned long*)com_page.start)[1] = (unsigned long)current_file;
										tmp_file->used_compt_page = com_page.start;
									
									}else{
										unsigned long com_dep = ((unsigned long*)tmp_file->used_compt_page)[0];
										((unsigned long*)tmp_file->used_compt_page)[com_dep + 1] = (unsigned long)current_file;
										((unsigned long*)tmp_file->used_compt_page)[0] = com_dep+1;
									}

										/**/
								}


								/*used component is used by inserted component*/
								if(ksym[ksym_i].file->used_compt_page==NULL){
									com_page = allocate_kernel_page(4096);
									((unsigned long*)com_page.start)[0] = 1; 
									((unsigned long*)com_page.start)[1] = (unsigned long)(current_file);
									ksym[ksym_i].file->used_compt_page = com_page.start;
									/*here*/
									
								}else{
									unsigned long com_dep = ((unsigned long*)ksym[ksym_i].file->used_compt_page)[0];
									((unsigned long*)ksym[ksym_i].file->used_compt_page)[com_dep + 1] = (unsigned long)current_file;
									((unsigned long*)ksym[ksym_i].file->used_compt_page)[0] = com_dep+1;
								}			

								ptr = (char*)ptr +32;
								rela_addr = *((unsigned long*)ptr);
								flag = 1; 
								break;
									
							}
							ptr = (char*)ptr +40;
							int para_count = *(int*)ptr;
							ptr = (int*)ptr + (para_count+1);
						}


						
						
					}

				}
				
					
					
				
				ksym_i++;
			}

			if(flag==1){
				unsigned int value = 0;
				if(((int)comp_start + (rela+init)->r_offset) > (rela_addr & 0xffffffff)){
					value = 0x3ffffff -((((int)comp_start + (rela+init)->r_offset) - rela_addr)/4)+1;
				}else{
					value =((rela_addr - ((int)comp_start + (rela+init)->r_offset))/4);
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



int swap_compt(char* new_compt,char* old_compt){
	char filename[12]={'\0'};
	char compt_name[32]={'\0'};
	memcpy(new_compt,filename,11);
	memcpy(old_compt,compt_name,32);
	int swap_num = 129;

	int length = strlength(compt_name);
	for(int num = kapi_count; num<128; num++){
		if(!memcmp(&ksym[num].ksym_name[0],compt_name,length)){
			swap_num = num;
			//bl_init( &_start_+ (unsigned int)ksym[num].sym_addr, para);
			//printf("Succeed to execute component!\n\r");				

		}
	}		
	if(swap_num == 129){
		printf("Not in component table:%s\n\r",compt_name);
		return 1;
	}	
	
	unsigned int clust = 0;
	volatile openfile* basefile;
        base = 0;
        struct dev* dev_param;
	struct fs_unit* return_fs;
	

	for(int k = 0;file_dir[k].name[0]!='\0';k++){
	 
	   if(!memcmp(file_dir[k].name,filename,8)){
		clust =((unsigned int)file_dir[k].ch)<<16|file_dir[k].cl;
		if(clust){

			return_fs = fs_type_support(partition[cd_rem].type);
                        dev_param = &partition[cd_rem];
               		if(return_fs){
				basefile = bl_init( &_start_+ (unsigned int)return_fs->addr_readfile, clust, dev_param);
				base = (unsigned long)basefile->log_addr;
			}else{
				printf("Not support %x type in File system",partition[cd_rem].type);
				return 0;
			}
			
			/*remove*/

			
			/*init file*/
			unsigned long size_u = file_dir[k].size;
			current_file = ksym[swap_num].file;
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
				//opera_addr = use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"oprt_compt");
				rmcom_addr = use_compt_func(base,(char *)(base + move_sec[5].addr),move_sec[5].size,"exit_compt");
				/*clear component table*/
				//cfile[com_index].rmcom = rmcom_addr + comp_start;
			
				if(init_addr<0){
					printf("Without init_comp function!");
				}else if(rmcom_addr<0){
					printf("Without exit_comp function!");
				}else{
					check_config = 1;/*swap*/
					bl_init(comp_start+init_addr,0);
					if(check_config == 2){
						printf("Swap fail! Parameter is not compare.");
						check_config = 0;
						return 1;
					}
					
					/*without*/
				}
			}
			

			printf("\n\r----------------------Component exit----------------------\n\r");
			bl_init( &_start_+ (unsigned int)ksym[swap_num].rm_addr, 0);
			check_config = 0;
			
			
			memcpy(filename,ksym[swap_num].file->filename,11);
			free_page(ksym[swap_num].rm_addr,1);
			//memzero(&(ksym[swap_num].ksym_name[0]),32);
			free_page(ksym[swap_num].sym_addr,1);
			//current_file->sym->sym_addr = comp_start + opera_addr;/**/
		        current_file->sym->rm_addr = comp_start + rmcom_addr;
			//copy_process(SERVER_THREAD, (char *)comp_start, 0, 0);
			
						

		}
		else{
			printf("\n\rNot file:%s", filename);
			return 1;		
		}
		return 0;
	   }
	   

        }

	printf("\n\rNot file:%s",filename);
	return 1;
}



