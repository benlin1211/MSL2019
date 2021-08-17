    .syntax unified
    .cpu cortex-m4
    .thumb
    .data
    .text
        m: .word  0xFFFF00FF
        n: .word  0xAA00
    .global main
    .equ RCC_AHB2ENR, 0x4002104C

    .equ GPIOA_MODER, 0x48000000
    .equ GPIOA_OTYPER, 0x48000004
    .equ GPIOA_OSPEEDR, 0x48000008
    .equ GPIOA_PUPDR, 0x4800000C
    .equ GPIOA_IDR, 0x48000010

    .equ GPIOB_MODER, 0x48000400
    .equ GPIOB_OTYPER, 0x48000404
    .equ GPIOB_OSPEEDR, 0x48000408
    .equ GPIOB_PUPDR, 0x4800040C
    .equ GPIOB_ODR, 0x48000414

    .equ GPIOC_MODER, 0x48000800
    .equ GPIOC_IDR, 0x48000810

main:
    bl      GPIO_init
Loop:
    bl      Handle
    ldr     r0, [r5]
    movs    r4, #1
    lsl     r4, #13
    ands    r0, r0, r4

	it    eq
	beq   do_pushed
pedal:
    movs    r0, #0b1111000
    strh    r0, [r3]
    b       Loop

Compare:
    mov     r9, #1
    cmp     r2, #0b0011
    it      eq
    moveq   r9, #3
    b       DisplayLED

DisplayLED:
    movs    r6, #0
    strh    r6, [r3]
    bl      delay
    movs    r6, #0b1111000
    strh    r6, [r3]
    bl      delay

    sub     r9, #1
    cmp     r9, #0
    bne     DisplayLED

    b       out


Handle:
    ldr     r0, [r1]
    and     r2, r0, #0b11110000
    lsr     r2, r2, #4
    eor     r2, r2, #0b1111
    bx      lr

GPIO_init:
    //initia; a
    movs    r0, #0x7
    ldr     r1, =RCC_AHB2ENR
    str     r0, [r1]

    movs    r0, #0
    ldr     r1, =GPIOA_MODER
    ldr     r2, [r1]
    ldr     r3, m
    and     r2, r3
    orrs    r2, r2, r0
    str     r2, [r1]

    ldr     r4, n
    movs    r0, r4
    ldr     r1, =GPIOA_OSPEEDR
    strh    r0, [r1]

    ldr     r1, =GPIOA_IDR
    //initial b
    movs    r0, #0x1540
    ldr     r3, =GPIOB_MODER
    ldr     r2, [r3]
    and     r2, #0xFFFFC03F
    orrs    r2, r2, r0
    str     r2, [r3]

    movs    r0, #0x2A80
    ldr     r3, =GPIOB_OSPEEDR
    strh    r0, [r3]

    ldr     r3, =GPIOB_ODR
    movs    r5, #0b1111000
    strh    r5, [r3]
    //initial c
	ldr 	r5,	=GPIOC_MODER
	ldr 	r0,	[r5]		 //               r13
	ldr 	r2,	=#0xF3FFFFFF //Set Pin13 :F(0010)FFFFFF
	and 	r0,	r2
	str 	r0,	[r5]
	ldr 	r5,	=GPIOC_IDR
	bx      lr

delay:
    LDR   R7, = 1000
	L1: LDR R8,	= 250
	L2: SUBS R8, #1
	BNE   L2
	SUBS  R7, #1
	BNE   L1
	BX    LR

do_pushed:
    LDR   R7, = 100
	L3: LDR R8,	= 250
	L4: SUBS R8, #1
	BNE   L4
	SUBS  R7, #1
	BNE   L3

//again
	ldr   r0, [r5]
	movs  r4, #1
	lsl   r4, #13
	ands  r0, r0, r4
	bne   out
	//達到條件
    b     Compare
    out: b   pedal
