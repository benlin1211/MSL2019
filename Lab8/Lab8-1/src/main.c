#include "stm32l476xx.h"
#include "cmsis_gcc.h"
#include "core_cm4.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cmSimd.h"
#include "system_stm32l4xx.h"

#include <string.h>
#define SET_REG(REG,SELECT,VAL) {((REG)=((REG)&(~(SELECT)))|(VAL));};

//????10?ձ?9

void GPIO_Init(void) {
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);
// UART:PA9 & PA10
	//TM_GPIO_Init(GPIOA, 9, TM_GPIO_Mode_AF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	GPIOA->MODER=(GPIOA->MODER&0xFFF3FFFF)|(0b10<<18);
	GPIOA->OTYPER=(GPIOA->OTYPER&0xFFFFFDFF)|0;
	GPIOA->PUPDR=GPIOA->PUPDR&0xFFF3FFFF;
	GPIOA->OSPEEDR=GPIOA->OSPEEDR&0xFFF3FFFF;
	//TM_GPIO_Init(GPIOA, 10, TM_GPIO_Mode_AF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	GPIOA->MODER=(GPIOA->MODER&0xFFCFFFFF)|(0b10<<20);
	GPIOA->OTYPER=(GPIOA->OTYPER&0xFFFFFBFF)|0;
	GPIOA->PUPDR=GPIOA->PUPDR&0xFFCFFFFF;
	GPIOA->OSPEEDR=GPIOA->OSPEEDR&0xFFCFFFFF;
	// AF7 for pin9&10
	GPIOA->AFR[1] = (GPIOA->AFR[1] & ~(0x00000FF0)) | 0x00000770;
	//(0111 0111)000000
// BUTTON: PC13
	//TM_GPIO_Init(GPIOC, 13, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Medium);
	GPIOC->MODER=(GPIOC->MODER&0xF3FFFFFF);
	GPIOC->OTYPER=(GPIOC->OTYPER&0xFFFFDFFF)|0;
	GPIOC->PUPDR=GPIOC->PUPDR&0xF3FFFFFF;
	GPIOC->OSPEEDR=(GPIOC->OSPEEDR&0xF3FFFFFF)|(0b01<<26);
}

//PA9 & PA10 => USART1
void USART1_Init(void) {
/* Enable clock for USART1 */
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
// CR1 /* Receiver Enable & Transmitter Enable */
	//MODIFY_REG(USART1->CR1, USART_CR1_M | USART_CR1_PS | USART_CR1_PCE | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8, USART_CR1_TE | USART_CR1_RE);

	SET_REG(USART1->CR1, USART_CR1_M, USART_CR1_M0);//00: 1 Start bit, 8+1 data bits, n stop bits
	SET_REG(USART1->CR1, USART_CR1_PS, 0);//0: Even parity
	SET_REG(USART1->CR1, USART_CR1_PCE, USART_CR1_PCE);//1: the computed parity is inserted at the MSB position(9th bit if M=1)
	SET_REG(USART1->CR1, USART_CR1_RE, 0);//0: Receiver is disabled
	SET_REG(USART1->CR1, USART_CR1_OVER8, 0);//0: Oversampling by 16; 1: Oversampling by 8

// CR2
	//MODIFY_REG(USART1->CR2, USART_CR2_STOP, 0x0); // 1-bit stop
	SET_REG(USART1->CR2, USART_CR2_STOP, 0);//00: 1 stop bit

// CR3
	//MODIFY_REG(USART1->CR3, (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT), 0x0); // none hwflow control
	SET_REG(USART1->CR3, USART_CR3_RTSE, 0);//0
	SET_REG(USART1->CR3, USART_CR3_CTSE, 0);//0
	SET_REG(USART1->CR3, USART_CR3_ONEBIT, 0);//0
// BRR
	//MODIFY_REG(USART1->BRR, 0xFF, 4000000L/115200);
	SET_REG(USART1->BRR, 0xFF, 4000000/115200);//0
	//clk rate = 4MHz (MSI)

/*
In asynchronous mode, the following bits must be kept cleared:
- LINEN and CLKEN bits in the USART_CR2 register,
- SCEN, HDSEL and IREN bits in the USART_CR3 register.
*/
	USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
	USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);

// Finally, enable UART

	USART1->CR1 |= (USART_CR1_UE);
}

void EXTI_config()
{
	SET_REG(RCC->APB2ENR,RCC_APB2ENR_SYSCFGEN,RCC_APB2ENR_SYSCFGEN);
	SET_REG(SYSCFG->EXTICR[3],SYSCFG_EXTICR4_EXTI13,SYSCFG_EXTICR4_EXTI13_PC);
	//Interrupt mask(in IMR1)
	SET_REG(EXTI->IMR1,EXTI_IMR1_IM13,EXTI_IMR1_IM13);
	//trigger: rise edge(in FTSR1)
	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT13,EXTI_FTSR1_FT13);
}

void NVIC_config()
{
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}


//automatic handler
void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR1 & EXTI_PR1_PIF13) == EXTI_PR1_PIF13)
	{
		EXTI->PR1 = EXTI_PR1_PIF13;
		char *s = "Hello World! ";
		UART_Transmit(s, strlen(s));
		int a=0;
		while ( a<80000 ) a++;
	}
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);//clear pending
}

int UART_Transmit(uint8_t *arr, uint32_t size) {
//TODO: Send str to UART and return how many bytes are successfully transmitted.
	SET_REG(USART1->CR1, USART_CR1_TE, USART_CR1_TE);//1: Transmitter is enabled
	int i;
	int a;
	while ( ( USART1->ISR & USART_ISR_TXE )== 0 );
	for (i = 0; i < size; i++) {
		//The TE bit should not be reset(0) during transmission of data.
		USART1->TDR = arr[i]; //Transmit data register
		while ( ( USART1->ISR & USART_ISR_TXE )== 0 );
		//wait until TXE=1.
		//This indicates that the transmission of the last frame is complete.
	}
	//while (!IS_UART_TRANS_DONE);//lock
	while ( ( USART1->ISR & USART_ISR_TC )== 0 );
	//SET_REG(USART1->CR1, USART_CR1_TE, 0);//1
	return i;
}

int main(void){
	GPIO_Init();
	EXTI_config();
	NVIC_config();
	USART1_Init();

	while(1){
		//detect
	}
}
