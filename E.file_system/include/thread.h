#ifndef	_THREAD_H
#define	_THREAD_H
#include "threadtype.h"
#include "sched.h"
/*for user*/
int thread_create(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* b);
int thread_equal (thread_t t1, thread_t t2);
int thread_join ( thread_t thread, void **value_ptr);
int thread_mutex_unlock (struct thread_mutex *mutex);
int thread_mutex_lock (struct thread_mutex *mutex);
int thread_mutex_trylock (struct thread_mutex *mutex);
thread_t sys_thread_self(void);
/*for kernel*/
int _thread_mutex_unlock (struct thread_mutex *mutex);
int _thread_mutex_lock (struct thread_mutex *mutex);
int _thread_mutex_trylock (struct thread_mutex *mutex);
void _thread_cond_signal(thread_t thread);
void *_thread_exit (void *status);
int _thread_create(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg);
int _thread_mutex_init ( struct thread_mutex *mutex, struct thread_attr_t *attr );
#endif
