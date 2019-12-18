#include "printf.h"
#include "fs.h"
extern unsigned char _start_;
int fseek(FILE *stream, long int offset, int whence){
	FILE * real_addr = (&_start_ + (unsigned int)stream);
	if(offset/real_addr->_bufsize==real_addr->_cnt){
		if(whence == SEEK_SET){
			real_addr->_ptr = (char*)(real_addr->_base) +offset - (offset/real_addr->_bufsize)*real_addr->_bufsize;
		}else if(whence == SEEK_CUR){
			real_addr->_ptr = (char*)(real_addr->_ptr) +offset - (offset/real_addr->_bufsize)*real_addr->_bufsize;
		}else if(whence == SEEK_END){
			/*to be continue*/
			real_addr->_ptr = (char*)(real_addr->_base) + real_addr->_bufsize + offset;
		}else{

			printf("Without support whence:%d\n\r",whence);
			return 1;/*fail*/
		}
	}else{
		printf("RELOAD BUF");
	}
	//printf("%d\n\r",real_addr->_ptr);
	
	return 0;/*succeed*/
}

