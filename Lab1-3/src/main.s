	.syntax unified
	.cpu cortex-m4
	.thumb
	.data //to be sorted
		arr1: .byte 0x19, 0x34, 0x55, 0x32, 0x12, 0x23, 0x61, 0x29
		arr2: .byte 0x18, 0x17, 0x33, 0x16, 0xFA, 0x20, 0x55, 0xAC
	.text
		.global main

	do_sort:
	//TODO
		movs r2, #7
	begin_sort:
		movs r1, #0
	loop:
		ldrb r3, [r0, r1] //r3 & r4
		add r1, r1, #1
		ldrb r4, [r0, r1]
		cmp r3, r4
		bgt no_swap
		mov r5, r3 //r5:tmp
		mov r3, r4
		mov r4, r5
		sub r1, r1, #1
		strb r3, [r0, r1]
		add r1, r1, #1
		strb r4, [r0, r1]
	no_swap:
		cmp r1, r2
		bne loop
	//swap

		sub r2, r2, #1
		cmp r2, #1
		bne begin_sort
		bx lr

	main:
		ldr r0, =arr1
		bl do_sort
		ldr r0, =arr2
		bl do_sort
	L: b L

