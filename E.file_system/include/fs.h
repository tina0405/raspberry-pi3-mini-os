#ifndef	_FS_H
#define	_FS_H
#include <stdint.h>
#include <elf.h>
#define FOPEN 1
#define FCLOSE 2
#define FFLUSH 3
void list(void);
void build_kernel_directory(void);
void build_root(void);
void ls_dev(void);
int unreg_compt(char* compt_name);
int reg_compt(char* compt_name);
int rm_compt_file(char* file_name);
int compt_file(char* file_name);
int run_file(char* file_name);
int dump(char* file_name);
int cd(char* file_name);
void cd_root(void);
struct fs_unit* fs_type_support(int type);
extern struct thread_mutex fs_lock;
extern unsigned int current_page;
char sd_p[4][11]; 

typedef struct {
    volatile unsigned int lock;
} arch_rwlock_t;

struct pos{
	uint32_t real_addr;
	int next_cluster;
};
struct File{
   char* _ptr;/*next pos*/
   char* _base;
   int   _bufsize;
   int _tmpname; 
   arch_rwlock_t rw_lock;
};
typedef struct File FILE;
// directory entry structure
typedef struct {
    char            name[8];
    char            ext[3];
    char            attr[9];
    unsigned short  ch;
    unsigned int    attr2;
    unsigned short  cl;
    unsigned int    size;
} __attribute__((packed)) fatdir_t;

typedef struct {/*save struct*/
    char* log_addr;
    unsigned long* phy_addr;
} openfile;

struct file{/*save struct*/
    fatdir_t dir_record;
    openfile addr;/*dir in sd Card*/
    unsigned int num_fatdir;/*offset*/ 
    unsigned int directory;/*file: record buff*/
    int dev;
};

typedef struct {
    struct file* file_info;
    unsigned int open;//open:phy unopen:0 dev:-1
    char tmp_name[32];
} symbolic_node;

// the BIOS Parameter Block (in Volume Boot Record)
typedef struct {
    char            jmp[3];
    char            oem[8];
    unsigned char   bps0;
    unsigned char   bps1;
    unsigned char   spc;/*sector per cluster*/
    unsigned short  rsc;
    unsigned char   nf;/*number of FAT:2(now)*/
    unsigned char   nr0;
    unsigned char   nr1;
    unsigned short  ts16;
    unsigned char   media;
    unsigned short  spf16;
    unsigned short  spt;/*sector per fat*/
    unsigned short  nh;
    unsigned int    hs;
    unsigned int    ts32;
    unsigned int    spf32;/*sector per fat*/
    unsigned int    flg;
    unsigned int    rc;
    char            vol[6];
    char            fst[8]; 
    char            dmy[20];
    char            fst2[8];
} __attribute__((packed)) bpb_t;

struct mod_section{
	unsigned long num;
	unsigned long addr;
	int size;
};

struct dev{
	unsigned int dbr;
	unsigned int partitionlba;
	int type;
	int dev_type; /*nope=0, sd=1*/
	unsigned int fat_table_start;
	unsigned int empty[16];
	openfile directory_addr;
	unsigned int op_dir;
	//unsigned int fat_table_end;	
}; 
struct user_fs{/*for user dir*/
    char           name[11];
    char            attr[9];
    unsigned int    size;
    unsigned short  ch;
    unsigned short  cl;
    //struct user_fs* folder;
};
struct fs_unit{
    int type;
    unsigned int addr_directory;
    unsigned int addr_getcluster;
    unsigned int addr_readfile;
}; 
struct dev partition[4];
struct user_fs file_dir[20];
struct fs_unit fs_support[32];
void user_dir(char* page);
char* _search_file(openfile* addr,char* page,struct dev sd_num);
symbolic_node symbolic_fs_array[128];
fatdir_t *dir;
#endif
