.globl get_el
get_el:
	mrs x0, CurrentEL
	lsr x0, x0, #2
	ret

.globl put32
put32:
	str w1,[x0]
	ret

.globl get32
get32:
	ldr w0,[x0]
	ret

.globl delay
delay:
	subs x0, x0, #1
	bne delay
	ret

.globl set_pgd
set_pgd:
	msr	ttbr0_el1, x0
	tlbi vmalle1is
  	DSB ISH              // ensure completion of TLB invalidation
	isb
	ret

.globl get_pgd
get_pgd:
	mov x1, 0
	ldr x0, [x1]
	mov x0, 0x1000
	msr	ttbr0_el1, x0
	ldr x0, [x1]
	ret

.globl try_lock
1:      
	mov      w4, #1
	mov  	 w0, w4
	ret
try_lock: /*w3  w4 tmp*/
	ldaxr    w3, [x0] /*x0:lock*/
        cbnz     w3, 1b
	add      w3, w3, #1	
        stxr     w4, w3, [x0]
        cbnz     w4, 1b	
	mov  	 w0, w4
	ret


.globl unlock
loop:  bl schedule
unlock: /*w3 w4 tmp*/
	ldxr    w3, [x0]
	cbz     w3, error_unlock       
        sub     w3, w3, #1
    	stlxr   w4, w3, [x0]
        cbnz    w4, loop
	mov     w0, w4
	ret

/*
.globl semaphore_take;
.type semaphore_take, %function
semaphore_take:
     	mov   w2, 1
    	ldaxr   w1, [x0]    	
	stxr    w3, w2, [x0]
    	cbnz    w3, semaphore_take
    	cbnz    w1, semaphore_take
	dmb ish
   	ret
.size	semaphore_take, .-semaphore_take
*/

.globl error_unlock  
error_unlock:
	mov      w0, #1
	ret

.globl bl_init
bl_init:
	mov     x8, x30	
	blr	x0
	mov     x30, x8
	ret

