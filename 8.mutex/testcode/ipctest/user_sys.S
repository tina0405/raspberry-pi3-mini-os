.section ".text.user"

.set SYS_WRITE_NUMBER, 0 		// syscal numbers 
.set SYS_FORK_NUMBER, 1 	
.set SYS_EXIT_NUMBER, 2
.set SYS_LED_NUMBER, 3 
.set SYS_READ_NUMBER, 4	
.set SYS_WRITE_INT_NUMBER, 5 
.set SYS_CREATE_THREAD, 6
.set SYS_THREAD_SELF, 7
.set SYS_THREAD_JOIN, 8
.set SYS_THREAD_EXIT, 9
.set SYS_THREAD_SIGNAL, 10
.set SYS_LIST_NUMBER, 11
.set SYS_CD_NUMBER, 12
.set SYS_DUMP_NUMBER, 13
.set SYS_ROOT_NUMBER, 14
.set SYS_MOD_NUMBER, 15
.set SYS_SEND_NUMBER, 16

.globl user_delay
user_delay:
	subs x0, x0, #1
	bne user_delay
	ret

.globl call_sys_write
call_sys_write:
	mov w8, #SYS_WRITE_NUMBER	
	svc #0
	ret

.globl call_sys_write_int
call_sys_write_int:
	mov w8, #SYS_WRITE_INT_NUMBER	
	svc #0
	ret

.globl call_sys_exit
call_sys_exit:
	mov w8, #SYS_EXIT_NUMBER	
	svc #0
	ret

.globl call_sys_fork
call_sys_fork:
	mov w8, #SYS_FORK_NUMBER	
	svc #0
	ret

.globl call_sys_led
call_sys_led:
	mov w8, #SYS_LED_NUMBER
	svc #0
	ret

.globl call_sys_read
call_sys_read:
	mov w8, #SYS_READ_NUMBER
	svc #0
	ret

.globl thread_create
thread_create:
	mov w8, #SYS_CREATE_THREAD
	svc #0
	ret

.globl thread_self
thread_self:
	mov w8, #SYS_THREAD_SELF
	svc #0
	ret


.globl thread_join
thread_join:
	mov w8, #SYS_THREAD_JOIN
	svc #0
	ret

.globl thread_signal
thread_signal:
	mov w8, #SYS_THREAD_SIGNAL
	svc #0
	ret

.globl call_sys_list
call_sys_list:
	mov w8, #SYS_LIST_NUMBER
	svc #0
	ret

.globl call_sys_cd
call_sys_cd:
	mov w8, #SYS_CD_NUMBER
	svc #0
	ret

.globl call_sys_dump
call_sys_dump:
	mov w8, #SYS_DUMP_NUMBER
	svc #0
	ret

.globl call_sys_root
call_sys_root:
	mov w8, #SYS_ROOT_NUMBER
	svc #0
	ret

.globl call_sys_mod
call_sys_mod:
	mov w8, #SYS_MOD_NUMBER
	svc #0
	ret

.globl call_sys_send
call_sys_send:
	mov w8, #SYS_SEND_NUMBER
	svc #0
	ret

/* Return true if T1 and T2 both name the same thread.  Otherwise, false.  */
.globl thread_equal
thread_equal:
	cmp x0, x1
	beq eq
	mov x0, #0
	b neq
eq:     
	mov x0, #1
neq:	
	ret


