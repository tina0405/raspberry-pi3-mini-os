#include "sched.h"
int thread_mutex_trylock (struct thread_mutex *mutex)
{
	int ret = try_lock(&mutex->__lock);
	return ret;/*0 = succeed*/

}
