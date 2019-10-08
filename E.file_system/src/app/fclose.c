#include<fs.h>
#include<printf.h>
#include <stddef.h>
extern unsigned char _start_;
int fclose(FILE *stream){
		

	FILE * real_addr = (&_start_ + (unsigned int)stream);
	int num = 0; 
	if(real_addr->_bufsize%4096){
		num = (real_addr->_bufsize/4096)+1;	
	}else{
		num = real_addr->_bufsize/4096;
	}

	if(real_addr->_bufsize <= 512){
		/*send to file system*/
		sdTransferBlocks ((char*)symbolic_fs_array[real_addr->_tmpname].file_info->directory, 1, (unsigned long)real_addr->_base , 1);
	}else{
		printf("File buff > 512\n\r");
	}

	printf("close page:%x",num);
	free_page(real_addr->_base,num);
	symbolic_fs_array[real_addr->_tmpname].open = 0;/*soft symbolic*/ 
	symbolic_fs_array[real_addr->_tmpname].file_info->directory = NULL;
	stream = NULL;	
	return 0;
}
