#include "printf.h"
#include "fs.h"
#include "ipc.h"
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
}

static inline void arch_write_unlock(arch_rwlock_t* rw)
{
	asm volatile(
	"	stlr	%w1, [%0]\n"
	: : "r" (&rw->lock), "r" (0) : "memory");
}


int fwrite(void *ptr, size_t size, size_t nobj, FILE *stream){
	/*know cluster size*/
	FILE * real_addr = (&_start_ + (unsigned int)stream);
	arch_write_lock(&real_addr->rw_lock);	
	/*
	int buf =(real_addr->_bufsize);
	int tmp = (size*nobj)/buf;
	
	if(!tmp){
		memcpy(ptr, (char*)(real_addr->_ptr), size*nobj);
		real_addr->_ptr= (char*)(real_addr->_ptr + size*nobj);
		arch_write_unlock(&real_addr->rw_lock);
		return nobj;
	}else{

		memcpy(ptr, (char*)(real_addr->_ptr) , ((int)(buf/size))*size);
		real_addr->_ptr= (char*)(real_addr->_ptr + ((int)(buf/size))*size);
		arch_write_unlock(&real_addr->rw_lock);
		return  (int)(buf/size);
	}
	*/

	int buf =(real_addr->_bufsize);
	int rest =0, cur =0, ret_obj;
	if(real_addr-> _ptr + size*nobj - real_addr-> _base > buf){/*over buff*/
		cur = (real_addr-> _base + buf - real_addr-> _ptr);
		memcpy(ptr, (char*)(real_addr->_ptr), cur); 
		rest = real_addr-> _ptr + size*nobj - real_addr-> _base - buf;
		real_addr-> _ptr = real_addr-> _ptr + cur;
		ret_obj = cur/size;
	}else{
			memcpy(ptr, (char*)(real_addr->_ptr), size*nobj);
			real_addr-> _ptr = real_addr-> _ptr +size*nobj;
			ret_obj = nobj;
	}
	/*Load next*/	
	if(rest){
		int next = real_addr->_cnt+1;
		send_msg(LOAD_WBUF,thread_id_self(), FILESYS_MANAGER, real_addr, 4096);/*FM*/
		while(real_addr->_cnt != next){schedule();}
		
		memcpy(ptr, (char*)(real_addr->_ptr), rest);
		real_addr-> _ptr = real_addr-> _ptr +rest;
		ret_obj = (rest+cur)/size;

		
	}

	arch_write_unlock(&real_addr->rw_lock);
	return ret_obj;
	
}
