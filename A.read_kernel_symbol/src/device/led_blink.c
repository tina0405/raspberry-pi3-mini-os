#include "mini_uart.h"
#include "utils.h"


void led_blink(void){
     for(int b=0;b<30;b++){
     	set_gpio(21,2);
     	delay(5000000);
     	set_gpio(21,0);
     	delay(5000000);
     }
}
