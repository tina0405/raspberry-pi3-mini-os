#ifndef	_CM_H
#define	_CM_H

#define INCOM 1
#define RMCOM 2
#define SWAP 3
#define Change_Sched 4
#define OPERATION 5

#define kapi_count 48
/*type*/
#define FS_COM 1
#define DRV_COM 2
#define SCH_COM 3
/*interface*/
#define USER_DEF 1
#define SCH_DEF 2
#define FS_GETCLUSTER_DEF 3
#define FS_READFILE_DEF 4
#define FS_READDIR_DEF 5
#define FS_WRITEBUF_DEF 6
#define FS_WRITEDIR_DEF 7
struct symbol_struct{
	unsigned char ksym_name[32];
	unsigned long sym_addr;
	void* opera_sym_addr;
	unsigned long rm_addr;
	struct com_file* file;
	void* use_compt_page;
	void* used_compt_page;
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
