#ifndef _K_SERVICE_
#define _K_SERVICE_
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

struct para_config{
    int op_func;
    /* a group*/
    int interface;
    char name[32];
    int pnum;
    int para_1;
    int para_2;

    int interface2;
    char name2[32];
    int pnum2;
    int para2_1;
 };

void kservice_uart_write(char *fmt, ...);
int kservice_reg_compt(char* compt_name,int type,struct para_config* para);
void kservice_unreg_compt(char* compt_name);
int kservice_config_compt(void* para);
void kservice_put32( unsigned long a, unsigned int b);
int kservice_region_request(unsigned long address);
void delay(int time);
#endif
