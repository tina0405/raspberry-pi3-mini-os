#ifndef _K_SERVICE_
#define _K_SERVICE_

void kservice_uart_write(char *fmt, ...);
void kservice_reg_compt(char* compt_name);
void kservice_unreg_compt(char* compt_name);
int kservice_config_compt(void* para);
void kservice_put32( unsigned long a, unsigned int b);
void delay(int time);
#endif
