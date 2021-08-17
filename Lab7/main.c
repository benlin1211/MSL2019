#include "stm32l476xx.h"
#include "core_cm4.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cmSimd.h"
#include "system_stm32l4xx.h"
#include "stdbool.h"

#define SET_REG(REG,SELECT,VAL) {((REG) = ((REG)&(~(SELECT))) | (VAL));};

int Table[4][4];
int colm;
void init_GPIO()
{
	RCC->AHB2ENR = RCC->AHB2ENR|0x7;

	//Set PA8,9,10,12 as output mode
	GPIOA->MODER= (GPIOA->MODER&0xFCC0F3FF)|0x01150400;
	//set PA8,9,10,12 is Pull-up output
	GPIOA->PUPDR=(GPIOA->PUPDR&0xFCC0F3FF)|0x1150400;
	//Set PA8,9,10,12 as medium speed mode
	GPIOA->OSPEEDR=GPIOA->OSPEEDR|0x1150400;
	//Set PA8,9,10,12 as high
	GPIOA->ODR=(GPIOA->ODR&0)|10111<<8;
	// SET keypad gpio INPUT //
	//Set PB5,6,7,9 as INPUT mode
	GPIOB->MODER=GPIOB->MODER&0xFFF303FF;
	//set PB5,6,7,9 is Pull-down input
	GPIOB->PUPDR=(GPIOB->PUPDR&0xFFF303FF)|0x8A800;
	//Set PB5,6,7,9 as medium speed mode
	GPIOB->OSPEEDR=GPIOB->OSPEEDR|0x45400;

}

void EXTI_config()
{
	SET_REG(RCC->APB2ENR,RCC_APB2ENR_SYSCFGEN,RCC_APB2ENR_SYSCFGEN);
	SET_REG(SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI5,SYSCFG_EXTICR2_EXTI5_PB);
	SET_REG(SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI6,SYSCFG_EXTICR2_EXTI6_PB);
	SET_REG(SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI7,SYSCFG_EXTICR2_EXTI7_PB);
	SET_REG(SYSCFG->EXTICR[2],SYSCFG_EXTICR3_EXTI9,SYSCFG_EXTICR3_EXTI9_PB);
	SET_REG(EXTI->IMR1,EXTI_IMR1_IM13,EXTI_IMR1_IM5);
	SET_REG(EXTI->IMR1,EXTI_IMR1_IM13,EXTI_IMR1_IM6);
	SET_REG(EXTI->IMR1,EXTI_IMR1_IM13,EXTI_IMR1_IM7);
	SET_REG(EXTI->IMR1,EXTI_IMR1_IM13,EXTI_IMR1_IM9);
	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT13,EXTI_FTSR1_FT5);
	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT13,EXTI_FTSR1_FT6);
	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT13,EXTI_FTSR1_FT7);
	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT13,EXTI_FTSR1_FT9);
}
void NVIC_config()
{
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void delay()
{
	int k = 600;
	while(k!=0)
	{
		int t = 600;
		while(t!=0)
		{
			t--;
		}
		k--;
	}
}
void EXTI9_5_IRQHandler(void)
{
	int row , time;
	if((EXTI->PR1 & EXTI_PR1_PIF5) == EXTI_PR1_PIF5)
	{
		EXTI->PR1 = EXTI_PR1_PIF5;
		row = 0;
	}
	else if((EXTI->PR1 & EXTI_PR1_PIF6) == EXTI_PR1_PIF6)
	{
		EXTI->PR1 = EXTI_PR1_PIF6;
		row = 1;
	}
	else if((EXTI->PR1 & EXTI_PR1_PIF7) == EXTI_PR1_PIF7)
	{
		EXTI->PR1 = EXTI_PR1_PIF7;
		row = 2;
	}
	else if((EXTI->PR1 & EXTI_PR1_PIF9) == EXTI_PR1_PIF9)
	{
		EXTI->PR1 = EXTI_PR1_PIF9;
		row = 3;
	}
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	int tmp_colm = (colm==0)?3:colm-1;
	time = Table[row][tmp_colm];
	for(int i=0;i<time;i++)
	{
		GPIOA->ODR =  0<<5;
		delay();
		GPIOA->ODR =  1<<5;
		delay();

	}

}

int main()
{
	init_GPIO();
	EXTI_config();
	NVIC_config();
	int position_c;
	colm = 0;

	for(int i=0;i<4;i++){
		 if(i==3)
		 {
		    Table[i][0] = 15;
		    Table[i][1] = 0;
		    Table[i][2] = 14;
		    Table[i][3] = 13;
		 }
		 else for(int j=0;j<4;j++)
		 {
		    if(j==3) Table[i][j] = 10 + i;
		    else Table[i][j] = 3*i+j + 1;
		 }
	}

	GPIOA->ODR |= 1<<5;

	while(1)
	{

			//scan keypad from first column
			position_c= 8+colm;
			if(colm==3)position_c++;
			//set PA8,9,10,12(column) low and set pin high from PA8
			GPIOA->ODR=(GPIOA->ODR&0xFFFFE8FF)|1<<position_c;

			colm = (colm == 3)?0:colm+1;
	}


}
