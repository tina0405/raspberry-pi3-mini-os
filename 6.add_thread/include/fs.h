#ifndef	_FS_H
#define	_FS_H
#include <stdint.h>
#include <elf.h>
void list(void);
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

struct mod_section{
	unsigned long num;
	unsigned long addr;
	int size;
}; 
#endif
