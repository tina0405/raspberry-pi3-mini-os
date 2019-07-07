/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "sd.h"
#include "mini_uart.h"
#include "printf.h"
#include "fs.h" 
// get the end of bss segment from linker
extern unsigned char _end;
extern struct sd_dev partition[4];

extern unsigned int t;	
// the BIOS Parameter Block (in Volume Boot Record)

typedef struct {
    char            jmp[3];
    char            oem[8];
    unsigned char   bps0;
    unsigned char   bps1;
    unsigned char   spc;
    unsigned short  rsc;
    unsigned char   nf;
    unsigned char   nr0;
    unsigned char   nr1;
    unsigned short  ts16;
    unsigned char   media;
    unsigned short  spf16;
    unsigned short  spt;
    unsigned short  nh;
    unsigned int    hs;
    unsigned int    ts32;
    unsigned int    spf32;
    unsigned int    flg;
    unsigned int    rc;
    char            vol[6];
    char            fst[8];
    char            dmy[20];
    char            fst2[8];
} __attribute__((packed)) bpb_t;


/**
 * Find a file in root directory entries
 */
unsigned int fat16_getcluster(char *fn)
{
    bpb_t *bpb=(bpb_t*)&_end;
    fatdir_t *dir=(fatdir_t*)(&_end+512);
    unsigned int root_sec, s;
    // find the root directory's LBA
    root_sec=((bpb->spf16)*bpb->nf)+bpb->rsc;
    s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);
    // add partition LBA
    root_sec+=partition[1].partitionlba;
    // load the root directory
    if(sd_readblock(root_sec,(unsigned char*)dir,s/512+1)) {
        // iterate on each entry and check if it's the one we're looking for
        for(;dir->name[0]!='U';dir++) {
            // is it a valid entry?
            if(dir->name[0]==0xE5 || dir->attr[0]==0xF) continue;
            // filename match?
	   

            if(!memcmp(dir->name,fn,8)) {

                // if so, return starting cluster
                return ((unsigned int)dir->ch)<<16|dir->cl;
            }
        }
        //uart_puts("ERROR: file not found\n");
    } else {
        uart_puts("ERROR: Unable to load root directory\n");
    }
    return 0;
}

/**
 * Read a file into memory
 */
char *fat16_readfile(unsigned int cluster)
{
    // BIOS Parameter Block
    bpb_t *bpb=(bpb_t*)&_end;
    // File allocation tables. We choose between FAT16 and FAT32 dynamically
    unsigned int *fat32=(unsigned int*)(&_end+bpb->rsc*512);
    unsigned short *fat16=(unsigned short*)fat32;
    // Data pointers
    unsigned int data_sec, s;
    unsigned char *data, *ptr;
    // find the LBA of the first data sector
    data_sec=((bpb->spf16?bpb->spf16:bpb->spf32)*bpb->nf)+bpb->rsc;
    s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);
    
    data_sec+=(s+511)>>9;
    
    // add partition LBA
    data_sec+=partition[1].partitionlba;
    // dump important properties
    uart_puts("FAT Bytes per Sector: ");
    uart_hex(bpb->bps0 + (bpb->bps1 << 8));
    uart_puts("\nFAT Sectors per Cluster: ");
    uart_hex(bpb->spc);
    uart_puts("\nFAT Number of FAT: ");
    uart_hex(bpb->nf);
    uart_puts("\nFAT Sectors per FAT: ");
    uart_hex((bpb->spf16?bpb->spf16:bpb->spf32));
    uart_puts("\nFAT Reserved Sectors Count: ");
    uart_hex(bpb->rsc);
    uart_puts("\nFAT First data sector: ");
    uart_hex(data_sec);
    uart_puts("\n");
    // load FAT table
    s=sd_readblock(partition[1].partitionlba+1,(unsigned char*)&_end+512,(bpb->spf16?bpb->spf16:bpb->spf32)+bpb->rsc);
    // end of FAT in memory
    data=ptr=&_end+512+s;
    // iterate on cluster chain
    while(cluster>1 && cluster<0xFFF8) {
        // load all sectors in a cluster
        sd_readblock((cluster-2)*bpb->spc+data_sec,ptr,bpb->spc);
        // move pointer, sector per cluster * bytes per sector
        ptr+=bpb->spc*(bpb->bps0 + (bpb->bps1 << 8));
        // get the next cluster in chain
        cluster=bpb->spf16>0?fat16[cluster]:fat32[cluster];
    }
return (char*)data;
}






