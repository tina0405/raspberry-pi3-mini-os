#include <stddef.h>
#include "sched.h"
int _thread_mutex_init (struct thread_mutex *mutex, struct thread_attr_t *attr){
	mutex->__lock = 0;
	mutex->__attr = 0;
	return 0;
}
