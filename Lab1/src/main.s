	.syntax unified
	.cpu cortex-m4
	.thumb
	.data
		result: .byte 0
	.text
		.global main
		.equ X, 0x1234
		.equ Y, 0x4567
	hamm:
		and r3, r0, #1 //r3:bit 1
		adds r4, r4, r3 //r4 count
		LSR r0, #1
		cmp r0, #0
		bne hamm
		bx lr
	main:
		movw r0, #X
		movw r1, #Y
		ldr r2, =result
		ldr r4, [r2]
		eor r0, r0, r1 //r0:eor of two reg
		bl hamm
		str r4, [r2]
	L: b L
