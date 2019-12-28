#ifndef	_SETGPIO_H
#define	_SETGPIO_H
struct para{
	unsigned long gpio;
	int on_off;
};
void kservice_setgpio_on_off(struct para parameter);
#endif
