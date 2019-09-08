#include "sd.h"
#include "mini_uart.h"
#include "printf.h"
#include "fs.h"
#include "fat.h"
extern unsigned char _end; 

/**
 * Get the starting LBA address of the first partition
 * so that we know where our FAT file system starts, and
 * read that volume's BIOS Parameter Block
 */

int fat_getpartition(void)
{
    unsigned char *mbr=&_end;
    bpb_t *bpb=(bpb_t*)&_end;
    // read the partitioning table
    if(sd_readblock(0,&_end,1)) {/*mbr:lba->0*/
        // check magic
        if(mbr[510]!=0x55 || mbr[511]!=0xAA) {
            uart_puts("ERROR: Bad magic in MBR\n");
            return 0;
        }
        // check partition type
        for(int p_index = 0x0;p_index < 0x40;p_index = p_index + 0x10){
            if(mbr[0x1C2 + p_index] == 0x0/*FAT32 LBA*/){
	        break;
	    }
	    partition[p_index/0x10].record = (p_index/0x10)*512;
	    partition[p_index/0x10].type = mbr[0x1C2 + p_index]; 
	    // should be this, but compiler generates bad code...
            //partitionlba=*((unsigned int*)((unsigned long)&_end+0x1C6));
	    partition[p_index/0x10].partitionlba = mbr[0x1C6 + p_index] + (mbr[0x1C7 + p_index]<<8) + (mbr[0x1C8+ p_index]<<16) + (mbr[0x1C9+ p_index]<<24);/*read DBR*/
	   
	}
	int i = 0;
	// read the boot record
        while(partition[i].partitionlba!=0){
	    if(!sd_readblock(partition[i].partitionlba,&_end+512*i,1)) {/*DBR DATA*/
            	uart_puts("ERROR: Unable to read boot record\n");
            	return 0;
            }
	    i++;
	}
	
        // check file system type. We don't use cluster numbers for that, but magic bytes
       
	 if( !(bpb->fst[0]=='F' && bpb->fst[1]=='A' && bpb->fst[2]=='T') &&
            !(bpb->fst2[0]=='F' && bpb->fst2[1]=='A' && bpb->fst2[2]=='T')) {
            uart_puts("ERROR: Unknown file system type for boot\n");
            return 0;
         }

        return 1;
    }
    printf("Cannot read sd card block.");
    return 0;
}


void fat_listdirectory(unsigned int* addr)
{
    //bpb_t *bpb=(bpb_t*)addr;
    dir=(fatdir_t*)addr;
}
