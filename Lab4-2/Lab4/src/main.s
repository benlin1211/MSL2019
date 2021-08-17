    .syntax unified
    .cpu cortex-m4
    .thumb
    .data
		arr: .byte 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47
		//0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F
		//TODO: put 0 to F 7-Seg LED pattern here
	.text
		.global main
		.equ RCC_AHB2ENR, 0x4002104C //address offset: 4C
    	.equ GPIOA_MODER, 0x48000000  //address offset: 00
    	.equ GPIOA_OTYPER, 0x48000004  //address offset: 04
    	.equ GPIOA_OSPEEDR, 0x48000008  //address offset: 08
   	 	.equ GPIOA_PUPDR, 0x4800000C  //address offset: 0C // pull_up address
    	//.equ GPIOA_ODR, 0x48000014  //address offset: 14
    	//----------
    	.equ GPIOA_BASE, 0x48000000
    	.equ DATA, 0x20 //send to PA5  000000(0010 0000)
		.equ LOAD, 0x40 //send to PA6  000000(0100 0000)
		.equ CLOCK, 0x80 //send to PA7 000000(1000 0000)
		//Data above are control by BSRR instead of ODR
    	.equ GPIO_BSRR_OFFSET, 0x18 //	修改ODR會一次改到整個GPIO port的值，若只需改動到某一個pin腳時可利用BSRR register存取
		.equ GPIO_BRR_OFFSET, 0x28
    	//----------
    	// 00:non op
    	// 01~08:digit 0~7
    	.equ DIGIT_0, 0x01
    	.equ DECODE_MODE, 0x09
    	.equ DISPLAY_TEST, 0x0F
    	.equ SCAN_LIMIT, 0x0B
    	.equ INTENSITY, 0x0A
    	.equ SHUTDOWN, 0x0C

	main:
		BL GPIO_init
		BL max7219_init
	loop:
		BL Display0toF
		B loop
	GPIO_init:
		//TODO: Initialize three GPIO pins as output for max7219 DIN, CS and CLK
		movs    r0, #0x1 //0000000(0001) :active A block
    	ldr     r1, =RCC_AHB2ENR
    	str     r0, [r1]

    	movs    r0, #0x5400 //00005400 = 0000(0101 0100 0000 0000)
    	ldr     r1, =GPIOA_MODER
    	ldr     r2, [r1]
    	//mask: keep from modifying the origin setting
    				 //FFFF03FF = FFFF(0000 0011 1111 1111)
    	and     r2, #0xFFFF03FF //mask
    	orrs    r2, r2, r0
    	str     r2, [r1]

    	movs    r0, #0xA800 //0000(1010 1000 0000 0000)
   		//(10):high speed
    	ldr     r1, =GPIOA_OSPEEDR
    	strh    r0, [r1]


		BX LR
	Display0toF:
		//TODO: Display 0 to F at first digit on 7-SEG LED. Display one per second.
		ldr r2, =arr
		mov r3, #0

		Loop:
		ldrb r1, [r2, r3]
		ldr r0, =DIGIT_0
		push {r2, r3, lr}
		BL MAX7219Send
		BL delay
		pop {r2, r3, lr}
		add r3, r3, #1
		cmp r3, #16
		bne Loop

		sub r3, r3, #1

		Loop2:
		ldrb r1, [r2, r3]
		ldr r0, =DIGIT_0
		push {r2, r3, lr}
		BL MAX7219Send
		BL delay
		pop {r2, r3, lr}
		sub r3, r3, #1
		cmp r3, #-1
		bne Loop2

		BX LR

	MAX7219Send:
		//The function of max7219_init
		//input parameter: r0 is ADDRESS , r1 is DATA
		//TODO: Use this function to send a message to max7219
		lsl r0, r0, #8 //addr is D11~D8, shift it!
		add r0, r0, r1
		ldr r1, =GPIOA_BASE
		ldr r2, =LOAD //latch
		ldr r3, =DATA
		ldr r4, =CLOCK
		ldr r5, =GPIO_BSRR_OFFSET
		ldr r6, =GPIO_BRR_OFFSET
		mov r7, #16 //r7 = i
		max7219send_loop: //while r7>0
		mov r8, #1
		sub r9, r7, #1
		lsl r8, r8, r9 // r8 = mask
		str r4, [r1,r6]//HAL_GPIO_WritePin(GPIOA, CLOCK, 0); ///
		tst r0, r8
		beq bit_not_set//bit not set
		str r3, [r1,r5]
		b if_done
		bit_not_set:
		str r3, [r1,r6]
		if_done:
		str r4, [r1,r5]
		subs r7, r7, #1
		bgt max7219send_loop
		str r2, [r1,r6] //latched
		str r2, [r1,r5]

		BX LR
	max7219_init:
		// Din is connneted to D13 (PA7)
		// operate pa7 to set Din
		//TODO: Initialize max7219 registers
		push {r0, r1, r2, lr}
		// In DECODE_MODE: 依照表格把D0~D3的值解碼成7-seg LED
		// Not: 將D0~D7直接顯示在7-seg LED上
		ldr r0, =DECODE_MODE //set how many digits are decoded
		ldr r1, =0x00 //no decode for all digit0~digit7
		BL MAX7219Send
		ldr r0, =DISPLAY_TEST // Test the leds are fine
		ldr r1, =0x0
		//0:normal operation
		//1:test
		BL MAX7219Send
		ldr r0, =SCAN_LIMIT //set how many digits are displayed
		ldr r1, =0x00 //0x00~0x07: display only digit0 => display digit0~digit7
		BL MAX7219Send
		ldr r0, =INTENSITY //brightness (0~F)
		ldr r1, =0xA
		BL MAX7219Send
		ldr r0, =SHUTDOWN
		ldr r1, =0x1
		//0:shutdown
		//1:operation
		BL MAX7219Send
		pop {r0, r1, r2, lr}
		BX LR
	delay:
    	LDR   R3,=3000
		L1: LDR R4,	=200
		L2: SUBS R4, #1
		BNE   L2
		SUBS  R3, #1
		BNE   L1
		BX    LR
