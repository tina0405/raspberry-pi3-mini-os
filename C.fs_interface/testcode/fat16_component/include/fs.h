#ifndef	_FS_H
#define	_FS_H
#include <stdint.h>
#include <elf.h>

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


struct dev{
	unsigned int record;
	unsigned int partitionlba;
	int type;
	int dev_type; /*nope=0, sd=1*/	
}; 
struct user_fs{
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

#endif
