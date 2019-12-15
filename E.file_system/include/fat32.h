#ifndef	_FAT32_H
#define	_FAT32_H
unsigned int fat32_getcluster(void* nope, char *fn,struct dev* sd_num);
openfile* fat32_readfile(void* nope, int cluster,struct dev* sd_num);
openfile* fat32_read_directory(void* nope, struct dev* sd_num);
openfile* fat32_readbuf(void* nope, int cluster,struct dev* sd_num);
int fat32_writefile(void* nope, struct dev* sd_num,char* phy,int filesize, unsigned long log,char* phy_dir,char* log_dir);
#endif
