#include "printf.h"
#include "fs.h"
typedef int size_t;
extern unsigned char _start_;
int fread(void *ptr, size_t size, size_t nobj, FILE *stream){
	/*if file size is allow*/	
	int buf =(((FILE *)((unsigned long)stream + &_start_))->_bufsize);
	int tmp = (size*nobj)/buf;
	
	if(!tmp){
		memcpy(((unsigned long)(((FILE *)((unsigned long)stream + &_start_))->_base) + &_start_), ptr , size*nobj);
		return nobj;
	}else{

		memcpy(((unsigned long)(((FILE *)((unsigned long)stream + &_start_))->_base) + &_start_), ptr , ((int)(buf/size))*size);
		return  (int)(buf/size);
	}

	
}
