#include <stddef.h>
#include <mutex.h> 
#define Rendezvous 1

void * thread_run(void *arg);
struct thread_mutex mutex;
int count = 0;

int main()
{
    //thread_t thread1, thread2;
    thread_mutex_lock(&mutex);
    //pthread_mutex_init(&mutex, 0);
    //thread_create(&thread1, NULL, thread_run, 0);
    //thread_create(&thread2, NULL, thread_run, 0);

    //thread_join(thread1, 0);
    //thread_join(thread2, 0);
    //pthread_mutex_destroy(&mutex);
    return 0;
}


void * thread_run(void *arg)
{
    int i;
    thread_mutex_lock(&mutex);
/*
    for (i = 0; i < 3; i++) {
	call_sys_write("thread[");
	call_sys_write_int(thread_self());
	call_sys_write("] ");
        call_sys_write_int(count++);
	call_sys_write("\n\r");
    }
    thread_mutex_unlock(&mutex);
*/
    return 0;
}


