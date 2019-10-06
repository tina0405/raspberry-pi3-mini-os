#include "printf.h"
#include "fs.h"
typedef int size_t;
//sdTransferBlocks (sect, 1, ptr, 1);
extern unsigned char _start_;
int fwrite(void *ptr, size_t size, size_t nobj, FILE *stream){
/*know cluster size*/
	//
	int buf =(((FILE *)((unsigned long)stream + &_start_))->_bufsize);
	int tmp = (size*nobj)/buf;
	
	if(!tmp){
		memcpy(ptr,((unsigned long)(((FILE *)((unsigned long)stream + &_start_))->_base) + &_start_) , size*nobj);
		return nobj;
	}else{

		memcpy(ptr,((unsigned long)(((FILE *)((unsigned long)stream + &_start_))->_base) + &_start_), ptr , ((int)(buf/size))*size);
		return  (int)(buf/size);
	}
}
