#include <stddef.h>
#include <stdint.h>

void DELAY(unsigned long def){
	asm volatile(
	    "delay: subs x0, x0, #1\n"
	    "bne delay\n"
	    "ret"
	);

}

int application(void) { 
	call_sys_write("EXECUTE COMPONENT!");	
	call_sys_execom("kservice_pullup_21");	
	for(int a; a<5000 ;a++){	
		call_sys_write("+");
	}	
	return 0; 
}




