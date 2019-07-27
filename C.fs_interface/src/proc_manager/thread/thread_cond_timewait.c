int thread_cond_timewait (struct thread_cond *cond,struct thread_mutex * mutex,unsigned int abstime){
	return thread_cond_timedwait_internal(cond, mutex, abstime);
}
