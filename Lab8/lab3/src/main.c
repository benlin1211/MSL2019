#include "stm32l476xx.h"
#include "cmsis_gcc.h"
#include "core_cm4.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cmSimd.h"
#include "system_stm32l4xx.h"

#include <string.h>
#include <stdlib.h>

#define SET_REG(REG,SELECT,VAL) {((REG)=((REG)&(~(SELECT)))|(VAL));};


char cmd[100];
int len = 0;
char id[7] = { '0', '6', '1', '0', '0', '3', '2' };
//ºñ±µ10¥Õ±µ9
int mode = 0;

int ADC1ConvertedValue;
int ADC1ConvertedVoltage;


void GPIO_Init(void) {
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);
	// UART:PA9 & PA10
	//TM_GPIO_Init(GPIOA, 9, TM_GPIO_Mode_AF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	GPIOA->MODER = (GPIOA->MODER & 0xFFF3FFFF) | (0b10 << 18);
	GPIOA->OTYPER = (GPIOA->OTYPER & 0xFFFFFDFF) | 0;
	GPIOA->PUPDR = GPIOA->PUPDR & 0xFFF3FFFF;
	GPIOA->OSPEEDR = GPIOA->OSPEEDR & 0xFFF3FFFF;
	//TM_GPIO_Init(GPIOA, 10, TM_GPIO_Mode_AF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	GPIOA->MODER = (GPIOA->MODER & 0xFFCFFFFF) | (0b10 << 20);
	GPIOA->OTYPER = (GPIOA->OTYPER & 0xFFFFFBFF) | 0;
	GPIOA->PUPDR = GPIOA->PUPDR & 0xFFCFFFFF;
	GPIOA->OSPEEDR = GPIOA->OSPEEDR & 0xFFCFFFFF;
	// AF7 for pin9&10
	GPIOA->AFR[1] = (GPIOA->AFR[1] & ~(0x00000FF0)) | 0x00000770;
	//(0111 0111)000000
	// BUTTON: PC13
	//TM_GPIO_Init(GPIOC, 13, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Medium);
	GPIOC->MODER = (GPIOC->MODER & 0xF3FFFFFF);
	GPIOC->OTYPER = (GPIOC->OTYPER & 0xFFFFDFFF) | 0;
	GPIOC->PUPDR = GPIOC->PUPDR & 0xF3FFFFFF;
	GPIOC->OSPEEDR = (GPIOC->OSPEEDR & 0xF3FFFFFF) | (0b01 << 26);
	// LED: pa5
	//TM_GPIO_Init(GPIOA, 10, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	GPIOA->MODER &=~ GPIO_MODER_MODER5_1;
	GPIOA->MODER |= GPIO_MODER_MODER5_0;
	GPIOA->OTYPER  &=~ GPIO_OTYPER_OT5;
	GPIOA->PUPDR  &=~ GPIO_PUPDR_PUPD5_1;
	GPIOA->PUPDR  |= GPIO_PUPDR_PUPD5_0;//01 pull up
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1;
	GPIOA->OSPEEDR &=~ GPIO_OSPEEDR_OSPEED5_0;//10 high

	//PC2: analog mode
	GPIOC->MODER |= GPIO_MODER_MODER2_1;
	GPIOC->MODER |= GPIO_MODER_MODER2_0;
	GPIOC->OTYPER  &=~ GPIO_OTYPER_OT2; //PP
	GPIOC->PUPDR  &=~ GPIO_PUPDR_PUPD2_1;
	GPIOC->PUPDR  &=~ GPIO_PUPDR_PUPD2_0;//00 no
	GPIOC->OSPEEDR &=~ GPIO_OSPEEDR_OSPEED2_1;
	GPIOC->OSPEEDR &=~ GPIO_OSPEEDR_OSPEED2_0;//00 low
	GPIOC->ASCR |= GPIO_ASCR_ASC2;
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
	SET_REG(USART1->CR1, USART_CR1_TE, 0);
	SET_REG(USART1->CR1, USART_CR1_RE, USART_CR1_RE);//1
	//recv interrupt enable bit
	SET_REG(USART1->CR1, USART_CR1_TXEIE, 0);//0
	SET_REG(USART1->CR1, USART_CR1_TCIE, 0);//0
	SET_REG(USART1->CR1, USART_CR1_RXNEIE, USART_CR1_RXNEIE);//1
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
	SET_REG(USART1->BRR, 0xFF, 4000000 / 9600);//0
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
	SET_REG(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN, RCC_APB2ENR_SYSCFGEN);
	SET_REG(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI13, SYSCFG_EXTICR4_EXTI13_PC);
	//Interrupt mask(in IMR1)
	SET_REG(EXTI->IMR1, EXTI_IMR1_IM13, EXTI_IMR1_IM13);
	//trigger: rise edge(in FTSR1)
	SET_REG(EXTI->FTSR1, EXTI_FTSR1_FT13, EXTI_FTSR1_FT13);
}

void NVIC_config()
{
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_EnableIRQ(USART1_IRQn);
}


int UART_Transmit(uint8_t *arr, uint32_t size) {
	//TODO: Send str to UART and return how many bytes are successfully transmitted.
	SET_REG(USART1->CR1, USART_CR1_TE, USART_CR1_TE);
	int i;
	while ((USART1->ISR & USART_ISR_TXE) == 0);
	for (i = 0; i < size; i++) {
		USART1->TDR = arr[i];
		while ((USART1->ISR & USART_ISR_TXE) == 0);
	}
	while ((USART1->ISR & USART_ISR_TC) == 0);

	return i;
}


//automatic handler
void EXTI15_10_IRQHandler(void)
{

	if ((EXTI->PR1 & EXTI_PR1_PIF13) == EXTI_PR1_PIF13)
	{
		unsigned char *s = "Hello Charlie. ";
		EXTI->PR1 = EXTI_PR1_PIF13;
		UART_Transmit(s, strlen(s));
		int a = 0;
		while (a<80000) a++;
	}
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);//clear pending
}

void USART1_IRQHandler(void){
	unsigned char c;

	while ((USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE){ //USART_CR1_RXNEIE?
		c = USART1->RDR;
	}
	int cmp=1;
	int none=1;
	if (len >= 100){
		if ( c == 127 || c == 8 ){ //backspace available
			if (len > 0){
				UART_Transmit(&c, 1);
				len --;
			}
		}
		else if(c == 13){//handle 100
			UART_Transmit("\r\n", 4);
			mode = 6;//inappropriate command
			len = 0;
		}
	}
	else if( ~TIM5->CR1 & TIM_CR1_CEN == TIM_CR1_CEN ){// not in light mode
		if (c == 13){ //\n
			UART_Transmit("\r\n", 4);
			cmd[len]= 0; //remind :\0
			cmp = strcmp(cmd, "showid");
			if (cmp == 0) {
				mode = 1;
				none = 0;
			}
			cmp = strcmp(cmd, "light");
			if (cmp == 0) {
				mode = 2;
				none = 0;
			}
			cmp = strcmp(cmd, "led on");
			if (cmp == 0) {
				mode = 3;
				none = 0;
			}
			cmp = strcmp(cmd, "led off");
			if (cmp == 0) {
				mode = 4;
				none = 0;
			}
			if (len == 0) {
				mode = 5;
				none = 0;
			};
			cmp = strcmp(cmd, "led show");
			if (cmp == 0) {
				mode = 7;
				none = 0;
			}
			// Finally, catch all inappropriate command
			if (none) mode = 6;
			len = 0;
		}
		else if (c == 127 || c == 8){ //back delete
			if (len > 0){
			//UART_Transmit("\r" , 2);
			//UART_Transmit(cmd , len-1);
			// no need to refresh
				UART_Transmit(&c, 1);
				len --;
			}
		}
		else{
			UART_Transmit(&c, 1);
			cmd[len++] = c;
		}
	}
	else if ( c == 'q'){ //exit light
		UART_Transmit(&c, 1);
		TIM5->CR1 &=~ TIM_CR1_CEN;
		UART_Transmit("\r\n", 4);
		UART_Transmit(">", 1);
	}
	/////////////////////handle///////////////////////
	if (mode){ //some function triggered
		if (mode == 1){ //showid
			UART_Transmit(id, strlen(id));
			UART_Transmit("\r\n", 4);
		}
		else if (mode == 2){ //light
			//if( ~TIM5->CR1 & TIM_CR1_CEN == TIM_CR1_CEN ) //0 check
			TIM5->CR1 |= TIM_CR1_CEN;
		}
		else if (mode == 3){//led on
			GPIOA->BSRR = 1 << 5; //set these gpio addresss to 1
		}
		else if (mode == 4){//led off
			GPIOA->BRR = 1 << 5; //set these gpio addresss to 0
		}
		else if (mode == 5){//enter
			//nothing
		}
		else if (mode == 6) {//others
			UART_Transmit("Unknown Command", 15);
			UART_Transmit("\r\n", 4);
		}
		else if (mode == 7) {//others
			if( (~GPIOA->ODR) & (1 << 5) == 0 ) UART_Transmit("off", 3);
			else UART_Transmit("on", 2);
			UART_Transmit("\r\n", 4);
		}
		// pending ">"
		if( mode !=2 ){
			UART_Transmit(">", 1);
		}
		mode = 0;
	}
	NVIC_ClearPendingIRQ(USART1_IRQn);
}



void Timer_init()

{	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;//timer5 clock enable
	TIM5->PSC = (uint32_t)(400-1);//Prescalser
	TIM5->ARR = (uint32_t)(5000-1);//Reload value
	TIM5->EGR = TIM_EGR_UG;//Reinitialize the counter.
	//4MHz * 400 * 10000 = 1(s)
}//update per 0.5s



int ADC_Init(){
	int calibration_value;
		//ADC1: one of ADC's instances
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN; //
	//ADC peripheral clock
	RCC->CCIPR |= (RCC_CCIPR_ADCSEL_1|RCC_CCIPR_ADCSEL_0); //as system clock

	ADC1->CFGR &=~ ADC_CFGR_RES_0;
	ADC1->CFGR &=~ ADC_CFGR_RES_1;//00:12-bit resolution
	ADC1->CFGR |= ADC_CFGR_OVRMOD;//1:Overrun mode

	//PC2 is the output of ADC"1" regular channel"3" configuration
	//(See Production data Table 16. STM32L476xx pin definitions)
	ADC1->SQR1 &= ~ADC_SQR1_L; //0000: Regular channel sequence length = 1
	// 1 conversion => sq1
	ADC1->SQR1 &=~ ADC_SQR1_SQ1_3;
	ADC1->SQR1 &=~ ADC_SQR1_SQ1_3;
	ADC1->SQR1 &=~ ADC_SQR1_SQ1_2;
	ADC1->SQR1 |= ADC_SQR1_SQ1_1;
	ADC1->SQR1 |= ADC_SQR1_SQ1_0;//0011 = channel"3"

	ADC1->SMPR1 |= (ADC_SMPR1_SMP3_1 |ADC_SMPR1_SMP3_0);
	// channel"3"
	//0x03:sampling time 7.5 clock cycle


	/*ADC1 calibration, Ensure DEEPPWD=0, ADVREGEN=1, ADEN = 0 */
	ADC1->CR &=~ ADC_CR_ADEN;
	ADC1->CR &=~ ADC_CR_DEEPPWD;
	ADC1->CR |= ADC_CR_ADVREGEN;
	/* The software must wait for the startup time of the ADC voltage regulator
	  (TADCVREG_STUP) before launching a calibration or enabling the ADC.*/
	//TADCVREG_STUP
	int a;
	a = 0;
	while(a <40000)a++;

 	// calibration
	ADC1->CR &= ~ADC_CR_ADCALDIF; //single-ended inputs mode
	ADC1->CR |= ADC_CR_ADCAL; //start ADC calibration
	//read in 1 means that a calibration in progress
	while(ADC1->CR & ADC_CR_ADCAL);//wait until calibration done
	calibration_value = ADC1->CALFACT;

	/*ADC1 configuration*/
	ADC1->CFGR |= ADC_CFGR_CONT; //continuous mode (won't stop)
	ADC1->CFGR &= ~ADC_CFGR_RES; //00:12-bit data resolution
	ADC1->CFGR &= ~ADC_CFGR_ALIGN; //Right alignment4

	ADC1->ISR |= ADC_ISR_ADRDY;
	ADC1->CR |= ADC_CR_ADEN;
	while(!ADC1->ISR & ADC_ISR_ADRDY );
	ADC1->CR |= ADC_CR_ADSTART;

	/*
	DEEPPWD=0.
	It is mandatory to enable the ADC internal voltage regulator by
	setting the bit ADVREGEN=1
	*/

	return calibration_value;
}

char* change_to_ASCII(int value){
	static char s[12];
	int digit = 0;
	int temp;
	temp = value;
	while(temp > 0){
		temp /= 10;
		digit = digit + 1;
	}
	temp = value;
	int i;
	s[digit] = 32;
	for(i=digit-1;i>=0;i--){
		s[i] = temp % 10 + 48;
		temp /= 10;
	}
	return s;
}

int main(void){
	GPIO_Init();
	EXTI_config();
	NVIC_config();
	USART1_Init();

	int timerValue;
	int t = 0;
	Timer_init();

	int calibration_value;
	calibration_value = ADC_Init();

	len = 0;
	int a = 0;
	while (a<5000) a++;
	UART_Transmit(">", 1);

	while (1){
		//detect
			//TODO: Polling the timer count and do lab requirements
		timerValue = TIM5->CNT;
		if (timerValue == 9  && (TIM5->CR1 & TIM_CR1_CEN == 1) ){//0~9
			while(!ADC1->ISR & ADC_ISR_ADRDY );
			ADC1ConvertedValue = ADC1->DR;
			//clean flag
			ADC1->ISR |= ADC_ISR_EOC;
			ADC1ConvertedVoltage = ADC1ConvertedValue* 2200 / 4096;
			char *s;
			s = change_to_ASCII(ADC1ConvertedVoltage);
			int d = strlen(s);
			UART_Transmit( s, d );
		}
	}
}
