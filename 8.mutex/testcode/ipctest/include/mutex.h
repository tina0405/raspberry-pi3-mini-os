struct __thread_mutex{
	void* owner;
	unsigned int __locks;
	struct __pthread *__queue;

};

typedef struct __thread_mutex thread_mutex;
