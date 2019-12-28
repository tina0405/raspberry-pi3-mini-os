#include <stddef.h>
#include <stdint.h>

void DELAY(unsigned long def){
	asm volatile(
	    "delay: subs x0, x0, #1\n"
	    "bne delay\n"
	    "ret"
	);

}
struct para_config{
    int para_num;
};
struct para_config drv_config ={
    para_num: 0,
};

int application(void) { 
	call_sys_write("EXECUTE COMPONENT!");	
	while(1){
		call_sys_execom("kservice_gpio_pullup",&drv_config);			
	}
	return 0; 
}


