#ifndef _K_SERVICE_
#define _K_SERVICE_
#include <stddef.h>
#define FS_COM 1
#define DRV_COM 2
#define SCH_COM 3

#define USER_DEF 1
#define SCH_DEF 2
#define FS_GETCLUSTER_DEF 3
#define FS_READFILE_DEF 4
#define FS_READDIR_DEF 5
#define FS_WRITEBUF_DEF 6
#define FS_WRITEDIR_DEF 7
#define SWAP NULL

struct para_config{
    int op_func;
    /* a group*/
    char name[32];
    int interface;
    int pnum;
 };

void kservice_uart_write(char *fmt, ...);
int kservice_reg_compt(char* compt_name,int type,struct para_config* para);
void kservice_unreg_compt(char* compt_name);
int kservice_config_compt(void* para);
void kservice_put32( unsigned long a, unsigned int b);
int kservice_region_request(unsigned long address);
void delay(int time);

#endif
