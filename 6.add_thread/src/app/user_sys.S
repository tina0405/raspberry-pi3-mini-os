.section ".text.user"

.set SYS_WRITE_NUMBER, 0 		// syscal numbers 
.set SYS_FORK_NUMBER, 1 	
.set SYS_EXIT_NUMBER, 2
.set SYS_LED_NUMBER, 3 
.set SYS_READ_NUMBER, 4	
.set SYS_WRITE_INT_NUMBER, 5 
.set SYS_CREATE_THREAD, 6
.set SYS_THREAD_SELF, 7


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


