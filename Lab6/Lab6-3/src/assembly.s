    .syntax unified
    .cpu cortex-m4
    .thumb
	.text
		.global main
		.global GPIO_init
		.global max7219_send
		.global max7219_init
		.equ RCC_AHB2ENR, 0x4002104C //address offset: 4C
    	.equ GPIOA_MODER, 0x48000000  //address offset: 00
    	.equ GPIOA_OTYPER, 0x48000004  //address offset: 04
    	.equ GPIOA_OSPEEDR, 0x48000008  //address offset: 08
   	 	.equ GPIOA_PUPDR, 0x4800000C  //address offset: 0C // pull_up address

      	.equ GPIOB_MODER, 0x48000400  //address offset: 00
    	.equ GPIOB_OTYPER, 0x48000404  //address offset: 04
    	.equ GPIOB_OSPEEDR, 0x48000408  //address offset: 08
   	 	.equ GPIOB_PUPDR, 0x4800040C  //address offset: 0C // pull_up address
    	//.equ GPIOA_ODR, 0x48000014  //address offset: 14

    	.equ GPIOC_MODER, 0x48000800
    	 //----------
    	.equ DIGIT_0, 0x01
    	.equ DECODE_MODE, 0x09
    	.equ DISPLAY_TEST, 0x0F
    	.equ SCAN_LIMIT, 0x0B
    	.equ INTENSITY, 0x0A
    	.equ SHUTDOWN, 0x0C
    	 //----------
    	.equ GPIOA_BASE, 0x48000000
    	.equ DATA, 0x20 //send to PA5  000000(0010 0000)
		.equ LOAD, 0x40 //send to PA6  000000(0100 0000)
		.equ CLOCK, 0x80 //send to PA7 000000(1000 0000)
    	.equ GPIO_BSRR_OFFSET, 0x18 //	修改ODR會一次改到整個GPIO port的值，若只需改動到某一個pin腳時可利用BSRR register存取
		.equ GPIO_BRR_OFFSET, 0x28
    	//---------
	GPIO_init:
		push {r1-r11, lr}


		movs    r0, #0x7 //0000000(0111)
    	ldr     r1, =RCC_AHB2ENR
    	str     r0, [r1]

    	movs    r0, #0x5400
    	add     r0, r0, #1 //00005401 = 0000(0101 0100 0000 0001)
    	ldr     r1, =GPIOA_MODER
    	ldr     r2, [r1]
    	//mask: keep from modifying the origin setting

    	and     r2, #0xFFFF03FF
    	sub     r2, r2, #0b11  //mask
    	//FFFF03FC = FFFF(0000 0011 1111 1100)

    	orrs    r2, r2, r0
    	str     r2, [r1]

    	movs    r0, #0xA800
    	add     r0, r0, #2 //0000(1010 1000 0000 0010)
   		//(10):high speed
    	ldr     r1, =GPIOA_OSPEEDR
    	strh    r0, [r1]

		//BUTTON
		ldr 	r1,	=GPIOC_MODER
		ldr 	r0,	[r1]		 //
		ldr 	r2,	=#0xF3FFFFFF //Set Pin13 :F(0011)FFFFFF
		and 	r0,	r2
		str 	r0,	[r1]

		pop {r1-r11, pc}

	max7219_init:
		// Din is connneted to D13 (PA7)
		// operate pa7 to set Din
		//TODO: Initialize max7219 registers
		push {r1-r11, lr}
		// In DECODE_MODE: 依照表格把D0~D3的值解碼成7-seg LED
		// Not: 將D0~D7直接顯示在7-seg LED上
		ldr r0, =DECODE_MODE //set how many digits are decoded
		ldr r1, =0xff //no decode for all digit0~digit7
		BL max7219_send
		ldr r0, =DISPLAY_TEST // Test the leds are fine
		ldr r1, =0x0
		//0:normal operation
		//1:test
		BL max7219_send
		ldr r0, =SCAN_LIMIT //set how many digits are displayed
		ldr r1, =0x07 //0x00~0x07: display only digit0 => display digit0~digit7
		BL max7219_send
		ldr r0, =INTENSITY //brightness (0~F)
		ldr r1, =0xA
		BL max7219_send
		ldr r0, =SHUTDOWN
		ldr r1, =0x1
		//0:shutdown
		//1:operation
		BL max7219_send
		pop {r1-r11, pc}


	max7219_send:
		push {r1-r11, lr}
		//The function of max7219_init
		//input parameter: r0 is ADDRESS(1~8) , r1 is DATA
		//TODO: Use this function to send a message to max7219
		//
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

		pop {r1-r11, pc}
