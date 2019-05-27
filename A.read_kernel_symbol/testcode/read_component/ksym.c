#include<kservice.h>
#include<gpio.h>


void init_compt(void){
	//kservice_reg_compt("led_blink");
	kservice_uart_write("Initial component!\n\r");

}

void led_21_blink(void){
     /*
     for(int b=0;b<30;b++){
     	set_gpio(21,2);
     	delay(5000000);
     	set_gpio(21,0);
     	delay(5000000);
     }
     */
}

void led_20_blink(int a){
     /*
     for(int b=0;b<30;b++){
     	set_gpio(20,2);
     	delay(5000000);
     	set_gpio(20,0);
     	delay(5000000);
     }
     */
}


void * const operation[] = {led_21_blink,led_20_blink};



void set_gpio(unsigned long gpio,int on_off){
/*        
	put32(GPPUD,on_off);
        delay(150);
        put32(GPPUDCLK0,(1<<gpio));
        delay(150);
        put32(GPPUDCLK0,0);
*/
}


void clean_compt(void){
	//kservice_reg_compt("led_blink");
	kservice_uart_write("Clean up GPIO component!\n\r");
}

