#ifndef	_PM_H
#define	_PM_H
#define mail_size 64
#define Rendezvous 1
#define Mailbox 2
#define END_Thread 3
#define Change_Sched 4

typedef int __thread_t;
typedef __thread_t thread_t;
unsigned int sched_type;
void send_msg(unsigned int type, int tid,int addr , void* msg,int size);
struct mailbox recieve_msg(unsigned int type);
void accept_reply(void);
#endif
