#ifndef	_FAT32_H
#define	_FAT32_H
unsigned int fat32_getcluster(void* nope, char *fn,struct dev* sd_num);
char *fat32_readfile(void* nope, int cluster,struct dev* sd_num);
char* fat32_read_directory(void* nope, struct dev* sd_num);
struct pos fat32_getpos(void* nope, int cluster,struct dev* sd_num);
#endif
