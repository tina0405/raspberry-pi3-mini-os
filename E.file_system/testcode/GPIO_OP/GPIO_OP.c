#include<kservice.h>
#include<gpio.h>
#include<set_gpio.h>
#include<sched.h>
#define oper_compt main
#define GPIO_21 21

struct para_config{
    int para_num;
};

struct para_config drv_config ={
    para_num: 0,
};

void init_compt(void){ /*initial*/
	kservice_uart_write("Initial pullup_21 component!\n\r");
	if(!kservice_reg_compt("pullup_21")){
		kservice_config_compt(&drv_config);
	}

	
}

void oprt_compt(void){ /*operation*/
	struct para gpio21;
	gpio21.gpio = 21;
	gpio21.on_off = 2;
	if(!kservice_region_request(GPIO_21)){
		kservice_set_gpio(gpio21);
 	}	
}


void exit_compt(void){ /*exit*/
	kservice_unreg_compt("pullup_21");
	kservice_uart_write("Clean up pullup_21 component!\n\r");
}

