#include "stm32l476xx.h"
#include "cmsis_gcc.h"
#include "core_cm4.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cmSimd.h"
#include "system_stm32l4xx.h"



#define SET_REG(REG,SELECT,VAL) {((REG)=((REG)&(~(SELECT)))|(VAL));};
// You can use your way to store TIME_SEC. Maybe it is `int` or `float` or any you want
#define TIME_SEC 6.12
#define ZERO 0x7e
#define SPACE 0

extern void GPIO_init();
extern void max7219_init();
//extern void Display();
extern void max7219_send(int address, int data);

void SystemClock_Config()
{
	RCC->CR |= RCC_CR_HSION;// turn on HSI16 oscillator
	while((RCC->CR & RCC_CR_HSIRDY) == 0);//check HSI16 ready

	SET_REG (RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI);
	while( (RCC->CFGR & RCC_CFGR_SWS ) != RCC_CFGR_SWS_HSI);//check HSI16 config ready
	SET_REG(RCC->CFGR, RCC_CFGR_HPRE, 0);//SYSCLK 16MHz


	if((RCC->CR & RCC_CR_HSIRDY) == 0)
		return;
// Use HSI16 as system clock
//APB1 prescaler not divide
//APB2 prescaler not divide
}

void Timer_init( TIM_TypeDef *timer)

{	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;//timer5 clock enable
	timer->PSC = (uint32_t)(1600-1);//Prescalser
	timer->ARR = (uint32_t)(100-1);//Reload value
	//update per 10
	timer->EGR = TIM_EGR_UG;//Reinitialize the counter.
	//16MHz * 1600 * 10000 = 1(s)
}
void Timer_start( TIM_TypeDef *timer){
//TODO: start timer and show the time on the 7-SEG LED.
	//start timer
	timer->CR1 |= TIM_CR1_CEN;

	display_init();
}

void display_init(){
	int i;
	for(i=1;i<=8;i++){
		if(i<3) max7219_send(i,ZERO); //ADDRESS , DATA
		else if(i==3) max7219_send(i,ZERO + (1<<7));
		else max7219_send(i,SPACE); //ADDRESS , DATA
	}
}
//decode

//0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b
//0,1,2,3,4,5,6,7,8,9



int main(){
	GPIO_init();
	max7219_init();
	SystemClock_Config();
	Timer_init(TIM5);
	Timer_start(TIM5);

	int timerValue;
	int time_limit = TIME_SEC*100;
	int t = 0;
	while(1)
	{
		//TODO: Polling the timer count and do lab requirements
		timerValue = TIM5->CNT;
		if (timerValue == 9 && t < time_limit){//0~9
			//GPIOA->BSRR = 0b100001;//on
			t++;
			if(t>1000000) display_init();
			else display(t);
		}
	}
}

void display(int data)
{
	int num_digs=8;
	int digit;
	digit = 1;
	int number;
	int tmp;
	if(data == 0){
		display_init();
	}
	else{
		while(digit <= num_digs){
			tmp = data/10;
			number = data - tmp*10;
			number = decode(number);
			//;
			if(digit == 3) number += (1<<7); //dot
			max7219_send(digit, number);
			data = tmp;
			digit = digit+1;
			if(data == 0){
				break;
			}
		}
	}
	while(digit <= num_digs){
		if(digit <= 3){
			if(digit == 3) max7219_send(digit , ZERO + (1<<7));
			else max7219_send(digit , ZERO );
		}
		else{
			max7219_send(digit, SPACE);
		}
		digit = digit+1;
	}//

}

int decode( int number ){
	int value;
	switch (number){
	case 0:
		value = 0x7e;
		break;
	case 1:
		value = 0x30;
		break;
	case 2:
		value = 0x6d;
		break;
	case 3:
		value = 0x79;
		break;
	case 4:
		value = 0x33;
		break;
	case 5:
		value = 0x5b;
		break;
	case 6:
		value = 0x5f;
		break;
	case 7:
		value = 0x70;
		break;
	case 8:
		value = 0x7f;
		break;
	case 9:
		value = 0x7b;
		break;
	default:
		break;
	}
	return value;
}

