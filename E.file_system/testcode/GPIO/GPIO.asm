	.arch armv8-a
	.file	"GPIO.c"
	.comm	task,512,8
	.text
	.align	2
	.global	DELAY
	.type	DELAY, %function
DELAY:
	sub	sp, sp, #16
	str	x0, [sp, 8]
#APP
// 7 "GPIO.c" 1
	delay: subs x0, x0, #1
bne delay
ret
// 0 "" 2
#NO_APP
	nop
	add	sp, sp, 16
	ret
	.size	DELAY, .-DELAY
	.comm	input,16,8
	.global	drv_config
	.data
	.align	3
	.type	drv_config, %object
	.size	drv_config, 12
drv_config:
	.word	2
	.word	8
	.word	4
	.section	.rodata
	.align	3
.LC0:
	.string	"Initial GPIO component!\n\r"
	.align	3
.LC1:
	.string	"set_gpio"
	.text
	.align	2
	.global	init_compt
	.type	init_compt, %function
init_compt:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	kservice_uart_write
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	kservice_reg_compt
	cmp	w0, 0
	bne	.L4
	adrp	x0, drv_config
	add	x0, x0, :lo12:drv_config
	bl	kservice_config_compt
.L4:
	nop
	ldp	x29, x30, [sp], 16
	ret
	.size	init_compt, .-init_compt
	.section	.rodata
	.align	3
.LC2:
	.string	"GPIO Operation!\n\r"
	.text
	.align	2
	.global	oprt_compt
	.type	oprt_compt, %function
oprt_compt:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	stp	x0, x1, [x29, 16]
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	bl	kservice_uart_write
	ldr	w0, [x29, 24]
	mov	w1, w0
	mov	x0, 148
	movk	x0, 0x20, lsl 16
	bl	kservice_put32
	mov	w0, 150
	bl	delay
	ldr	x0, [x29, 16]
	mov	w1, w0
	mov	w0, 1
	lsl	w0, w0, w1
	mov	w1, w0
	mov	x0, 152
	movk	x0, 0x20, lsl 16
	bl	kservice_put32
	mov	w0, 150
	bl	delay
	mov	w1, 0
	mov	x0, 152
	movk	x0, 0x20, lsl 16
	bl	kservice_put32
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	oprt_compt, .-oprt_compt
	.section	.rodata
	.align	3
.LC3:
	.string	"Clean up GPIO component!\n\r"
	.text
	.align	2
	.global	exit_compt
	.type	exit_compt, %function
exit_compt:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	kservice_unreg_compt
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	kservice_uart_write
	nop
	ldp	x29, x30, [sp], 16
	ret
	.size	exit_compt, .-exit_compt
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
