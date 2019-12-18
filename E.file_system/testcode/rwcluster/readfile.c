#include <stddef.h>
#include <stdint.h>
#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3

struct record { char name[10]; int age; }; 
typedef struct {
    volatile unsigned int lock;
} arch_rwlock_t;

struct File{
   char* _ptr;/*next pos*/
   char* _base;
   int  _cnt;
   int   _bufsize;
   int _fsize; 
   int _tmpname; 
   arch_rwlock_t rw_lock;
};
int application(void) { 
	 
	struct FILE *fp = call_sys_fopen("READ    TXT", "r");
	struct record array[2];
	int error = 0;
	if (fp == NULL) { call_sys_write("Open file fail!"); call_sys_exit(1); } 
	
	//int num = call_sys_fread(&array[0], 14, 2, fp);
	//call_sys_write("Name1: %s Age1: %d\n\r", array[0].name, array[0].age); 
	//call_sys_write("Name2: %s Age2: %d\n\r", array[1].name, array[1].age);
	//call_sys_write("return num: %d\n\r",num);  
	
	for(int a =0;a<2050;a++){
		call_sys_fread(&array[0], sizeof(struct record), 2, fp);
		call_sys_write("count: %d %d\n\r", a,call_sys_ftell(fp));
		if(array[0].name!="Belly"&& array[1].name!="Louis" && array[0].age!=17 && array[1].age!=33){
				call_sys_write("Name1: %s Age1: %d\n\r", array[0].name, array[0].age); 
				call_sys_write("Name2: %s Age2: %d\n\r", array[1].name, array[1].age);
				call_sys_write("count: %d %d\n\r", a,call_sys_ftell(fp));
				error=1;		
		}
	}
	
	if(!error){
		call_sys_write("Succeed! fptr:%d\n\r",call_sys_ftell(fp));
	}


	call_sys_write("Write!");
	struct record array_w[2] = {{"Tina", 25}, {"Sunny", 31}};
	if (fp == NULL) { call_sys_write("Open file fail!"); call_sys_exit(1); }
	for(int a = 2048; a < 2052; a++){ 
		call_sys_fseek(fp ,32*a, SEEK_SET);
		call_sys_fwrite(array_w, sizeof(struct record), 2, fp);
	}	
	

	call_sys_fclose(fp); 
	
	return 0; 
}




