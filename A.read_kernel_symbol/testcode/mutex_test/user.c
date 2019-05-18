#include <stddef.h>
#include <mutex.h> 
#define Rendezvous 1

void * thread_run(void *arg);
struct thread_mutex mutex;
int count = 0;
int main()
{
    thread_t thread1;
    thread_mutex_lock(&mutex);
    //pthread_mutex_init(&mutex, 0);
    thread_create(&thread1, NULL, thread_run, 0);

    thread_join(thread1, 0);
    //pthread_mutex_destroy(&mutex);
    
    return 0;
}


void * thread_run(void *arg)
{
    int i;
    thread_mutex_unlock(&mutex);

    return 0;
}


