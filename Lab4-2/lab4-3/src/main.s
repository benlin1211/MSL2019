    .syntax unified
    .cpu cortex-m4
    .thumb
    .data
		arr: .byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
		state: .byte 0 //state=0:wait, state=1:pressed, state=2:init
		digits: .byte 0x00 // how many digits, 0 = 1 digit ,7 = 8 digits
		//0,1,2,3,4,5,6,7,8,9
		//TODO: put decoded value
	.text
		.global main
		.equ RCC_AHB2ENR, 0x4002104C //address offset: 4C
    	.equ GPIOA_MODER, 0x48000000  //address offset: 00
    	.equ GPIOA_OTYPER, 0x48000004  //address offset: 04
    	.equ GPIOA_OSPEEDR, 0x48000008  //address offset: 08
   	 	.equ GPIOA_PUPDR, 0x4800000C  //address offset: 0C // pull_up address
    	//.equ GPIOA_ODR, 0x48000014  //address offset: 14
    	.equ GPIOC_MODER, 0x48000800
    	.equ GPIOC_IDR, 0x48000810
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

    	.equ DECODE_MODE, 0x09
    	.equ DISPLAY_TEST, 0x0F
    	.equ SCAN_LIMIT, 0x0B
    	.equ INTENSITY, 0x0A
    	.equ SHUTDOWN, 0x0C

	main:
		BL GPIO_init
		BL max7219_init
		ldr r0, =state
		mov r1, #0
		str r1, [r0]

	Fibo:

		mov r0, #0x01
		mov r1, #0
		push {r0, r1, lr}
		BL MAX7219Send
		pop {r0, r1, lr}
		add r0, r0, #1
		mov r1, #15
		L:
			push {r0, r1, lr}
			BL MAX7219Send
			pop {r0, r1, lr}
			add r0, r0, #1
			cmp r0, #0x09
			bne L
		//eliminate pressing
		ldr r0, =state
		ldr r1, [r0]
		cmp r1 ,#2
		bne init //for first pass
		movs  r1, #1
		lsl   r1, #13
		ldr   r2, =GPIOC_IDR
		Keep_pressing:
			ldr   r0, [r2]
			ands  r0, r0, r1
			beq	  Keep_pressing
		init:
		mov r1, #1
		mov r2, #1
		mov r3, #0
		//here to eliminate press
		Next_loop:
		mov r0, #0x1//digit0
		push {r0, r1, r2, r3, lr}
		wait_for_b:
			ldr r0, =state
			mov r1, #0
			str r1, [r0]

			ldr   r2, =GPIOC_IDR
			ldr   r0, [r2]
			movs  r1, #1  //                    r13
			lsl   r1, #13 // 00000001 => 0000(0001)000
			ands  r0, r0, r1 // extract pc13 status
			bne wait_for_b      //!=0: don't do_pushed
			bl  do_pushed //pc13=0:do_pushed
			//pressed check
			ldr r0, =state
			ldr r1, [r0]
			cmp r1, #0 //equal 0: wait
			beq wait_for_b
			//check init
			cmp r1 ,#2
			beq Fibo
	//display
		pop {r0, r1, r2, r3, lr} // reflesh r0 & r1 for display
		//push {r2, r3, lr}
		mov r5, #10
		//input parameter: r0 is ADDRESS , r1 is DATA
		transfer:
			mov r6, r1 //used for mod
			udiv r1, r1, r5
			mov r4, r1
			cmp r1, #0
			mul r1, r1, r5
			sub r1, r6, r1
			//Carry:
			//lsl r0, #1
			//add r0, r0, #1
			push {r0, r1, r2, r3, r4, r5, r6, lr}
			BL MAX7219Send//
			pop {r0, r1, r2, r3, r4, r5, r6, lr}
			//next digit
			//= 9:over
			cmp r4 ,#0
			beq next
			mov r1, r4
			add r0, r0, #1
			cmp r0 ,#9
			beq Overflow
			b transfer

		//pop {r2, r3, lr}
		next:
		add r1, r2, r3
		mov r3, r2
		mov r2, r1

		//detect overflow
		//Overflow://??

		//send to digit0~7

		ldr r7, =state
		ldr r8, [r7]
		cmp r8, #2
		beq Fibo
		b   Next_loop
	Overflow:
		mov r0, #0x01
		mov r1, #1
		push {r0, r1, lr}
		BL MAX7219Send
		pop {r0, r1, lr}

		add r0, r0, #1
		mov r1, #10
		push  {r0, r1, lr}
		BL MAX7219Send
		pop  {r0, r1, lr}

		add r0, r0, #1
		mov r1, #15 //blank
		Loop_Overflow:
			push  {r0, r1, lr}
			BL MAX7219Send
			pop  {r0, r1, lr}
			add r0, r0, #1
			cmp r0, #0x09
			bne Loop_Overflow
		locked:
			ldr   r2, =GPIOC_IDR
			ldr   r0, [r2]
			movs  r1, #1  //                    r13
			lsl   r1, #13 // 00000001 => 0000(0001)000
			ands  r0, r0, r1 // extract pc13 status
			bne locked      //!=0: don't do_pushed
			bl  do_pushed
			//check init
			ldr r0, =state
			ldr r1, [r0]
			cmp r1 ,#2
			beq Fibo
		b   locked

//-------------------------------------
	do_pushed:
	//debounce delay
	    LDR   R0, =150
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
	//normal display
		ldr r0, =state
		mov r1, #1
		str r1, [r0]
	//
		movs  r1, #1
		lsl   r1, #13
	//pressed
		mov   r3 ,#0
		mov   r4, #1
		lsl   r4, #18
		pressing:
			add   r3, #1
			cmp   r3, r4
			beq   reset
			ldr   r2, =GPIOC_IDR
			ldr   r0, [r2]
			ands  r0, r0, r1
			beq	  pressing
			bx lr
		reset:
			ldr r0, =state//#modify
			mov r1, #2
			str r1, [r0]
	    out: bx lr

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
	GPIO_init:
		//TODO: Initialize three GPIO pins as output for max7219 DIN, CS and CLK
		movs    r0, #0x5 //0000000(0101) :active A block
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
		//button//

		ldr 	r1,	=GPIOC_MODER
		ldr 	r0,	[r1]		 //
		ldr 	r2,	=#0xF2FFFFFF //Set Pin13 :F(0010)FFFFFF
		and 	r0,	r2
		str 	r0,	[r1]

		BX LR

	max7219_init:
		// Din is connneted to D13 (PA7)
		// operate pa7 to set Din
		//TODO: Initialize max7219 registers
		push {r0, r1, r2, lr}
		// In DECODE_MODE: 依照表格把D0~D3的值解碼成7-seg LED
		// Not: 將D0~D7直接顯示在7-seg LED上
		ldr r0, =DECODE_MODE //set how many digits are decoded
		ldr r1, =0xff //(1111 1111):decode for all digit7~digit0
		BL MAX7219Send
		ldr r0, =DISPLAY_TEST // Test the leds are fine
		ldr r1, =0x0
		//0:normal operation
		//1:test
		BL MAX7219Send
		ldr r0, =SCAN_LIMIT //set how many digits are displayed
		ldr r1, =0x07 //0x00~0x07: display only digit0 => display digit0~digit7
		// will be changed!
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


