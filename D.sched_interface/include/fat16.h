#ifdef FAT16
#ifndef	_FAT16_H
#define	_FAT16_H
unsigned int fat16_getcluster(void* nope, char *fn,struct dev* sd_num);
char *fat16_readfile(void* nope, int cluster,struct dev* sd_num);
char* fat16_read_directory(void* nope, struct dev* sd_num);
#endif
#endif
