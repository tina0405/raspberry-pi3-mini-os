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
unsigned int t;	
unsigned long user_page_start;
void kernel_process(){
	printf("Kernel process started. EL %d\r\n", get_el());
	unsigned long begin = (unsigned long)&user_begin;
	unsigned long end = (unsigned long)&user_end;
	unsigned long process = (unsigned long)&user_process;
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
	timer_init();

	//set_gpio(20,2);
	//set_gpio(16,2);
	//set_gpio(26,2);
	//set_gpio(19,2);
	//set_gpio(12,2);

	enable_interrupt_controller();
	enable_irq();

#ifdef FS
        unsigned int cluster;
	if(sd_init()==SD_OK) {
		
		
		// read the master boot record and find our partition
		if(fat_getpartition()) {
		    // find out file in root directory entries
		    cluster=fat_getcluster("OVERLAYS    ");
		    if(cluster==0)
		        cluster=fat_getcluster("KERNEL8 IMG");
		    if(cluster) {
		        // read into memory
		        t = fat_readfile(cluster);
			fat_listdirectory(&_end+(t-(unsigned int)&_end));
		    }
		} else {
		    uart_puts("FAT partition not found???\n");
		}
		

        } 
	while(!uart_recv()){}
	/*root directory*/
	t = fat_readfile(2);
	/*list root directory*/
	fat_listdirectory(&_end+(t-(unsigned int)&_end));
#endif	
	printf("copy\n\r");
	int res = copy_process(PF_KTHREAD, (unsigned long)&kernel_process, 0);
	
	if (res < 0) {
		printf("error while starting kernel process");
		return;
	}
	printf("sched\n\r");
	while (1){

                printf("kernel while schedule\n\r");
		schedule();
	}	
	
}	


