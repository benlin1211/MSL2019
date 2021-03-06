#include "stm32l476xx.h"
//#define RCC_CR_HSION 1
//#define RCC_CR_HSIRDY 1

#define SET_REG(REG,SELECT,VAL) {((REG)=((REG)&(~(SELECT)))|(VAL));};
//REG: register address
//SELECT:selected region sets to 1

void GPIO_init();
void Delay1sWith4MHz();
void SystemClock_Config();

int main(){
	SystemClock_Config();
	GPIO_init();
	while(1){
		// make LED light
		Delay1sWith4MHz();
		GPIOA->BSRR = 0b100001; //set these gpio addresss to 1
		// make LED dark
		Delay1sWith4MHz();
		GPIOA->BRR = 0b100001; //set these gpio addresss to 0
	}
}

void SystemClock_Config(){

/*
	// Use HSI16 16MHz as system (main) clock
	RCC->CR |= RCC_CR_HSION;// turn on HSI16 oscillator
	while( (RCC->CR & RCC_CR_HSIRDY) == 0);//check HSI16 ready
	//SET_REG(RCC->CFGR, RCC_CFGR_HPRE, 0b1000<<4);//n=0~7:origin, n=8~15:divide by 2^(n-7)
	//SYSCLK divide by 2. SYSCLK = 16MHz/2 = 8Mhz
	SET_REG(RCC->CFGR, RCC_CFGR_HPRE, 0);
*/
	// Use PLL (from MSI) as system (main) clock
	//switch the sysclk to a different clock (MSI)- the one used as PLL source is  running,
	RCC->CR |= RCC_CR_MSION;

	while( (RCC->CR & RCC_CR_MSIRDY) == 0);//check MSI ready
	SET_REG(RCC->CR, RCC_CR_MSIPLLEN, 1);// enable MSI to PLL
	SET_REG(RCC->CR, RCC_CR_MSIRGSEL, 1);// choose MSIRANGE
	SET_REG(RCC->CR, RCC_CR_MSIRANGE, RCC_CR_MSIRANGE_6);// MSI range

// stop PLL, wait until down
	RCC->CR &= ~RCC_CR_PLLON; // Disable PLL
	while( (RCC->CR & RCC_CR_PLLRDY) == 1);//check PLL down

//change PLL params; if needed, change FLASH params and/or APB prescalers
	SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC, RCC_PLLCFGR_PLLSRC_MSI);

	//SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, 1);
	//SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ, 1);


////Finally, switch system clock to PLL & start PLL, wait until done
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
	//SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, 1);
	RCC->CR |= RCC_CR_PLLON;// turn on PLL oscillator
	while( (RCC->CR & RCC_CR_PLLRDY ) == 0);//check PLL ready
	//RCC->CFGR |= RCC_CFGR_SW_PLL;// PLL set as clock
	SET_REG(RCC->CFGR, RCC_CFGR_SW, 11);
	while( (RCC->CFGR & RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);//check PLL ready




	if((RCC->CR & RCC_CR_PLLRDY) == 0)
		return;

//APB1 prescaler not divide
//APB2 prescaler not divide

}
void Delay1sWith4MHz(){

	delay();
}
