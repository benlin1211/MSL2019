#include "stm32l476xx.h"
#include "cmsis_gcc.h"
#include "core_cm4.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cmSimd.h"
#include "system_stm32l4xx.h"
#include "stdbool.h"

#define EMPTY -1 //pb1
#define X0 (GPIOB->ODR&0xFFFFFBFF)|1<<10
#define X1 (GPIOA->ODR&0xFFFFFCFF)|1<<8
#define X2 (GPIOA->ODR&0xFFFFFCFF)|1<<9
#define X3 (GPIOC->ODR&0xFFFFFFBF)|1<<7
#define XB_reset (GPIOB->ODR&0xFFFFFBFF)
#define XA_reset (GPIOA->ODR&0xFFFFFCFF)
#define XC_reset (GPIOC->ODR&0xFFFFFF7F)


extern void max7219_init();
extern void MAX7219Send();
extern void GPIO_init();


void display(int data)
{
	int num_digs = 8;
	int digit;
	digit = 1;
	int number;
	int tmp;
	if (data != EMPTY){
		while (digit <= num_digs){
			tmp = data / 10;
			number = data - tmp * 10;
			max7219_send(digit, number);
			data = tmp;
			digit = digit + 1;
			if (data == 0){
				break;
			}
		}
	}
	while (digit <= num_digs){
		max7219_send(digit, 15);
		digit = digit + 1;
	}//

}


void keypad_init()
{
	// SET keypad gpio OUTPUT //
	RCC->AHB2ENR = RCC->AHB2ENR|0x7;
	//Set PB10, PA8, 9, PC7 as output mode
	GPIOB->MODER= 0xFFFFFFFF;
	GPIOA->MODER= GPIOA->MODER&0xFFF5FFFF;
	GPIOB->MODER= GPIOB->MODER&0xFFDFFFFF;
	GPIOC->MODER= GPIOC->MODER&0xFFFF7FFF;
	//set PB10, PA8, 9, PC7 as Pull-up output
	GPIOA->PUPDR=GPIOA->PUPDR|0x50000;
	GPIOB->PUPDR=0x0;
	GPIOB->PUPDR=GPIOB->PUPDR|0x100000;
	GPIOC->PUPDR=GPIOC->PUPDR|0x7000;
	//Set PB10, PA8, 9, PC7 as medium speed mode
	GPIOA->OSPEEDR=GPIOA->OSPEEDR|0x50000;
	GPIOB->OSPEEDR=0x0;
	GPIOB->OSPEEDR=GPIOB->OSPEEDR|0x100000;
	GPIOC->OSPEEDR=GPIOC->OSPEEDR|0x7000;
	//Set PB10, PA8, 9, PC7 as high
	GPIOA->ODR=GPIOA->ODR|11<<8;
	GPIOB->ODR=GPIOB->ODR|1<<10;
	GPIOC->ODR=GPIOC->ODR|1<<7;
	// SET keypad gpio INPUT //
	//Set PA10, PB3, 5, 4 as INPUT mode
	GPIOA->MODER=GPIOA->MODER&0xFFCFFFFF;
	GPIOB->MODER=GPIOB->MODER&0xFFFFF03F;
	//set  PA10, PB3, 5, 4 as Pull-down input
	GPIOA->PUPDR=GPIOA->PUPDR|0x200000;
	GPIOB->PUPDR=GPIOB->PUPDR|0xA80;
	//Set PA10, PB3, 5, 4  as medium speed mode
	GPIOA->OSPEEDR=GPIOA->OSPEEDR|0x100000;

	GPIOB->OSPEEDR=GPIOB->OSPEEDR|0x540;
}

void play_init()
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= ~(GPIO_MODER_MODER0_0);
	GPIOB->MODER |= GPIO_MODER_MODER0_1;
	//GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0);
	//GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED0);
	//GPIOB->AFR[1] = 0x1 << ((13-8)*4);
	GPIOB->AFR[0] = 0x2;
}

void play(int freq, int pwm_percent)
{
	int duration;
	int pwm_dur;
	duration = 40000000/freq;
	pwm_dur = duration * pwm_percent / 100;

	TIM3->PSC = 0;
	TIM3->ARR = duration;
	TIM3->CR1 |= TIM_CR1_ARPE;
	TIM3->CCR3 = pwm_dur;
	TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE ;
	TIM3->CCER |= TIM_CCER_CC3E;
	TIM3->EGR |= TIM_EGR_UG;
	TIM3->CR1 |= TIM_CR1_CEN;
}

void stop_play(){
	TIM3->CR1 &= ~TIM_CR1_CEN;
}
void display_init(){
	display(EMPTY);
}

int main()
{

	int y_pin[4];
	int playing_flag = 0;

	int pwm = 50;
	int flag_debounce;
	int k;
	GPIO_init();
	max7219_init();
	keypad_init();
	play_init();
	display_init();
	int Table[4][4] = {
		{ 2616, 2937, 3296, 0 },
		{ 3492, 3920, 4400, 0 },
		{ 4939, 5233, 0, 0 },
		{ 1, 0, 2, 0 }
	};
    int pressed = 0;
		//keypad_init();
		//play_note_init();
	while(1)
	{

		flag_debounce = 0;
		y_pin[0] = (GPIOA->IDR&1<<10);
		y_pin[1] = (GPIOB->IDR&1<<3);
		y_pin[2] = (GPIOB->IDR&1<<5);
		y_pin[3] = (GPIOB->IDR&1<<4);
		GPIOA->ODR=GPIOA->ODR|11<<8;
		GPIOB->ODR=GPIOB->ODR|1<<10;
		GPIOC->ODR=GPIOC->ODR|1<<7;
		//flag_keypad=GPIOB->IDR&10111<<5;
		if(y_pin[0]!=0 || y_pin[1]!=0 || y_pin[2]!=0 || y_pin[3]!=0)
		{
			k=5000;
			while(k!=0)
			{
				y_pin[0] = (GPIOA->IDR&1<<10);
				y_pin[1] = (GPIOB->IDR&1<<3);
				y_pin[2] = (GPIOB->IDR&1<<5);
				y_pin[3] = (GPIOB->IDR&1<<4);
				flag_debounce=(y_pin[0]!=0 || y_pin[1]!=0 || y_pin[2]!=0 || y_pin[3]!=0);
				k--;
			}
			if (flag_debounce != 0)//...
			{
				for (int i = 0; i<4; i++){ //scan keypad from first column
					if (i == 0){
						GPIOA->ODR = XA_reset;
						GPIOB->ODR = X0;
						GPIOC->ODR = XC_reset;
					}
					if (i == 1){
						GPIOA->ODR = X1;
						GPIOB->ODR = XB_reset;
						GPIOC->ODR = XC_reset;
					}
					if (i == 2){
						GPIOA->ODR = X2;
						GPIOB->ODR = XB_reset;
						GPIOC->ODR = XC_reset;
					}
					if (i == 3){
						GPIOA->ODR = XA_reset;
						GPIOB->ODR = XB_reset;
						GPIOC->ODR = X3;
					}
					for (int j = 0; j<4; j++){ //read input from first row
						y_pin[0] = (GPIOA->IDR & 1 << 10);
						y_pin[1] = (GPIOB->IDR & 1 << 3);
						y_pin[2] = (GPIOB->IDR & 1 << 5);
						y_pin[3] = (GPIOB->IDR & 1 << 4);
						if ((y_pin[j]) != 0){
							if(pressed == 0){
								if (Table[j][i] == 1 && pwm < 90) pwm += 5; //* => +
								else if (Table[j][i] == 2 && pwm >10) pwm -= 5; //# => -
								pressed = 1;
							}
							display(Table[j][i]);
						}
					}
				}
			}
			else
			{
				//stop_play();
				//playing_flag = 0;
				play(Table[0][0], pwm);
				display(pwm);
				pressed = 0;
			}
		}

	}
}
