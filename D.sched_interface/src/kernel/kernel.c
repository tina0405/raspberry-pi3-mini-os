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
#include "fs.h"
#include "sd.h"
#include "boot.h"
#include "SDCard.h"	
extern unsigned char _end;/*in linker script*/
unsigned long user_page_start;
extern unsigned int pm_daemon;
extern unsigned int fs_daemon;
extern unsigned int user_ipc_service;
extern unsigned int ipc_test;
extern unsigned long shell_user_process;
extern unsigned long try;
unsigned int fat_addr;
struct thread_mutex mm_lock;
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

	unsigned long user_page_2 = move_to_user_mode((unsigned long)start, size, 0);
	if (user_page_2 < 0){
		printf("Error while moving process to user mode\n\r");
	} 
	
	
}

void kernel_main()
{	
	
	uart_init();
	init_printf(NULL, putc);
	irq_vector_init(); 
	if(sd_init() == SD_OK) {
		// read the master boot record and find our partition
		if(fat_getpartition()) {
		     /*compt.c*/
		     //read_ksymbol();
		} else {
		    uart_puts("FAT partition not found???\n\r");
		}	
       } 
        //printf("&_end:%x\n\r",&_end);
	enable_cache();
	_thread_mutex_init(&mm_lock,(void *)0);
	sched_type = &round_robin;

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

        res = copy_process(UMODE_SERVER_THREAD, (unsigned long)&kernel_process, 0, 0);
	
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


