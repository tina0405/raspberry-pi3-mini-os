#include <gpio.h>
void clear_gpio(unsigned long gpio){
        put32(GPPUD,0);
        delay(150);
        put32(GPPUDCLK0,(1<<gpio));
        delay(150);
        put32(GPPUDCLK0,0);
	
}
