#include<kservice.h>
#include<gpio.h>
#include<sched.h>
#define oper_compt main


/*
void led_21_blink(void){
     
     for(int b=0;b<30;b++){
     	set_gpio(21,2);
     	delay(5000000);
     	set_gpio(21,0);
     	delay(5000000);
     }
}

void led_20_blink(int a){
     for(int b=0;b<30;b++){
     	set_gpio(20,2);
     	delay(5000000);
     	set_gpio(20,0);
     	delay(5000000);
     }
}
*/

void init_compt(void){ /*initial*/
	kservice_uart_write("Initial GPIO component!\n\r");
	//kservice_reg_compt("set_gpio");
}


void oper_compt(unsigned long gpio,int on_off,int A,char b,char* name){ /*operation*/
	kservice_uart_write("GPIO Operation!\n\r");        
	/*
	put32(GPPUD,on_off);
        delay(150);
        put32(GPPUDCLK0,(1<<gpio));
        delay(150);
        put32(GPPUDCLK0,0);
	*/

}


void exit_compt(void){ /*exit*/
	//kservice_unreg_compt("set_gpio");
	kservice_uart_write("Clean up GPIO component!\n\r");
        kservice_uart_write("BACK!\n\r");
}

