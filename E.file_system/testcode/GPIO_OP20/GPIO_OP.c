#include<kservice.h>
#include<gpio.h>
#include<set_gpio.h>
#include<sched.h>

#define oper_compt main
#define GPIO_20 20

struct para_config{
    int para_num;
};

struct para_config drv_config ={
    para_num: 0,
};

typedef struct {
    char            name[8];
    char            ext[3];
    char            attr[9];
    unsigned short  ch;
    unsigned int    attr2;
    unsigned short  cl;
    unsigned int    size;
} __attribute__((packed)) fatdir_t;


void init_compt(void){ /*initial*/
	kservice_uart_write("Initial pullup_20 component!\n\r");
	if(!kservice_reg_compt("pullup_20")){
		kservice_config_compt(&drv_config);
	}

	
}

void oprt_compt(void){ /*operation*/
	struct para gpio20;
	gpio20.gpio = 20;
	gpio20.on_off = 2;
	if(!kservice_region_request(GPIO_20)){
		kservice_set_gpio(gpio20);
 	}	
}


void exit_compt(void){ /*exit*/
	kservice_unreg_compt("pullup_20");
	kservice_uart_write("Clean up pullup_20 component!\n\r");
}

