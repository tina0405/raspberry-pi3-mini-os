#ifndef __KERNEL_API__
#define __KERNEL_API__

#include <stdarg.h>
int kservice_dev_read(int dev,unsigned int lba, unsigned char *buffer, unsigned int num);
void kservice_uart_write(char *fmt, ...);
void fat_listdirectory(unsigned int* addr);
int memcmp(char* src, char* dst, unsigned long n);


#endif
