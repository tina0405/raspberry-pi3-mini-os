#include<fs.h>
#include<printf.h>
extern unsigned char _start_;
int fclose(FILE *stream){
	uint32_t real = (((FILE *)((unsigned long)stream + &_start_))->_real.real_addr);
	char* buf_start = (((FILE *)((unsigned long)stream + &_start_))->_base);
	int next_cluster = (((FILE *)((unsigned long)stream + &_start_))->_real.next_cluster);
	printf("%x\n\r",real);
	if(!next_cluster){
		sdTransferBlocks (real, 1, (&_start_+(unsigned long)buf_start) , 1);
		return 0;
	}
	return 1;
}
