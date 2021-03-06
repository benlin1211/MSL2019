    .syntax unified
    .cpu cortex-m4
    .thumb
    .data
    	state: .byte 0
	.text
		.global main
		.global delay
		.equ GPIOC_IDR, 0x48000810


delay:
	push {r1-r11, lr}
    LDR   R3,=2500
	L1: LDR R4,	=200
	L2: SUBS R4, #1
	BNE   L2

//button detect status:
	ldr   r2, =GPIOC_IDR
	ldr   r0, [r2]
	movs  r1, #1  //                    r13
	lsl   r1, #13 // 00000001 => 0000(0001)000
	ands  r0, r0, r1 // extract pc13 status
	bne next      //!=0: don't do_pushed

	bl   do_pushed //pc13=0:do_pushed
//
	next:SUBS  R3, #1
	BNE   L1
//check state
//	ldr r0, =state
//	ldr r1, [r0]
//	cmp   r1, #0
//	bne   delay
	pop {r1-r11, pc}

do_pushed:
//debounce delay
	push {r1-r11, lr}

    LDR   R0, =200
	L3: LDR R5, =250
	L4: SUBS R5, #1
	BNE   L4
	SUBS  R0, #1
	BNE   L3
//again
	ldr   r2, =GPIOC_IDR
	ldr   r0, [r2]
	movs  r1, #1
	lsl   r1, #13
	ands  r0, r0, r1
	bne   out
	//pressed
	pressing:   ldr   r0, [r2]
			    ands  r0, r0, r1
			    beq	  pressing
//event

//End event
    out: pop {r1-r11, pc}
