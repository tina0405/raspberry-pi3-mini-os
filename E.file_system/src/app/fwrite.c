#include "printf.h"
#include "fs.h"
typedef int size_t;
//sdTransferBlocks (sect, 1, ptr, 1);
extern unsigned char _start_;

static inline void arch_write_lock(arch_rwlock_t* rw)
{
    unsigned int tmp;
    asm volatile(
    "2:    ldaxr    %w0, %1\n"
    "      cbnz     %w0, yield_cpu\n"
    "      stxr     %w0, %w2, %1\n"
    "      cbnz     %w0, 2b\n"
    : "=&r" (tmp), "+Q" (rw->lock)
    : "r" (0x80000000)
    : "memory");
     printf("w_lock:%x\n\r",rw->lock);
}

static inline void arch_write_unlock(arch_rwlock_t* rw)
{
	asm volatile(
	"	stlr	%w1, [%0]\n"
	: : "r" (&rw->lock), "r" (0) : "memory");
	printf("w_unlock:%x\n\r",rw->lock);
}


int fwrite(void *ptr, size_t size, size_t nobj, FILE *stream){
	/*know cluster size*/
	FILE * real_addr = (&_start_ + (unsigned int)stream);
	arch_write_lock(&real_addr->rw_lock);	
	
	int buf =(real_addr->_bufsize);
	int tmp = (size*nobj)/buf;
	
	if(!tmp){
		memcpy(ptr, (char*)(real_addr->_base), size*nobj);
		real_addr->_ptr= (char*)(real_addr->_base + size*nobj);
		arch_write_unlock(&real_addr->rw_lock);
		return nobj;
	}else{

		memcpy(ptr, (char*)(real_addr->_base) , ((int)(buf/size))*size);
		real_addr->_ptr= (char*)(real_addr->_base + size*nobj);
		arch_write_unlock(&real_addr->rw_lock);
		return  (int)(buf/size);
	}
	
}
