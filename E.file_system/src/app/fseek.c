#include "printf.h"
#include "fs.h"
extern unsigned char _start_;
int fseek(FILE *stream, long int offset, int whence){
	FILE * real_addr = (&_start_ + (unsigned int)stream);
	
	if(whence == SEEK_SET){
		real_addr->_ptr = (char*)(real_addr->_base) +offset;
	}else if(whence == SEEK_CUR){
		real_addr->_ptr = (char*)(real_addr->_ptr) +offset;
	}else if(whence == SEEK_END){
		/*to be continue*/
		real_addr->_ptr = (char*)(real_addr->_base) + real_addr->_bufsize + offset;
	}else{

		printf("Without support whence:%d\n\r",whence);
		return 1;/*fail*/
	}
	//printf("%d\n\r",real_addr->_ptr);
	
	return 0;/*succeed*/
}

