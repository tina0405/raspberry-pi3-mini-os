#include<fs.h>
#include<printf.h>
#include <stddef.h>
extern unsigned char _start_;

int ftell(FILE *stream){
	FILE * real_addr = (&_start_ + (unsigned int)stream);
	int ret = ((int)real_addr->_ptr - (int)real_addr->_base);
	return ret;

}
