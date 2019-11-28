#include "printf.h"
#include "fs.h"
typedef int size_t;
extern unsigned char _start_;
extern unsigned char schedule;

void yield_cpu(void){
    asm volatile(
    "bl schedule"
    );
}

static inline void arch_read_lock(arch_rwlock_t* rw)
{
    unsigned int tmp, tmp2;
    asm volatile(
    "2:    ldaxr    %w0, %2\n"
    "      add      %w0, %w0, #1\n"
    "      tbnz     %w0, #31, yield_cpu\n"
    "      stxr     %w1, %w0, %2\n"
    "      cbnz     %w1, 2b\n"
    : "=&r" (tmp), "=&r" (tmp2), "+Q" (rw->lock)
    :
    : "memory");
}

static inline void arch_read_unlock(arch_rwlock_t* rw)
{
    unsigned int tmp, tmp2;
    asm volatile(  
    "      ldxr    %w0, %2\n"
    "      sub     %w0, %w0, #1\n"
    "      stlxr   %w1, %w0, %2\n"
    "      cbnz    %w1, yield_cpu\n"
    : "=&r" (tmp), "=&r" (tmp2), "+Q" (rw->lock)
    :
    : "memory");
}


int fread(void *ptr, size_t size, size_t nobj, FILE *stream){
	/*if file size is allow*/
 	
        FILE * real_addr = (&_start_ + (unsigned int)stream);
	arch_read_lock(&real_addr->rw_lock);

	int buf =(real_addr->_bufsize);
	int tmp = (size*nobj)/buf;
	if(!tmp){
		memcpy((char*)(real_addr->_base), ptr , size*nobj);
		arch_read_unlock(&real_addr->rw_lock);
		return nobj;
	}else{

		memcpy((char*)(real_addr->_base), ptr , ((int)(buf/size))*size);
		arch_read_unlock(&real_addr->rw_lock);
		return  (int)(buf/size);
	}

	
}




