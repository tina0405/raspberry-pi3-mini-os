#include<kservice.h>
#include<gpio.h>
#include<set_gpio.h>
#include<sched.h>
#define oper_compt main
#define GPIO_21 21

struct para_config drv_config ={
    op_func: 1,
    /*1 operation*/
    name: "pullup",
    interface: USER_DEF,
    pnum: 0,
};


void init_compt(void){ /*initial*/
	kservice_uart_write("Initial pullup_21 component!\n\r");
	if(!kservice_reg_compt(SWAP, DRV_COM, &drv_config)){
		kservice_uart_write("Register is successful!");
	}

	
}

void pullup(void){ /*operation*/
	struct para gpio21;
	gpio21.gpio = 21;
	gpio21.on_off = 2;
	if(!kservice_region_request(GPIO_21)){
		kservice_setgpio_on_off(gpio21);
 	}	
}


void exit_compt(void){ /*exit*/
	kservice_unreg_compt(SWAP);
	kservice_uart_write("Clean up pullup_21 component!\n\r");
}

