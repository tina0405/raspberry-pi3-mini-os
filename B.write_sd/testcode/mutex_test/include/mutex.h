typedef int __thread_t;
typedef __thread_t thread_t;

struct thread_mutex
{
  struct pcb_struct *__queue;
  int __attr;
  void *__owner;
  unsigned __lock;

};
