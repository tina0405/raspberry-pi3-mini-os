#include<kservice.h>
#include<gpio.h>
#include<sched.h>
#define oper_compt main




void init_compt(void){ /*initial*/
	kservice_uart_write("Initial GPIO component!\n\r");
	kservice_set_gpio(21,0);
	//kservice_reg_compt("set_gpio");
}

void oprt_compt(unsigned long gpio,int on_off){ /*operation*/
 
}


void exit_compt(void){ /*exit*/
	//kservice_unreg_compt("set_gpio");
	kservice_uart_write("Clean up GPIO component!\n\r");
        kservice_uart_write("BACK!\n\r");
}

