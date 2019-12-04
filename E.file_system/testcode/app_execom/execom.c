#include <stddef.h>
#include <stdint.h>

int application(void) { 
	call_sys_write("EXECUTE COMPONENT!");
	call_sys_execom("kservice_pullup_21");
	return 0; 
}




