#include<kservice.h>
#include<gpio.h>
#include<set_gpio.h>
#include<sched.h>

#define oper_compt main
#define GPIO_20 20


struct para_config drv_config ={
    op_func: 1,
    /*1 operation*/
    name: "pullup",
    interface: USER_DEF,
    pnum: 0,
};



void init_compt(void){ /*initial*/
	kservice_uart_write("Initial gpio20 component!\n\r");
	if(!kservice_reg_compt("gpio", DRV_COM, &drv_config)){
		kservice_uart_write("Register is successful!");
	}

	
}

void pullup(void){ /*operation*/
	struct para gpio20;
	gpio20.gpio = 20;
	gpio20.on_off = 2;
	if(!kservice_region_request(GPIO_20)){
		kservice_setgpio_on_off(gpio20);
 	}	
}


void exit_compt(void){ /*exit*/
	kservice_unreg_compt("gpio");
	kservice_uart_write("Clean up pullup_20 component!\n\r");
}

