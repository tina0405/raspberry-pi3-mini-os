#include "printf.h"
#include "fs.h"
#include "sched.h"
#include "ipc.h"
typedef int size_t;
extern unsigned char _start_;
//extern unsigned char schedule;

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
	int rest =0, cur =0, ret_obj;
	if(real_addr-> _ptr + size*nobj - real_addr-> _base > buf){/*over buff*/
		cur = (real_addr-> _base + buf - real_addr-> _ptr);
		memcpy((char*)(real_addr->_ptr), ptr, cur); 
		rest = real_addr-> _ptr + size*nobj - real_addr-> _base - buf;
		real_addr-> _ptr = real_addr-> _ptr + cur;
		ret_obj = cur/size;
	}else{
		memcpy((char*)(real_addr->_ptr), ptr , size*nobj);
		/*over file size*/
		if(((int)real_addr->_ptr - (int)real_addr->_base) + real_addr->_cnt*buf >= real_addr->_fsize){
			ret_obj = (real_addr->_base + (real_addr->_fsize - real_addr->_cnt*buf) - real_addr->_ptr)/size;
			real_addr->_ptr = real_addr->_base + (real_addr->_fsize - real_addr->_cnt*buf);
			
		}else{
			real_addr-> _ptr = real_addr-> _ptr +size*nobj;
			ret_obj = nobj;
		}
		

	}
	/*Load next*/	
	if(rest){
		int next = real_addr->_cnt+1;
		send_msg(LOAD_BUF,thread_id_self(), FILESYS_MANAGER, real_addr, 4096);/*FM*/
		while(real_addr->_cnt != next){schedule();}
		memcpy((char*)(real_addr->_ptr), ptr , rest);
		/*over file size*/
		if(((int)real_addr->_ptr - (int)real_addr->_base) + real_addr->_cnt*buf >= real_addr->_fsize){
			ret_obj = (real_addr->_base + (real_addr->_fsize - real_addr->_cnt*buf) - real_addr->_ptr)/size;
		}else{
			real_addr-> _ptr = real_addr-> _ptr +rest;
			ret_obj = rest/size;
		}
		
	}
	
	arch_read_unlock(&real_addr->rw_lock);
	return ret_obj;
	
	
}




