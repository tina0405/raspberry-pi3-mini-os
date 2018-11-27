#include "mini_uart.h"
#include "utils.h"

void kernel(int procid)
{
	char procstr[2];
	procstr[0] = procid + '0';
	procstr[1] = '\0';

	if(procid == 0) {
		uart_init();
	}
        else {
		delay(100000 * procid);
	}

	uart_send_string("Hello from processor ");
	uart_send_string(procstr);
	uart_send_string("\r\n");

 
    if(procid ==0) {
		while (1) {
			uart_send(uart_recv());
		}
    }
    else {
	while(1) {}
    }
}
