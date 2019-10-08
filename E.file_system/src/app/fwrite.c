#include "printf.h"
#include "fs.h"
typedef int size_t;
//sdTransferBlocks (sect, 1, ptr, 1);
extern unsigned char _start_;
int fwrite(void *ptr, size_t size, size_t nobj, FILE *stream){
/*know cluster size*/
	FILE * real_addr = (&_start_ + (unsigned int)stream);
	int buf =(real_addr->_bufsize);
	int tmp = (size*nobj)/buf;
	
	if(!tmp){
		memcpy(ptr, (char*)(real_addr->_base), size*nobj);
		return nobj;
	}else{

		memcpy(ptr, (char*)(real_addr->_base) , ((int)(buf/size))*size);
		return  (int)(buf/size);
	}
	
}
