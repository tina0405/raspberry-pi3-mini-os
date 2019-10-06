typedef int __thread_t;
typedef __thread_t thread_t;

int thread_create(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* b);
int thread_equal (thread_t t1, thread_t t2);
int thread_join ( thread_t thread, void **value_ptr);
thread_t sys_thread_self(void);

