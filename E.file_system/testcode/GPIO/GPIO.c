#include<kservice.h>
#include<gpio.h>
#include<sched.h>
#define oper_compt main
struct para{
	unsigned long gpio;
	int on_off;
	int on;
};
struct para input;
int a;
struct para_config drv_config ={
    name: "oprt_compt",
    pnum: 2,
    para_1: sizeof(input.gpio),
    para_2: sizeof(input.on_off),
};

struct para_config drv_config1 ={
    name: "oprt_compt",
    pnum: 2,
    para_1: sizeof(input.gpio),
    para_2: sizeof(input.on_off),
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
	if(!kservice_reg_compt("set_gpio", DRV_COM ,(unsigned long)&drv_config,3)){
		 kservice_config_compt(&drv_config);
	}
	
}


void oprt_compt(struct para parameter){ /*operation*/
	kservice_uart_write("GPIO Operation!\n\r");  	
	kservice_put32(GPPUD,parameter.on_off);
        delay(150);
        kservice_put32(GPPUDCLK0,(1<<parameter.gpio));
        delay(150);
        kservice_put32(GPPUDCLK0,0);      
}


void exit_compt(void){ /*exit*/
	kservice_unreg_compt("set_gpio");
	kservice_uart_write("Clean up GPIO component!\n\r");
}

