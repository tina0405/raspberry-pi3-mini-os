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
//https://www.twblogs.net/a/5b7c43132b71770a43da21e2
#include "sd.h"
#include "mini_uart.h"
#include "printf.h"
#include "fs.h" 
#include "fat.h"
// get the end of bss segment from linker
extern unsigned char _end;
/**
 * Find a file in root directory entries
 */
unsigned int fat32_getcluster(char *fn,struct dev sd_num)
{

    bpb_t *bpb=(bpb_t*)&_end+ sd_num.record;/*DBR*/
    fatdir_t *dir_32=(fatdir_t*)(&_end+2048);
    unsigned int root_sec, s;
    // find the root directory's LBA
    root_sec=((bpb->spf32)*bpb->nf)+bpb->rsc;
    s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);
    root_sec+=(bpb->rc-2)*bpb->spc;
    // add partition LBA
    root_sec+= sd_num.partitionlba;  //root_sec+=partition[0].partitionlba;
    // load the root directory
    if(sd_readblock(root_sec,(unsigned char*)dir_32,s/512+1)) {
        // iterate on each entry and check if it's the one we're looking for
        for(;dir_32->name[0]!='U';dir_32++) {
            // is it a valid entry?
            if(dir_32->name[0]==0xE5 || dir_32->attr[0]==0xF) continue;
            // filename match?
            if(!memcmp(dir_32->name,fn,8)) {
                return ((unsigned int)dir_32->ch)<<16|dir_32->cl;
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
//int sect = 0;
char *fat32_readfile( int cluster,struct dev sd_num)
{

    // BIOS Parameter Block
    bpb_t *bpb=(bpb_t*)&_end+sd_num.record;
    // File allocation tables. We choose between FAT16 and FAT32 dynamically
    unsigned int *fat32=(unsigned int*)(&_end + (512*3-sd_num.record) + bpb->rsc*512);/*reserved: bpb->rsc*/
   // unsigned short *fat16=(unsigned short*)fat32;
    // Data pointers
    unsigned int data_sec, s;
    unsigned char *data, *ptr;
    // find the LBA of the first data sector
    data_sec = ((bpb->spf32)*bpb->nf)+bpb->rsc;

    s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);
    // add partition LBA
    data_sec+= sd_num.partitionlba;
    // dump important properties

    // load FAT table
    s = sd_readblock(sd_num.partitionlba+1,(unsigned char*)&_end+2048,(bpb->spf32)+bpb->rsc);

    // end of FAT in memory
    data = ptr = &_end+2048+s;
    // iterate on cluster chain
    while(cluster>1 && cluster<0xFFF8) {
	// load all sectors in a cluster
        sd_readblock((cluster-2)*bpb -> spc + data_sec, ptr ,bpb->spc);
	// move pointer, sector per cluster * bytes per sector
        ptr+=bpb->spc*(bpb->bps0 + (bpb->bps1 << 8));
        // get the next cluster in chain
        cluster=fat32[cluster];
    }
    return (char*)data;
}

char* fat32_read_directory(struct dev sd_num)
{
    bpb_t *bpb=(bpb_t*)&_end + sd_num.record;
    unsigned int fat_addr= fat32_readfile(bpb->rc, sd_num);	
    /*list root directory*/
    fat_listdirectory(&_end+(fat_addr-(unsigned int)&_end));
    build_root();
    char* name = search_file();  
    return name;
}


