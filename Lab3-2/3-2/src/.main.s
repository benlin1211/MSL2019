    .syntax unified
    .cpu cortex-m4
    .thumb
    .data
      leds: .byte 0
      state: .byte 0 //state=0:move, state=1:halt
    .text
    .global main
// In STM32L4 system all GPIO port connect on AHB2 bus
// Port	A system memory address start from 0x48000000
// Port	B from 48000400
// each variable has its own offset address
    .equ RCC_AHB2ENR, 0x4002104C //address offset: 4C

    .equ GPIOB_MODER, 0x48000400  //address offset: 00
    .equ GPIOB_OTYPER, 0x48000404  //address offset: 04
    .equ GPIOB_OSPEEDR, 0x48000408  //address offset: 08
    .equ GPIOB_PUPDR, 0x4800040C  //address offset: 0C // pull_up address
    .equ GPIOB_ODR, 0x48000414  //address offset: 14
// r0 r1:general temporary loader

	.equ GPIOC_MODER, 0x48000800
    .equ GPIOC_IDR, 0x48000810

main:
    bl    GPIO_init
    movs  r7, #0b1000 //0b'00001000
    movs  r8, #1
	//movs  r1, #1 //
Loop:

//r1 for GPIOB_ODR addr, don't move(not anymore!)
//r2 for GPIOC_IDR addr, don't move
//r7, r8 for led control status, don't move
//r8=+-1:left or right
//r8=0:stop
//r3 for button status, don't move
//r2 for
//In loop you can use r0, r4, r5
	ldr   r6, =leds
    strb  r7, [r6]
    // r8 = +-1: left or right
    // move left
    cmp   r8, #1
    it    eq
    lsleq r7, #1
    // move right
    cmp   r8, #-1
    it    eq
    lsreq r7, #1

    cmp   r7, #16 // start : 0(001 0)000=>0(001 1)000
    it    eq
    addeq r7, #8
    cmp   r7, #192 // 1(100 0)000
    it    eq
    moveq r8, #-1
    cmp   r7, #12  // 0(000 1)100
    it    eq
    moveq r8, #1


//

	bl    DisplayLED
    bl    delay
    b	  Loop


DisplayLED:
// DisplayLED or not

    ldr  r5, [r6]
    eor  r5, r5, #0b1111000 //0b'01111000

	ldr  r1, =GPIOB_ODR
    strh r5, [r1]
    bx   lr


GPIO_init:
    movs    r0, #0x7 //0000000(0111) :active C&B&A block
    ldr     r1, =RCC_AHB2ENR
    str     r0, [r1]
						//                  r6 r5r4 r3
    movs    r0, #0x1540 //00001540 = 0000(0001 0101 0100 0000)
    //(01): general purpose
    ldr     r1, =GPIOB_MODER
    ldr     r2, [r1]
    //mask: keep from modifying the origin setting
    					//FFFFC03F = FFFF(1100 0000 0011 1111)
    and     r2, #0xFFFFC03F //mask
    orrs    r2, r2, r0
    str     r2, [r1]

    movs    r0, #0x2A80
   	//(10):high speed
    ldr     r1, =GPIOB_OSPEEDR
    strh    r0, [r1]

    //button//

	ldr 	r1,	=GPIOC_MODER
	ldr 	r0,	[r1]		 //               r13
	ldr 	r2,	=#0xF3FFFFFF //Set Pin13 :F(0010)FFFFFF
	and 	r0,	r2
	str 	r0,	[r1]

	ldr 	r2,	=GPIOC_IDR

	bx      lr



delay:
    LDR   R3,=200
	L1: LDR R4,	=2500
	L2: SUBS R4, #1
	BNE   L2

//button detect status:
	ldr   r0, [r2]
	movs  r1, #1  //                    r13
	lsl   r1, #13 // 00000001 => 0000(0001)000
	ands  r0, r0, r1 // extract pc13 status
	bne next      //!=0: don't do_pushed

	push {lr}
	bl   do_pushed //pc13=0:do_pushed
	pop {lr}
//
	next:SUBS  R3, #1
	BNE   L1
//check state
	ldr r0, =state
	ldr r1, [r0]
	cmp   r1, #0
	bne   delay
//
	BX    LR

do_pushed:
//debounce delay
    LDR   R0, =200
	L3: LDR R5, =250
	L4: SUBS R5, #1
	BNE   L4
	SUBS  R0, #1
	BNE   L3
//again
	ldr   r0, [r2]
	movs  r1, #1
	lsl   r1, #13
	ands  r0, r0, r1
	bne   out
	//pressed
	pressing:   ldr   r0, [r2]
			    ands  r0, r0, r1
			    beq	  pressing
//

	//debounce delay again
    LDR   R0, =200
	L5: LDR R5, =250
	L6: SUBS R5, #1
	BNE   L6
	SUBS  R0, #1
	BNE   L5

	ldr r0, =state
	ldr r1, [r0]  //equal 0: halt
	cmp r1, #0    //if equal 0
	mov r1, #0    //to 0 first
	it eq
	moveq   r1,#1 //if equal 0: to 1
	str r1, [r0]
    out: bx lr
