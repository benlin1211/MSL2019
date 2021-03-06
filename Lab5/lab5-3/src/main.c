//These functions inside the asm file
extern void GPIO_init();
extern void max7219_init();
extern void max7219_send(unsigned int address, unsigned int data);
//auto branch to the pc of the name
//stack parameter: address = r0, data = r1

/**
* TODO: Show data on 7-seg via max7219_send
* Input:
* data: decimal value
* num_digs: number of digits will show on 7-seg
* Return:
* 0: success
* -1: illegal data range(out of 8 digits range)
*/
#include "stm32l476xx.h"
#include "stdbool.h"
#define EMPTY -1 //pb1
//TODO: define your gpio pin
/*
#define X0 1 //pb0
#define X1 2 //pb1
#define X2 3 //pb2
#define X3 4 //pb3
#define Y0 8 //pc8
#define Y1 7 //pc7
#define Y2 6 //pc6
#define Y3 5 //pc5
unsigned int x_pin[4] = {X0, X1, X2, X3}; //row:in
unsigned int y_pin[4] = {Y0, Y1, Y2, Y3}; //col:out
*/



/* TODO: initial keypad gpio pin, X as output and Y as input */
void keypad_init() //and 0 / or 1
{
// SET keypad gpio OUTPUT //
	RCC->AHB2ENR = RCC->AHB2ENR|0x7;
//Set PC5,6,7,8 as output mode 01
	GPIOC->MODER = (GPIOC->MODER & 0xFFFC03FF)|0x15400;
//set PC5,6,7,8 is Pull-up output 01
	GPIOC->PUPDR = (GPIOC->PUPDR & 0xFFFC03FF)|0x15400;
//Set PC5,6,7,8 as medium speed mode 01
	GPIOC->OSPEEDR = (GPIOC->OSPEEDR & 0xFFFC03FF)|0x15400;
//Set PC5,6,7,8 as high initially
	GPIOC->ODR = (GPIOC->ODR&0xFFFFFE1F)|0b1111<<5;

	//GPIOC->OTYPER =  GPIOC->OTYPER|0x1E0; //1 1110 0000

// SET keypad gpio INPUT //
//Set PB0,1,2,3 as INPUT mode 00
	GPIOB->MODER = GPIOB->MODER & 0xFFFFFF00;
//set PB0,1,2,3 is Pull-down input 10
	GPIOB->PUPDR = (GPIOB->PUPDR & 0xFFFFFF00) |0xAA;
//Set PB0,1,2,3 as medium speed mode 01
	GPIOB->OSPEEDR =  (GPIOB->PUPDR & 0xFFFFFF00) |0x55;
}

void keypad_init_2()
{
// SET keypad gpio INPUT //
	RCC->AHB2ENR = RCC->AHB2ENR|0x7;
//Set PC5,6,7,8 as INPUT mode 00
	GPIOC->MODER = GPIOC->MODER & 0xFFFC03FF;
//set PC5,6,7,8 is Pull-down input 10
	GPIOC->PUPDR = (GPIOC->PUPDR  & 0xFFFC03FF)|0x2A800;
//Set PC5,6,7,8 as medium speed mode 01
	GPIOC->OSPEEDR = (GPIOC->OSPEEDR& 0xFFFC03FF)|0x15400;



// SET keypad gpio output //
//Set PB0,1,2,3 as output mode 01
	GPIOB->MODER = (GPIOB->MODER & 0xFFFFFF00) | 0x55 ;
//set PB0,1,2,3 is Pull-up output 01
	GPIOB->PUPDR = (GPIOB->PUPDR & 0xFFFFFF00) | 0x55;
//Set PB0,1,2,3 as medium speed mode 01
	GPIOB->OSPEEDR = (GPIOB->PUPDR & 0xFFFFFF00) | 0x55;
//Set PB0,1,2,3 as high initially
	GPIOB->ODR = (GPIOB->ODR&0xFFFFFFF0) |0b1111;
}

void display(int data)
{
	int num_digs=8;
	int digit;
	digit = 1;
	int number;
	int tmp;
	if(data != EMPTY){
		while(digit <= num_digs){
			tmp = data/10;
			number = data - tmp*10;
			max7219_send(digit, number);
			data = tmp;
			digit = digit+1;
			if(data == 0){
				break;
			}
		}
	}
	while(digit <= num_digs){
		max7219_send(digit, 15);
		digit = digit+1;
	}//

}
/* TODO: scan keypad value
return:
 >=0: key pressedvalue
 -1: no keypress
*/
char keypad_scan()
{
}

int main(){
	GPIO_init();
	max7219_init();

	int value;
	int tmp;
	int flag_keypad_r;
	int flag_keypad;
	int flag_debounce;
	int k;
	int position_c, position_r;
	int digit ;
	int x ;
	bool zero;
	//int mask;
	int Table[4][4] = {
			{1,2,3,10},
			{4,5,6,11},
			{7,8,9,12},
			{15,0,14,13}
	};
	bool Press[4][4] = {0};

	while(1){ //polling
		// row is input


		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				Press[i][j]= false;
			}
		}
		keypad_init();
		//PB0,1,2,3
		value = 0;
		flag_keypad = GPIOB->IDR&0b1111;
		tmp =  GPIOB->IDR ;
		flag_debounce = 0;
		if(flag_keypad!=0){
			k=45500;
			while(k!=0){
				flag_debounce=GPIOB->IDR&0b1111;
				k--;
			}
			//flag_debounce=GPIOB->IDR&0b1111;
		}
		if(flag_debounce!=0){
			//set PC5,6,7,8(column) opendrain
			//GPIOC->OTYPER =  GPIOC->OTYPER|0x1E0;
			//mask = 0xFFFFFFDF;// 1101 1111
			for(int i=0;i<4;i++){ //scan keypad from first column
				// output PC5,6,7,8
				position_c = i+5;

				//set PC5,6,7,8(column) low and set pin high
				//GPIOC->OTYPER = (GPIOC->OTYPER&0xFFFFFE1F)| 1<<position_c;
				//GPIOC->ODR|1111<<5;
				GPIOC->ODR = (GPIOC->ODR&0xFFFFFE1F) | 1<<position_c;//set to 1 (one by one)
				//GPIOC->ODR = GPIOC->ODR&mask; //set to 1 (one by one)
				//mask = mask<<1;
				//mask = mask+1;
				tmp =  GPIOB->IDR;

				for(int j=0;j<4;j++){ //read input from first row
					position_r=j;
					// input  PB0,1,2,3
					flag_keypad_r = GPIOB->IDR & 1<<position_r;
					if(flag_keypad_r!=0){
						//display(Table[j][i]);
						//value = value+Table[j][i];
						//keypad_init();
						Press[j][i] = true;

					}
				}
			}
			keypad_init_2();

			for(int i=0;i<4;i++){ //scan keypad from first column
							// output PB0,1,2,3
				position_c = i;
				GPIOB->ODR = (GPIOB->ODR&0xFFFFFFF0) | 1<<position_c;//set to 1 (one by one)
				for(int j=0;j<4;j++){ //read input from first row
					position_r=j+5;
					flag_keypad_r = GPIOC->IDR & 1<<position_r;
					if(flag_keypad_r!=0){
						Press[i][j] = true;//invert
					}
				}
			}
			zero = false;
			for(int i=0;i<4;i++){
				for(int j=0;j<4;j++){
					if(Press[j][i]== true){
						if(value > 0 || zero ){
							tmp = Table[j][i];
							if(value < tmp){
								x = value;
								value = tmp;
								tmp = x;
							}//value > tmp
							digit = 1;
							if(tmp == 0){
								digit = digit*10;
							}
							x = tmp;
							while(x>0){
								x = x/10;
								digit = digit*10;
							}
							value = value * digit;
							value = value + tmp;
						}
						else
							value = value+Table[j][i];
							zero = true;
					}
				}
			}
			display(value);
		}
		else{
			display(EMPTY);
		}
	}
}
