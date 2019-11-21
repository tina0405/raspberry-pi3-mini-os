#ifndef	_IPC_H
#define	_IPC_H
#define PROCESS_MANAGER 0
#define FILESYS_MANAGER 1
#define COMPONENT_MANAGER 2
int send_msg(unsigned int type, int tid,int addr , void* msg,int size, void* msg2);
#endif
