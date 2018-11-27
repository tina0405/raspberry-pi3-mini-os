#include "mm.h"

.section ".text.boot"

.globl _start
_start:
	mrs	x0, mpidr_el1		
	and	x0, x0,#0xFF		// Check processor id
	cbz	x0, master		// initial master CPU
	b	setup_stack

proc_hold: 
	b 	proc_hold

master:
	adr	x0, bss_begin
	adr	x1, bss_end
	sub	x1, x1, x0
	bl 	memzero

// each processor have to set up stack

setup_stack:
	mrs	x0, mpidr_el1		
	and	x0, x0,#0xFF		// Check processor id
    	mov 	x1, 0x4000        	// 2Kb for stack
	mul 	x1, x1, x0 		
    	mov 	x2, #LOW_MEMORY		// image adresses in 0x0; Start of stack pointer do not override image
	add 	x1, x1, x2		// base + offset
	mov	sp, x1 
	bl	kernel
	b 	proc_hold		// should never come here


