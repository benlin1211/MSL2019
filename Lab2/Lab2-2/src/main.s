.syntax unified
.cpu cortex-m4
.thumb

.data
    result: .word  0
    max_size:  .word  0
.text
    m: .word  0x4
    n: .word  0xC
.global main


exita:
   mov  r6, r1
   pop  {r7}
   pop  {pc}

exitb:
   mov  r6, r0
   pop  {r7}
   pop  {pc}

gcd:
   mov  r5, #1
   cmp  r0, #0    // a == 0
   beq  exita
   cmp  r1, #0    // b == 0
   beq  exitb
   mov  r2, r0
   mov  r3, r1
   and  r2, r2, #1   // a % 2 == 0
   and  r3, r3, #1   // b % 2 == 0
   cmp  r2, #0
   beq  state1
   cmp  r3, #0
   beq  state2
   subs r4, r0, r1   // abs(a - b)
   cmp  r4, #0
   it   lt
   neglt r4, r4
   cmp  r0, r1      // min(a, b)
   it   lt
   movlt r1, r0
   mov  r0, r4
   b    next

state1:
   lsr  r0, #1
   cmp  r3, #0
   it   eq
   moveq r5, #2
   beq  state2
   b    next

state2:
   lsr  r1, #1
   b    next

next:
   push {lr}
   push {r5}
   add  r8, r8, #2
   bl   gcd

right:
   mul  r6, r6, r7
   pop  {r7}
   pop  {pc}


main:
   ldr  r0, m
   ldr  r1, n
   mov  r8, #0
   mov  r9, #8
   bl   gcd
   mul  r6, r6, r7
   mul  r8, r8, r9
   ldr  r10, =result
   ldr  r11, =max_size
   str  r6, [r10]
   str  r8, [r11]
 L: b L

