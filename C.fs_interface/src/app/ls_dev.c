#include <printf.h>
#include <fs.h>
extern int support_type[4];
void ls_dev(void){
	/*sd_card*/
	printf("\n\rdevice     type     support     mounted\n\r");
	for(int dev_num=0;dev_num < 4 ;dev_num++){
		if(partition[dev_num].partitionlba > 0){
			printf("sda%d       ",dev_num+1);
			switch(partition[dev_num].type){
				case 0xC:					
					printf("FAT32    ");
					printf("yes         ");	
					if(dev_num == 0){
						printf("root/%s(reserved by system)\n\r",&sd_p[dev_num][0]);
					}else{
						printf("root/%s\n\r",&sd_p[dev_num][0]);
					}
					break;
				case 0xE:
					printf("FAT16    ");
					if(fs_type_support(partition[dev_num].type)){
						printf("yes         ");
						printf("root/%s\n\r",&sd_p[dev_num][0]);
					}else{
						printf("no ");
						printf("         no\n\r");
					}
						
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
