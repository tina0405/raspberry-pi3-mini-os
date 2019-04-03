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

extern unsigned char _end;
unsigned long user_page_start;
extern unsigned int pm_daemon;
extern unsigned int fs_daemon;
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
		    
	       	    // find out file in root directory entries

		    cluster = 0x137FE;
		    if(cluster==0){
		        //cluster=fat_getcluster("KERNEL8 IMG");
			//printf("123");
		    }
		    if(cluster) {
		        // read into memory
		        //fat_addr = fat_readfile(cluster);
			//fat_listdirectory(&_end+(fat_addr-(unsigned int)&_end));
			//uart_dump(fat_readfile(0x000137FE));
			//fat_listdirectory(&_end+(fat_addr-(unsigned int)&_end));
			//printf("456");
		    }


		} else {
		    uart_puts("FAT partition not found???\n");
		}
		

        } 
	
	/*root directory*/
	fat_addr= fat_readfile(2);
	
	/*list root directory*/
	fat_listdirectory(&_end+(fat_addr-(unsigned int)&_end));

        
	
	int res = copy_process(SERVER_THREAD, (unsigned long)&pm_daemon, 0);
	

	if (res < 0) {
		printf("error while starting process manager \n\r");
		return;
	}

	res = copy_process(SERVER_THREAD, (unsigned long)&fs_daemon, 0);
	
	if (res < 0) {
		printf("error while starting process manager \n\r");
		return;
	}
	
	res = copy_process(SERVER_THREAD, (unsigned long)&kernel_process, 0);
	
	if (res < 0) {
		printf("error while starting kernel process\n\r");
		return;
	}
       
	
	timer_init();
	enable_interrupt_controller();
	enable_irq();

	while (1){
		
		printf("kernel\n\r");
		schedule();
		
	}	
	
}	


