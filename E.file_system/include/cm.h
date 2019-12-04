#ifndef	_CM_H
#define	_CM_H

#define INCOM 1
#define RMCOM 2
#define SWAP 3
#define Change_Sched 4
#define kapi_count 43
struct symbol_struct{
	unsigned char sym_name[32];
	unsigned long sym_addr;
        /*hardware*/
	void* config_para;
};

struct com_file{
	char filename[32];
	struct symbol_struct* sym;
	unsigned long rmcom;
};

struct hard_struct{
	int app_count;
	unsigned long* app_page;
};
#endif
