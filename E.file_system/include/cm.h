#ifndef	_CM_H
#define	_CM_H

#define INCOM 1
#define RMCOM 2
#define SWAP 3
#define Change_Sched 4
#define OPERATION 5
#define kapi_count 46
struct symbol_struct{
	unsigned char ksym_name[32];
	unsigned long sym_addr;
	unsigned long rm_addr;
	struct com_file* file;
	void* component_page;
	void* app_page;
	void* hardware;
        /*hardware*/
	void* config_para;
};

struct com_file{
	char filename[32];
	struct symbol_struct* sym;
	//unsigned long rmcom;
};

struct hard_struct{
	int app_count;
	unsigned long* app_page;
};
#endif
