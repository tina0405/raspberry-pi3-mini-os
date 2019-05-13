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
#include "sd.h"
#include "fat.h"
#include "pm.h"

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
	printf("Kernel process started. EL %d\r\n", get_el());
	unsigned long begin = (unsigned long)&user_begin;
	unsigned long end = (unsigned long)&user_end;
	unsigned long process = (unsigned long)&shell_user_process;

	user_page_start = move_to_user_mode(begin, end - begin, process - begin);
	if (user_page_start < 0){
		printf("Error while moving process to user mode\n\r");
	} 

}

void mod_process(unsigned long* start,unsigned long size){
	printf("\r\nModule process started. EL %d\r\n", get_el());

	unsigned long user_page_2 = move_to_user_mode(start, size, 0);
	if (user_page_2 < 0){
		printf("Error while moving process to user mode\n\r");
	} 
	
	
}
int c = 2;
void kernel_main()
{
	//unsigned int cluster;
	uart_init();
	init_printf(NULL, putc);
	irq_vector_init(); 
	
	//set_gpio(20,2);
	//set_gpio(16,2);
	//set_gpio(26,2);
	//set_gpio(19,2);
	//set_gpio(12,2);


	
        unsigned int cluster;
	if(sd_init()==SD_OK) {

		// read the master boot record and find our partition
		if(fat_getpartition()) {
		     /*root directory*/
		     fat_addr= fat_readfile(2);	
		     /*list root directory*/
		     fat_listdirectory(&_end+(fat_addr-(unsigned int)&_end));
		     build_root();
		     search_file();

		} else {
		    uart_puts("FAT partition not found???\n");
		}
		

        } 
        enable_cache();
	int res = copy_process(SERVER_THREAD, (unsigned long)&pm_daemon, 0, 0);
	

	if (res < 0) {
		printf("error while starting process manager \n\r");
		return;
	}
/*
	res = copy_process(SERVER_THREAD, (unsigned long)&ipc_test, 0, 0);
	
	if (res < 0) {
		printf("error while starting kernel process\n\r");
		return;
	}
*/
	res = copy_process(SERVER_THREAD, (unsigned long)&fs_daemon, 0, 0);
	
	if (res < 0) {
		printf("error while starting process manager \n\r");
		return;
	}
	
	res = copy_process(SERVER_THREAD, (unsigned long)&kernel_process, 0, 0);
	
	if (res < 0) {
		printf("error while starting kernel process\n\r");
		return;
	}

		
	/*
	unsigned long base = fat_readfile(0xD214) + 0x10000;
	unsigned long size_u = 0x114;
	char map_array[size_u];
	memcpy((char*)base, (char*)&map_array[0],size_u);
	copy_process(SERVER_THREAD, (unsigned long)&mod_process, &map_array[0], size_u);
	*/

	
	timer_init();
	enable_interrupt_controller();
	enable_irq();

	/*test trylock and unlock*/
	int c = 0;
	printf("lock:%x\n\r",c);	
	int succeed = try_lock(&c);
	printf("lock_succeed:%d\n\r",succeed);
	printf("lock:%x\n\r",c);	
	printf("trylock\n\r");
	
	succeed = unlock(&c);
	printf("unlock_succeed:%d\n\r",succeed);
	printf("unlock:%x\n\r",c);
	while (1){
		
		printf("kernel\n\r");
		schedule();
		
	}	
	
}	


