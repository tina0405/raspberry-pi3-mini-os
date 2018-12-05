#include "printf.h"
#include "timer.h"
#include "irq.h"
#include "mini_uart.h"

char choose = '1';
void kernel_main(void)
{
	uart_init();
	init_printf(0, putc);
	irq_vector_init();
	while(1){	
		printf("If you want stop timer，just enter any key\n\r");
		printf("Please choose a timer number 1 or 3\n\r");
		choose = uart_recv();
		printf("Your choice:%c\n\r", choose);
		switch (choose) {
			case '1':
				timer_init();
				break;
			case '3':
				timer_init_3();
				break;
			default:
				printf("Undefine choose: %d\r\n", choose);
		}

		enable_interrupt_controller();
		enable_irq();
		uart_send(uart_recv());
		disable_irq();
	}
}
