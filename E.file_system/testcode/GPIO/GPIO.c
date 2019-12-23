#include<kservice.h>
#include<gpio.h>
#include<sched.h>
#define oper_compt main
struct para{
	unsigned long gpio;
	int on_off;
};

struct para2{
	unsigned long gpio;
};
struct para input;
struct para2 input2;
struct para_config drv_config ={
    op_func: 2,
    /*1 operation*/
    name: "on_off",
    interface: USER_DEF,
    pnum: 2,
    para_1: sizeof(input.gpio),
    para_2: sizeof(input.on_off),
    /*2 operation*/
    name2: "reset",
    interface2: USER_DEF,
    pnum2: 1,
    para2_1: sizeof(input2.gpio),
 
};


void DELAY(unsigned long def){
	asm volatile(
	    "delay: subs x0, x0, #1\n"
	    "bne delay\n"
	    "ret"
	);

}


void init_compt(void){ /*initial*/

	kservice_uart_write("Initial GPIO component!\n\r");
	if(!kservice_reg_compt("setgpio", DRV_COM ,&drv_config)){
		kservice_uart_write("Register is successful!");
	}
	
}


void on_off(struct para parameter){ /*operation*/
	kservice_uart_write("GPIO Operation!\n\r");  	
	kservice_put32(GPPUD,parameter.on_off);
        delay(150);
        kservice_put32(GPPUDCLK0,(1<<parameter.gpio));
        delay(150);
        kservice_put32(GPPUDCLK0,0);      
}

void reset(struct para parameter){ /*operation*/
	kservice_uart_write("GPIO Operation!\n\r");  	
	kservice_put32(GPPUD,0);
        delay(150);
        kservice_put32(GPPUDCLK0,(1<<parameter.gpio));
        delay(150);
        kservice_put32(GPPUDCLK0,0);      
}


void exit_compt(void){ /*exit*/
	kservice_unreg_compt("setgpio");
	kservice_uart_write("Clean up GPIO component!\n\r");
}

