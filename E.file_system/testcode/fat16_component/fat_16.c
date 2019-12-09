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
#ifdef FAT16
#include "sd.h"
#include "strcmp.h"
#include "mini_uart.h"
#include "printf.h"
#include "fs.h"
#include "fat.h" 
#include "mm.h" 
#define fs_type 1
#define R_FILE 1
#define R_DIR 2
#define R_CLUSTER 3
// get the end of bss segment from linker

/**
 * Find a file in root directory entries
 */

void init_compt(void){ /*initial*/
	kservice_uart_write("Initial FAT16 component!\n\r");
	if(!kservice_reg_compt("FAT16",fs_type)){
		kservice_config_compt(R_FILE,&drv_config);
		kservice_config_compt(R_DIR,&drv_config);
		kservice_config_compt(R_CLUSTER,&drv_config);
	}

	
}

unsigned int fat16_getcluster(void* nope,char *fn,struct dev* sd_num)
{

    bpb_t *bpb=(bpb_t*)kservice_kend(sd_num->dbr);/*DBR*/
    fatdir_t *dir=(fatdir_t*)kservice_kend((2048);
    unsigned int root_sec, s;
    // find the root directory's LBA
    root_sec=((bpb->spf16)*bpb->nf)+bpb->rsc;
    s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);
    // add partition LBA
    root_sec+= sd_num->partitionlba;  
    // load the root directory
    if(kservice_dev_read(1, root_sec, (unsigned char*)dir, s/512+1)) {
        // iterate on each entry and check if it's the one we're looking for
        for(;dir->name[0]!='U';dir++) {
            // is it a valid entry?
            if(dir->name[0]==0xE5 || dir->attr[0]==0xF) continue;
            // filename match?
            if(!strcmp(dir->name,fn)) {
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
openfile* fat16_readfile(void* nope, int cluster,struct dev* sd_num)
{
    openfile *ret;
    openfile tmp;
    ret = &tmp;
    // BIOS Parameter Block
    bpb_t *bpb=(bpb_t*)kservice_kend((sd_num->dbr);
    // File allocation tables. We choose between FAT16 and FAT32 dynamically
    //&_end + (512*3-sd_num->dbr) + bpb->rsc*512
    unsigned short *fat16=(unsigned int*)kservice_kstart(sd_num-> fat_table_start - 512 + bpb->rsc*512);
    // unsigned short *fat16=(unsigned short*)fat32;
    // Data pointers
    unsigned int data_sec, s;
    unsigned char *data, *ptr;
    // find the LBA of the first data sector
    data_sec = ((bpb->spf16)*bpb->nf)+bpb->rsc;
    s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);

    data_sec+=(s+511)>>9;//fat16
    // add partition LBA
    data_sec+= sd_num->partitionlba;
    // dump important properties
    // load FAT table

    //s = kservice_dev_read(1, sd_num->partitionlba+1,(unsigned char*)(&_end+2048),(bpb->spf16)+bpb->rsc);  
    // end of FAT in memory
    struct mm_info page = allocate_kernel_page(4096);/*improve*//*buff*/
    struct mm_info phy_page = allocate_kernel_page(4096);
    data = ptr = (unsigned char *)page.start;
    //data = ptr = (unsigned char*)(&_end+2048);
    // iterate on cluster chain
    //ret->phy_addr = (cluster-2)*bpb->spc+data_sec;
    int iterate = 0;
    while(cluster>1 && cluster<0xFFF8) {
	// load all sectors in a cluster
	((unsigned long *)phy_page.start)[iterate++] = (unsigned long)(cluster-2)*bpb->spc+data_sec;
	if(!kservice_dev_read(1, (cluster-2)*bpb -> spc + data_sec, ptr ,bpb->spc)){
		printf("Unable to read SD card!");
		return NULL;
	}
	// move pointer, sector per cluster * bytes per sector
        ptr+=bpb->spc*(bpb->bps0 + (bpb->bps1 << 8));
        // get the next cluster in chain
        cluster=fat16[cluster];
    }
    ret->phy_addr = (unsigned char *)phy_page.start;
    ret->log_addr = data;
    return ret;
}
openfile* fat16_read_directory(void* nope, struct dev* sd_num)
{ 
    openfile *ret;
    openfile tmp;
    ret = &tmp;
    bpb_t *bpb=(bpb_t*)kservice_kend(sd_num->dbr);
    unsigned int root_sec, s;
    // find the root directory's LBA
    root_sec=((bpb->spf16)*bpb->nf)+bpb->rsc;
    s = (bpb->nr0 + (bpb->nr1 << 8));
    s *= sizeof(fatdir_t);
    // add partition LBA
    root_sec+=sd_num->partitionlba;
    // load the root directory
    struct mm_info dir_page;
    dir_page =  allocate_kernel_page(4096);
    struct mm_info phy_page =  allocate_kernel_page(4096);
    if(!kservice_dev_read(1, root_sec,(unsigned char*)(dir_page.start),s/512+1)){
		printf("Unable to read SD card!");
		return NULL;
    }
    //sd_num->directory = dir_page.start;
    //data_dump((unsigned int*)(&_start_ + sd_num->directory),256);
    //fat_listdirectory((unsigned int*)(&_start_ + sd_num->directory));
    ((unsigned long *)phy_page.start)[0]= root_sec;
    ret->log_addr = dir_page.start;
    ret->phy_addr= (unsigned char *)phy_page.start;
    return ret;/*buffer*/
}
#endif
