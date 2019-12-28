#include<printf.h>
#include<cm.h>
#include <stddef.h>

void ls_compt(void){
	printf("\n\r***************Components List***************\n\r");
	for(int compt_i = kapi_count; compt_i<128 && ksym[compt_i].ksym_name[0]!='\0'; compt_i++){
		int f_count = *((int*)ksym[compt_i].opera_sym_addr);
		void* ptr = (int*)ksym[compt_i].opera_sym_addr+1;
		printf("Component name: %s\n\r",ksym[compt_i].ksym_name);
		
		struct com_file* tmp_sym = NULL;
		unsigned long u_count = 0;
		if(ksym[compt_i].file->use_compt_page!=NULL){
			printf("	*Use component:");
			u_count = *((unsigned long*)ksym[compt_i].file->use_compt_page);
			for(int tmp_count=0; tmp_count<u_count; tmp_count++){
				tmp_sym = ((unsigned long*)ksym[compt_i].file->use_compt_page)[tmp_count+1];
				printf("%s ",tmp_sym->sym->ksym_name);
			}
			printf("\n\r");
		}
		if(ksym[compt_i].file->used_compt_page!=NULL){
			printf("	*Used component:");
			u_count = *((unsigned long*)ksym[compt_i].file->used_compt_page);
			for(int tmp_count=0; tmp_count < u_count; tmp_count++){
				tmp_sym = ((unsigned long*)ksym[compt_i].file->used_compt_page)[tmp_count+1];
				printf("%s ",tmp_sym->sym->ksym_name);
			}
			printf("\n\r");
		}
		for(int tmp_count=0; tmp_count<f_count; tmp_count++){
			printf("	*Operation function %d:%s",tmp_count+1,ksym[compt_i].ksym_name);
			printf("_%s  ",ptr);
			ptr = (char*)ptr + 40;
			int para_count = *(int*)ptr;
			printf("\n\r		para_count:%d  ",para_count);
			for(int a=1; a<para_count+1; a++){
				printf("p%d:%d bytes  ",a,*((int*)ptr+a));

			}
			printf("\n\r");
			ptr = (int*)ptr + (para_count+1);
		}
	}
}
