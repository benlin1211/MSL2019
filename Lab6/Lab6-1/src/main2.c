#include<stdio.h>
#include<stdlib.h>
#include <stm32l476xx.h>
#define SET_REG(REG,SELECT,VAL) {((REG) = ((REG)&(~(SELECT))) | (VAL));};


extern void GPIO_init();
extern void Delay1sWith4MHz();

void SystemClock_Config(){

	int flag = 1;

	RCC->CR |= RCC_CR_MSION; //enable
	while((RCC->CR & RCC_CR_MSIRDY) == 0); //check ready
	SET_REG(RCC->CR, RCC_CR_MSIRANGE, RCC_CR_MSIRANGE_6); //choose msi
	SET_REG(RCC->CR, (1<<3), RCC_CR_MSIRGSEL);
	while((RCC->CR & RCC_CR_MSIRDY) == 0);

	RCC->CFGR &= ~RCC_CFGR_SW;

	SET_REG(RCC->CR, RCC_CR_PLLON, 0); //disable pll
	RCC->CR = RCC->CR & 0xFEFFFFFF;
	while((RCC->CR & RCC_CR_PLLRDY) == RCC_CR_PLLRDY);


	SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC, RCC_PLLCFGR_PLLSRC_MSI); //give pll source

	if(GPIOB->ODR == 1){ // 1MHz
		SET_REG(RCC->CR, RCC_CR_MSIRANGE, RCC_CR_MSIRANGE_4);
		SET_REG(RCC->CR, (1<<3), RCC_CR_MSIRGSEL);
		while((RCC->CR & RCC_CR_MSIRDY) == 0);
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, (8U<<8));
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, (2U<<4));
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, (4U<<25));
	}
	else if(GPIOB->ODR == 3){ // 6MHz
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, (12U<<8));
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, (2U<<4));
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, (4U<<25));
	}
	else if(GPIOB->ODR == 7){ // 10MHz
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, (20U<<8));
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, (2U<<4));
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, (4U<<25));
	}
	else if(GPIOB->ODR == 15){ // 16MHz
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, (32U<<8));
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, (2U<<4));
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, (4U<<25));
	}
	else if(GPIOB->ODR == 31){ // 40MHz
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, (80U<<8));
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, (2U<<4));
		SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, (4U<<25));
	}

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;



	RCC->CR |= RCC_CR_PLLON; //re-enable
	while((RCC->CR & RCC_CR_PLLRDY) == 0);

	//SET_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL)
	RCC->CFGR |= RCC_CFGR_SW_PLL; //select as system clock
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	if((RCC->CR & RCC_CR_PLLRDY) == 0)
		return;
}

int main(){
	GPIO_init();
	SystemClock_Config();

	while(1){
		GPIOA->BSRR = (1<<5);
		Delay1sWith4MHz();
		SystemClock_Config();

		GPIOA->BRR = (1<<5);
		Delay1sWith4MHz();
		SystemClock_Config();
	}
}
