/* Return the thread ID of the calling thread.  */
#include "threadtype.h"
#include "sched.h"
extern struct pcb_struct* current;
thread_t thread_id_self ( void ){	
	return current -> tid;	/*now task*/
}
