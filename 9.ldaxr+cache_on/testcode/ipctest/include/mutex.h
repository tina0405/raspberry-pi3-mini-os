struct thread_mutex
{
  struct pcb_struct *__queue;
  int __attr;
  void *__owner;
  unsigned __lock;

};
