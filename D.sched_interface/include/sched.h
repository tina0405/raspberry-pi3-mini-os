#ifndef _SCHED_H
#define _SCHED_H
#define THREAD_CPU_CONTEXT			0 		// offset of cpu_context in task_struct 

#ifndef __ASSEMBLER__

#define THREAD_SIZE				4096

#define NR_TASKS				64 

#define FIRST_TASK task[0]
#define LAST_TASK task[NR_TASKS-1]

#define TASK_RUNNING				0
#define TASK_ZOMBIE				1
#define THREAD_JOINABLE 			2 /* The thread is running and joinable.  */
#define THREAD_DETACHED			3 /* The thread is running and detached.  */
#define THREAD_EXITED				4 /* A joinable thread exited and its return code is available.  */


//#define MODULE_THREAD				0x00000002
#define FORK_THREAD				0x00000003
#define APP_THREAD				0x00000002
#define EXTRA_SERVER_THREAD			0x00000001
#define SERVER_THREAD				0x00000000

extern struct pcb_struct *current;
extern struct task_struct *next_real;
struct pcb_struct *task[NR_TASKS];
extern int nr_tasks;
extern struct pcb_struct *task_prio_table[3];
extern struct pcb_struct *head[3];
#include <pm.h> 
struct mailbox {
	unsigned int letter_type;
	thread_t dst_task;
	struct pcb_struct *from;
	void* msg;
	//int empty;
};

struct cpu_context {
	unsigned long x19;
	unsigned long x20;
	unsigned long x21;
	unsigned long x22;
	unsigned long x23;
	unsigned long x24;
	unsigned long x25;
	unsigned long x26;
	unsigned long x27;
	unsigned long x28;
	unsigned long fp;
	unsigned long sp;
	unsigned long pc;
};

#define MAX_PROCESS_PAGES			16	

struct user_page {
	unsigned long phys_addr;
	unsigned long virt_addr;
};

struct mm_struct {
	unsigned long pgd;
	int user_pages_count;
	struct user_page user_pages[MAX_PROCESS_PAGES];
	int kernel_pages_count;
	unsigned long kernel_pages[MAX_PROCESS_PAGES];
};

struct task_struct {
	struct cpu_context cpu_context;
	long state;	
	long counter;
	long priority;
	long preempt_count;
	unsigned long flags;
	struct mm_struct mm;
};


/* The type of a spin lock object.  */
typedef volatile int __thread_spinlock_t;

struct thread_cond
{
  __thread_spinlock_t __lock;
  struct pcb_struct *__queue;
  //struct __pthread_condattr *__attr;
  //struct __pthread_condimpl *__impl;
  void *__data;
};


struct thread_mutex
{
  //__thread_spinlock_t __held;
  //__thread_spinlock_t __lock;
  /* In cthreads, mutex_init does not initialized thre third
     pointer, as such, we cannot rely on its value for anything.  */
  //char *__cthreadscompat1;
  struct pcb_struct *__queue;
  int __attr;
  //void *__data;
  /*  Up to this point, we are completely compatible with cthreads
     and what libc expects.  */
  void *__owner;
  unsigned int __lock;
  /* If NULL then the default attributes apply.  */
};

struct pcb_struct {
	struct cpu_context *cpu_context;
	long *state;	
	long *counter;
	long *priority;
	long *preempt_count;
	unsigned long *flags;
	struct mm_struct *mm;
	int pid;/*process_id*/	
	int tid;/*thread_id*/
	//void* status;/*exit status*/

	struct thread_mutex state_lock;
	struct thread_cond state_cond;
	/*for mutex*/
	/*for scheduler*/	
	struct pcb_struct *prevp;
	struct pcb_struct *nextp;
	/*thread child*/
	struct pcb_struct *main_thread;
	struct pcb_struct *thread_p;/*prev thread*/
	struct pcb_struct *thread_n;/*next thread*/	
	/*mailbox*/
	struct mailbox Box[8];
	/*Rendezvous*/
	struct mailbox Rdv;
	int reply;
	/* thread_set */
	/* thread_id */
};






extern void schedule(void);
extern void schedule_init(void);
extern void timer_tick(void);
extern void preempt_disable(void);
extern void preempt_enable(void);
extern void switch_to(struct pcb_struct* next);
extern void cpu_switch_to(struct task_struct* prev, struct task_struct* next);
extern void exit_process(void);
struct pcb_struct * round_robin(void* nope,struct pcb_struct * current_task, struct pcb_struct * head);
#define INIT_TASK \
/*cpu_context*/ { { 0,0,0,0,0,0,0,0,0,0,0,0,0}, \
/* state etc */	 0,0,15, 0, PF_KTHREAD, \
/* mm */ { 0, 0, {{0}}, 0, {0}} \
}

#endif
#endif
