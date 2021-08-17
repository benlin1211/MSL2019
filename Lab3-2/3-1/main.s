    .syntax unified
    .cpu cortex-m4
    .thumb
    .data
      leds: .byte 0
    .text
    .global main
    .equ RCC_AHB2ENR, 0x4002104C

    .equ GPIOB_MODER, 0x48000400
    .equ GPIOB_OTYPER, 0x48000404
    .equ GPIOB_OSPEEDR, 0x48000408
    .equ GPIOB_PUPDR, 0x4800040C
    .equ GPIOB_ODR, 0x48000414

main:
    bl    GPIO_init
    movs  r7, #0b1000
    ldr   r6, =leds
    movs  r8, #1

Loop:
    strb  r7, [r6]
    cmp   r8, #1
    it    eq
    lsleq r7, #1
    cmp   r8, #-1
    it    eq
    lsreq r7, #1

    cmp   r7, #16
    it    eq
    addeq r7, #8
    cmp   r7, #12
    it    eq
    moveq r8, #1
    cmp   r7, #192
    it    eq
    moveq r8, #-1

    bl    DisplayLED
    bl    delay
    b	  Loop

DisplayLED:
    ldr  r5, [r6]
    eor  r5, r5, #0b1111000
    strh r5, [r1]
    bx   lr

GPIO_init:
    movs    r0, #0x2
    ldr     r1, =RCC_AHB2ENR
    str     r0, [r1]

    movs    r0, #0x1540
    ldr     r1, =GPIOB_MODER
    ldr     r2, [r1]
    and     r2, #0xFFFFC03F
    orrs    r2, r2, r0
    str     r2, [r1]

    movs    r0, #0x2A80
    ldr     r1, =GPIOB_OSPEEDR
    strh    r0, [r1]

    ldr     r1, =GPIOB_ODR
    bx      lr

delay:
    LDR   R3, =2000
	L1: LDR R4,	=250
	L2: SUBS R4, #1
	BNE   L2
	SUBS  R3, #1
	BNE   L1
	BX    LR
