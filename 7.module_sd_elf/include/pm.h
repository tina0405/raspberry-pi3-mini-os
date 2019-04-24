#define mail_size 64
#define Rendezvous 1
#define Mailbox 2
#define END_Thread 3

typedef int __thread_t;
typedef __thread_t thread_t;

void send_msg(unsigned int type, int pid, int msg);
struct mailbox recieve_msg(unsigned int type);
