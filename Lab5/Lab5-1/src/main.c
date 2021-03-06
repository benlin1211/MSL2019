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
int display(int data, int num_digs)
{
	int number;
	int digit;
	digit = 1;
	int tmp;
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
	if(digit <= num_digs){
		max7219_send(digit, 0);
		digit = digit+1;
	}
	else return -1;
	while(digit <= num_digs){
		max7219_send(digit, 15);
		digit = digit+1;
		if(data == 0){
			break;
		}
	}
	return 0;
}
void main()
{
	int student_id = 610032;
	GPIO_init();
	max7219_init();
	display(student_id, 8);
 //
}
