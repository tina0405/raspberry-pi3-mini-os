#ifndef	_MINI_UART_H
#define	_MINI_UART_H

void uart_init ( void );
char uart_recv ( void );
void uart_send ( char c );
void putc ( void* p, char c );
void pr_int(int n);
void pr_T(void);
void set_gpio(unsigned long gpio,int on_off);
void uart_puts(char *s);
void uart_hex(unsigned int d);
void uart_dump(void *ptr);
#endif  /*_MINI_UART_H */
