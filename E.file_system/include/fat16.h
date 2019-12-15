#ifdef FAT16
#ifndef	_FAT16_H
#define	_FAT16_H
unsigned int fat16_getcluster(void* nope, char *fn,struct dev* sd_num);
openfile* fat16_readfile(void* nope, int cluster,struct dev* sd_num);
openfile* fat16_read_directory(void* nope, struct dev* sd_num);
openfile* fat16_readbuf(void* nope, int cluster,struct dev* sd_num);
int fat16_writefile(void* nope, struct dev* sd_num,char* phy,int filesize, unsigned long log,char* phy_dir,char* log_dir);
#endif
#endif
