#ifndef	_FAT32_H
#define	_FAT32_H
unsigned int fat32_getcluster(char *fn,struct dev* sd_num);
char *fat32_readfile( int cluster,struct dev* sd_num);
char* fat32_read_directory(void* nope, struct dev* sd_num);
#endif
