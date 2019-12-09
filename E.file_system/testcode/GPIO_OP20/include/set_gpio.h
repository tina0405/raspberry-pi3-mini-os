#ifndef	_SETGPIO_H
#define	_SETGPIO_H
struct para{
	unsigned long gpio;
	int on_off;
};
void kservice_set_gpio(struct para parameter);
#endif
