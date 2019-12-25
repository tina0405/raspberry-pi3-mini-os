#include<printf.h>
#include<cm.h>

void ls_compt(void){
	printf("\n\r***************Components List***************\n\r");
	for(int compt_i = kapi_count; compt_i<128 && ksym[compt_i].ksym_name[0]!='\0'; compt_i++){
		int f_count = *((int*)ksym[compt_i].opera_sym_addr);
		void* ptr = (int*)ksym[compt_i].opera_sym_addr+1;
		printf("Component name: %s\n\r",ksym[compt_i].ksym_name);
		for(int tmp_count=0; tmp_count<f_count; tmp_count++){
			printf("	Operation_function %d:%s",tmp_count+1,ksym[compt_i].ksym_name);
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
