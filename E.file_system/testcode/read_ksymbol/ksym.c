#include<printf.h>
void main(void){
	kservice_uart_write("Read kernel module!\n\r");
	kservice_uart_write("123\n\r");
	while(1);
}
