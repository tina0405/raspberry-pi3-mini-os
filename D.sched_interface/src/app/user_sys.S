.section ".text.user"

.set SYS_WRITE_NUMBER, 0 		// syscal numbers 
.set SYS_FORK_NUMBER, 1 	
.set SYS_EXIT_NUMBER, 2
.set SYS_LED_NUMBER, 3 
.set SYS_READ_NUMBER, 4	
.set SYS_CREATE_THREAD, 5
.set SYS_THREAD_SELF, 6
.set SYS_THREAD_JOIN, 7
.set SYS_THREAD_EXIT, 8
.set SYS_THREAD_SIGNAL, 9
.set SYS_LIST_NUMBER, 10
.set SYS_CD_NUMBER, 11
.set SYS_DUMP_NUMBER, 12
.set SYS_ROOT_NUMBER, 13
.set SYS_MOD_NUMBER, 14
.set SYS_SEND_NUMBER, 15
.set SYS_RECV_NUMBER, 16 /*here*/
.set SYS_TRYLOCK_NUMBER, 17
.set SYS_LOCK_NUMBER, 18
.set SYS_UNLOCK_NUMBER, 19
.set SYS_COM_NUMBER, 20
.set SYS_ALLOCP_NUMBER, 21
.set SYS_FREEP_NUMBER, 22
.set SYS_RMCOM_NUMBER, 23
.set SYS_LSDEV_NUMBER, 24
.set SYS_LSCOM_NUMBER, 25
.set SYS_SCHED_NUMBER, 26
.set SYS_FOPEN_NUMBER, 27
.set SYS_FWRITE_NUMBER, 28
.set SYS_FCLOSE_NUMBER, 29
.set SYS_FSCANF_NUMBER, 30


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

.globl thread_cond_signal
thread_cond_signal:
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

.globl call_sys_run
call_sys_run:
	mov w8, #SYS_MOD_NUMBER
	svc #0
	ret

.globl call_sys_com
call_sys_com:
	mov w8, #SYS_COM_NUMBER
	svc #0
	ret

.globl call_sys_send
call_sys_send:
	mov w8, #SYS_SEND_NUMBER
	svc #0
	ret

.globl call_sys_recv
call_sys_recv:
	mov w8, #SYS_RECV_NUMBER
	svc #0
	ret

.globl thread_mutex_trylock
thread_mutex_trylock:
	mov w8, #SYS_TRYLOCK_NUMBER
	svc #0
	ret

.globl thread_mutex_lock
thread_mutex_lock:
	mov w8, #SYS_LOCK_NUMBER
	svc #0
	ret

.globl thread_mutex_unlock
thread_mutex_unlock:
	mov w8, #SYS_UNLOCK_NUMBER
	svc #0
	ret

.globl call_sys_allocate_page
call_sys_allocate_page:
	mov w8, #SYS_ALLOCP_NUMBER
	svc #0
	ret

.globl call_sys_free_page
call_sys_free_page:
	mov w8, #SYS_FREEP_NUMBER
	svc #0
	ret

.globl call_sys_rmcom
call_sys_rmcom:
	mov w8, #SYS_RMCOM_NUMBER
	svc #0
	ret

.globl call_sys_lsdev
call_sys_lsdev:
	mov w8, #SYS_LSDEV_NUMBER
	svc #0
	ret

.globl call_sys_lscom
call_sys_lscom:
	mov w8, #SYS_LSCOM_NUMBER
	svc #0
	ret

.globl call_sys_sched
call_sys_sched:
	mov w8, #SYS_SCHED_NUMBER
	svc #0
	ret

.globl call_sys_fopen
call_sys_fopen:
	mov w8, #SYS_FOPEN_NUMBER
	svc #0
	ret

.globl call_sys_fwrite
call_sys_fwrite:
	mov w8, #SYS_FWRITE_NUMBER
	svc #0
	ret

.globl call_sys_fclose
call_sys_fclose:
	mov w8, #SYS_FCLOSE_NUMBER
	svc #0
	ret

.globl call_sys_fscanf
call_sys_fscanf:
	mov w8, #SYS_FSCANF_NUMBER
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

.globl sp_info
sp_info:
	ldr x0, [sp]
	ret
