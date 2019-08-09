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
#include "fat16.h"
#include "fat32.h"
#include "mini_uart.h"
extern unsigned char _end;
extern fatdir_t *dir;
extern char directory[20];
fatdir_t *origin;

struct fs_unit* fs_type_support(int type){
	for(int type_index=0; fs_support[type_index].type != 0 && type_index<32;type_index++){
		if(fs_support[type_index].type == type){
			return &fs_support[type_index];}
	}
	return (struct fs_unit*)0;
}

void build_kernel_directory(void){
    char* tmp;
    char* pa_name; 
    int pa_in = 0;
    memzero(&(sd_p[0][0]),44);
    for(int pnum=0; pnum<4; pnum++){
	 switch(partition[pnum].type){
		case 0x0:
			break;
		case 0x0e:/*fat16*/
			pa_name = fat16_read_directory(0, &partition[pnum]);	
			//change space to EOF
			pa_in = 0;
			while(*(pa_name+pa_in)!=(char)8 && *(pa_name+pa_in)!='\0' && *(pa_name+pa_in)!= (char)32){
				sd_p[pnum][pa_in] = *(pa_name+pa_in);
				pa_in++;
			}
			sd_p[pnum][pa_in] = '\0';
			break;	
		case 0x0c:/*fat32*/
			pa_name = fat32_read_directory(0, &partition[pnum]);
			//change space to EOF
			pa_in = 0;
			while(*(pa_name+pa_in)!=(char)8 && *(pa_name+pa_in)!='\0' && *(pa_name+pa_in)!= (char)32){
				sd_p[pnum][pa_in] = *(pa_name+pa_in);
				pa_in++;
			}
			sd_p[pnum][pa_in] = '\0';
			break;		
		default:
		     	printf("Not support %x type in File system",partition[pnum].type);
			break;

       	 }
	 //change space to EOF
	 while(*(pa_name+pa_in)!=(char)8 && *(pa_name+pa_in)!='\0' && *(pa_name+pa_in)!= (char)32){
		sd_p[pnum][pa_in] = *(pa_name+pa_in);
		pa_in++;
	 }
	 sd_p[pnum][pa_in] = '\0';
    }
    memzero(file_dir,sizeof(struct user_fs)*20);
    for(int build_d = 0; build_d<4 && sd_p[build_d][0] != (char)8; build_d++){
       	 memcpy((&sd_p[build_d][0]),&(file_dir[build_d].name[0]), 8);
	 file_dir[build_d].attr[0] = (char) 16;
    }
    //memcpy(fat32_read_directory(partition[0]), (&sd_p[0][0]), 11);
    //printf("A:%s\n\r",&sd_p[0][0]);
}

void build_root(void){
	origin = dir;
}


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






