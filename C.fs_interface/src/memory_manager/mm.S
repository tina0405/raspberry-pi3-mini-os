.globl memcpy
memcpy:
	ldr x3, [x0], #8
	str x3, [x1], #8
	subs x2, x2, #8
	b.gt memcpy
	ret
.globl memcmp
memcmp:
	ldr x3, [x0], #8
	ldr x4, [x1], #8
	cmp x3,x4
        bne back	
	subs x2, x2, #8 
	b.gt memcmp
	mov x0, #0 
	ret
back:	
        mov x0, #1 
	ret

.globl memzero
memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret
