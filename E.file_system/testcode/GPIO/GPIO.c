#include<kservice.h>
#include<gpio.h>
#include<sched.h>
#define oper_compt main

void DELAY(unsigned long def,int on_off){
	asm volatile(
	    "delay: subs x0, x0, #1\n"
	    "bne delay\n"
	    "ret"
	);

}

void init_compt(void){ /*initial*/
	kservice_uart_write("Initial GPIO component!\n\r");
	kservice_uart_write("%d\n\r",A(5));
	delay(5);
	kservice_reg_compt("set_gpio");
}



int A(int a){
	return a+1;
}


void oprt_compt(unsigned long gpio,int on_off){ /*operation*/
	kservice_uart_write("GPIO Operation!\n\r");  	
	kservice_put32(GPPUD,on_off);
        delay(150);
        kservice_put32(GPPUDCLK0,(1<<gpio));
        delay(150);
        kservice_put32(GPPUDCLK0,0);      
}


void exit_compt(void){ /*exit*/
	//kservice_unreg_compt("set_gpio");
	kservice_uart_write("Clean up GPIO component!\n\r");
        kservice_uart_write("BACK!\n\r");
}

