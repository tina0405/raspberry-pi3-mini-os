#include <stddef.h>
#include <stdint.h>
#include "printf.h"
#include "utils.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"
#include "mini_uart.h"
#include "sys.h"
#include "user.h"
#include "fat.h"
#include "pm.h"
#include "SDCard.h"	
extern unsigned char _end;
unsigned long user_page_start;
extern unsigned int pm_daemon;
extern unsigned int fs_daemon;
extern unsigned int user_ipc_service;
extern unsigned int ipc_test;
extern unsigned long shell_user_process;
extern unsigned long try;
unsigned int fat_addr;
void kernel_process(){
	unsigned long begin = (unsigned long)&user_begin;
	unsigned long end = (unsigned long)&user_end;
	unsigned long process = (unsigned long)&shell_user_process;

	user_page_start = move_to_user_mode(begin, end - begin, process - begin);
	if (user_page_start < 0){
		printf("Error while moving process to user mode\n\r");
	} 

}

void mod_process(unsigned long* start,unsigned long size){
	printf("\r\nUser process start to set. EL %d\r\n", get_el());

	unsigned long user_page_2 = move_to_user_mode(start, size, 0);
	if (user_page_2 < 0){
		printf("Error while moving process to user mode\n\r");
	} 
	
	
}

void kernel_main()
{
	uart_init();
	init_printf(NULL, putc);
	irq_vector_init(); 
	

        unsigned int cluster;
	//sdInitCard (&printf, &printf, true)
	if(sd_init() == SD_OK) {
		// read the master boot record and find our partition
		if(fat_getpartition()) {
		    /*root directory*/
		    fat_addr= fat_readfile(2);	
		    /*list root directory*/
		    //fat_listdirectory_16();
		    fat_listdirectory(&_end+(fat_addr-(unsigned int)&_end));
		    build_root();
		    search_file();
		    
		} else {
		    uart_puts("FAT partition not found???\n\r");
		}
		
       } 
      
	enable_cache();
	

	int res = copy_process(SERVER_THREAD, (unsigned long)&pm_daemon, 0, 0);
	

	if (res < 0) {
		printf("error while starting process manager \n\r");
		return;
	}

	res = copy_process(SERVER_THREAD, (unsigned long)&fs_daemon, 0, 0);
	
	if (res < 0) {
		printf("error while starting process manager \n\r");
		return;
	}

        res = copy_process(APP_THREAD, (unsigned long)&kernel_process, 0, 0);
	
	if (res < 0) {
		printf("error while starting kernel process\n\r");
		return;
	}
		
	
	timer_init();
	enable_interrupt_controller();
	enable_irq();


	while (1){
		
		//printf("kernel\n\r");
		schedule();
		
	}	
	
}	


