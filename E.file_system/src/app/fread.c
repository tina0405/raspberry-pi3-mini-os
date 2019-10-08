#include "printf.h"
#include "fs.h"
typedef int size_t;
extern unsigned char _start_;
int fread(void *ptr, size_t size, size_t nobj, FILE *stream){
	/*if file size is allow*/
        FILE * real_addr = (&_start_ + (unsigned int)stream);
	int buf =(real_addr->_bufsize);
	int tmp = (size*nobj)/buf;
	
	if(!tmp){
		memcpy((char*)(real_addr->_base), ptr , size*nobj);
		return nobj;
	}else{

		memcpy((char*)(real_addr->_base), ptr , ((int)(buf/size))*size);
		return  (int)(buf/size);
	}

}
