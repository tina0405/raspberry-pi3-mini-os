#include<printf.h>
#include<cm.h>
#include<mm.h>
#include<str.h>
int exe_com(char* compt_name,void* para){
	int length = strlength(compt_name);
	char str_name[32] = {'\0'};
	int i = 0,flag=0;
	while(*(compt_name+i)!='\0'){
		str_name[i] = *(compt_name+i);
		i++;
	}

	for(int num = kapi_count; num<128; num++){
		int k_length = strlength(&ksym[num].ksym_name[0]);
		if(k_length){
			flag = 0;
			for(int a=0;a<k_length;a++){
				if(ksym[num].ksym_name[a]!=str_name[a]){
					flag = 1;					
					break;
		
				}

			}
			
		}
		if(!flag){
			int f_count = *((int*)ksym[num].opera_sym_addr);
			void* ptr = (int*)ksym[num].opera_sym_addr+1;
			for(int tmp_count=0; tmp_count<f_count; tmp_count++){
				if(str_name[k_length] == '_' && !memcmp(ptr, &str_name[k_length+1] ,length-k_length-1)){
					/*dependency*/
					/*successful*/
					//printf("%s  ",ptr);
					ptr = (char*)ptr +32;
					unsigned long rela_addr = *((unsigned long*)ptr);
					bl_compt(para, rela_addr);
					//printf("Succeed to execute component!\n\r");				
					return 0;/*succeed*/
									
				}
				ptr = (char*)ptr +40;
				int para_count = *(int*)ptr;
				ptr = (int*)ptr + (para_count+1);
					
			}
			
		}
	}
	printf("Fail to execute component:%s\n\r",compt_name);
	return 1;
}

