#include <stddef.h>
#include <stdint.h>
struct record { char name[10]; int age; }; 

struct pos{
	uint32_t real_addr;
	int next_cluster;
};

struct File{
   char* _base;
   int   _bufsize;
   struct pos _real;
   char *_tmpfname; 
};
int application(void) { 
	struct record array[2]; 
	int a = 1;
	struct FILE *fp = call_sys_fopen("READ    TXT", "r");
	if (fp == NULL) { call_sys_write("Open file fail!"); call_sys_exit(a); } 
	int num = call_sys_fread(array, sizeof(struct record), 2, fp); 
	call_sys_write("Name1: %s Age1: %d\n\r", array[0].name, array[0].age); 
	call_sys_write("Name2: %s Age2: %d\n\r", array[1].name, array[1].age);
	call_sys_write("return num: %d\n\r",num);  

	struct record array_w[2] = {{"Tina", 24}, {"Lisa", 18}};
	if (fp == NULL) { call_sys_write("Open file fail!"); call_sys_exit(a); } 
	call_sys_fwrite(array_w, sizeof(struct record), 2, fp);
	call_sys_write("return write num: %d\n\r",num); 

	num = call_sys_fread(array, sizeof(struct record), 2, fp); 
	call_sys_write("Name1: %s Age1: %d\n\r", array[0].name, array[0].age); 
	call_sys_write("Name2: %s Age2: %d\n\r", array[1].name, array[1].age);
	call_sys_write("return num: %d\n\r",num);  
	call_sys_fclose(fp); 
	return 0; 
}




