#ifndef	_PM_H
#define	_PM_H
#define mail_size 64
#define Rendezvous 1
#define Mailbox 2
#define END_Thread 3

typedef int __thread_t;
typedef __thread_t thread_t;
unsigned int sched_type;
//struct mailbox recieve_msg(unsigned int type);
//void accept_reply(void);
#endif
