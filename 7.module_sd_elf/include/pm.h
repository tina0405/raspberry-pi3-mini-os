#define mail_size 64
#define Rendezvous 1
#define Mailbox 2
#define END_Thread 3

typedef int __thread_t;
typedef __thread_t thread_t;

struct mailbox {
	unsigned int letter_type;
	thread_t dst_task;
	int msg;
};
extern struct mailbox mail[mail_size];
