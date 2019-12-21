#ifndef _K_SERVICE_
#define _K_SERVICE_
#define FS_COM 1
#define DRV_COM 2
#define SCH_COM 3
struct para_config{
    char name[32];
    int pnum;
    int para_1;
    int para_2;
 };

void kservice_uart_write(char *fmt, ...);
int kservice_reg_compt(char* compt_name,int type,unsigned long para,int type2);
void kservice_unreg_compt(char* compt_name);
int kservice_config_compt(void* para);
void kservice_put32( unsigned long a, unsigned int b);
int kservice_region_request(unsigned long address);
void delay(int time);
#endif
