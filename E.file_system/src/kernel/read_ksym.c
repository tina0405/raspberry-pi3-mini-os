#include<printf.h>
#include<fs.h>
#include<cm.h>
#include "fat32.h"
extern unsigned char _start_;
extern void * const sys_call_table[];

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
					ksym[name_word].ksym_name[aa++] = *(name_addr + base_index);
				}else{
					ksym[name_word].sym_addr = sys_call_table[name_word];
					//printf("%s %x\n\r", ksym[name_word].ksym_name, ksym[name_word].sym_addr);
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
