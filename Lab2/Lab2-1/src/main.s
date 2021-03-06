	.syntax unified
	.cpu cortex-m4
	.thumb
	.data
		user_stack: .zero 128
		expr_result:.word 0
	.text
		.global main

		postfix_expr: .asciz "-1 5    + "// no minus temporarily
	.align 4
	main:
		LDR R0, =postfix_expr //r0: postfix_expr_ptr
		ldr r4, =expr_result //r4: expr_result_ptr
		mov r6, #0 //r6: result reg
		mov r5, #10 //r5: constant (for MUL)
//TODO: Setup stack pointer to end of user_stack and calculate the
//expression using PUSH, POP operators, and store the result into expr_result

		mov r1, #0 //r1: byte move
		mov r3, #0 //r3: int loader
	load:
		ldrb r2, [r0, r1] //r2: byte loader
		cmp r2, #0 //0: end of string?
		beq save
		cmp r2, #32 //verifying the space
		beq next
		cmp r2, #43 //+
		beq ad
		cmp r2, #45 //-
		beq sub_or_neg
		// check '-' is number or operation?

		//store int
		bl atoi // bl: update lr
		push {r3}
		mov r3, #0
		b next
		//clear
	ad: //add and push back result
		pop {r8}
		pop {r7}
		add r7, r7, r8
		push {r7}
		b next
	sub_or_neg:
		add r1, r1, #1
		ldrb r2, [r0, r1]  //loading next byte to determine
		cmp r2, #32
		beq sb //it's sub

		//otherwise it's neg
		bl atoi // bl: update lr
		rsb r3, r3, #0  //reverse
		push {r3}
		mov r3, #0
		b next
	sb:
		pop {r8}
		pop {r7}
		sub r7, r7, r8
		push {r7}
///
	next:
		add r1, r1, #1 //skipping the space
		b load
	save:
		pop {r7}
		str r7, [r4]
/////////////////////////////////////////
	program_end:
		B program_end
	atoi:
//TODO: implement a ??convert string to integer?? function
		ldrb r2, [r0, r1]
		cmp r2, #32
		beq back
		mul r3, r3, r5
		sub r2, r2, #48
		add r3, r3, r2
		add r1, r1, #1
		b atoi
	back:
		bx lr //bx: branch by reg context

		//recursive
		//functionA:
		//push {LR}
		//BL functionB
		//POP {PC}
		//bx LR

