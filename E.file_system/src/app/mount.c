#include <printf.h>
#include <fs.h>
#include <SDCard.h>
extern int sd_exist;
void mount(void){
	int count = 3;      
	while(count){	
		if(sdInitCard (&printf, &printf, true) == SD_OK) {	
			sd_exist =1;
			break;
		}
		count--;
		
	}
}
