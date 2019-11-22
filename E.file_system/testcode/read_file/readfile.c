#include <stddef.h>
#include <stdint.h>
struct record { char name[10]; int age; }; 
typedef struct {
    volatile unsigned int lock;
} arch_rwlock_t;

struct File{
   char* _base;
   int   _bufsize;
   char *_tmpname;
   arch_rwlock_t rw_lock; 
};
int application(void) { 
	 
	int a = 1;
	struct FILE *fp = call_sys_fopen("READ    TXT", "r");
	struct record array[3];
	if (fp == NULL) { call_sys_write("Open file fail!"); call_sys_exit(a); } 
	
	int num = call_sys_fread(&array[0], 14, 2, fp);
	call_sys_write("SIZE: %x \n\r", sizeof(&array[0]));  
	call_sys_write("Name1: %s Age1: %d\n\r", array[0].name, array[0].age); 
	call_sys_write("Name2: %s Age2: %d\n\r", array[1].name, array[1].age);
	call_sys_write("return num: %d\n\r",num);  

	struct record array_w[3] = {{"Tina", 25}, {"Gina", 31}, {"Rose", 25}};
	if (fp == NULL) { call_sys_write("Open file fail!"); call_sys_exit(a); } 
	call_sys_fwrite(array_w, sizeof(struct record), 3, fp);
	call_sys_write("return write num: %d\n\r",num); 

	num = call_sys_fread(array, sizeof(struct record), 3, fp); 
	call_sys_write("Name1: %s Age1: %d\n\r", array[0].name, array[0].age); 
	call_sys_write("Name2: %s Age2: %d\n\r", array[1].name, array[1].age);
	call_sys_write("Name2: %s Age2: %d\n\r", array[2].name, array[2].age);
	call_sys_write("return num: %d\n\r",num);  
	call_sys_fclose(fp); 
	
	return 0; 
}




