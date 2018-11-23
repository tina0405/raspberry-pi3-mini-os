#include "uart.h"
void kernel(void)
{
	uart_init();
	uart_send_string("Hello, world!\r\n");

	while (1) {
		char word = uart_recv();
		uart_send(word);
	}
}
