/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under Ultimate Liberty license
* SLA0044, the "License"; You may not use this file except in compliance with
* the License. You may obtain a copy of the License at:
*                             www.st.com/SLA0044
*
******************************************************************************
*/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_x-cube-ai.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHAPE 12*32*1 //384
#define SET_REG(REG,SELECT,VAL) {((REG)=((REG)&(~(SELECT)))|(VAL));};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char cmd[100];
int len = 0;
int input[SHAPE] = { 0 };
int read = 0;
char ascii_buffer[4];
int ascii_count = 0;

int end = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void GPIO_Init(void) {
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);
	// UART:PA9 & PA10 & PA2 & PA3
	//TM_GPIO_Init(GPIOA, 9, TM_GPIO_Mode_AF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	GPIOA->MODER = (GPIOA->MODER & 0xFFF3FFFF) | (0b10 << 18); //9
	GPIOA->OTYPER = (GPIOA->OTYPER & 0xFFFFFDFF) | 0;
	GPIOA->PUPDR = GPIOA->PUPDR & 0xFFF3FFFF;
	GPIOA->OSPEEDR = GPIOA->OSPEEDR & 0xFFF3FFFF;
	//TM_GPIO_Init(GPIOA, 10, TM_GPIO_Mode_AF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	GPIOA->MODER = (GPIOA->MODER & 0xFFCFFFFF) | (0b10 << 20); //10
	GPIOA->OTYPER = (GPIOA->OTYPER & 0xFFFFFBFF) | 0;
	GPIOA->PUPDR = GPIOA->PUPDR & 0xFFCFFFFF;
	GPIOA->OSPEEDR = GPIOA->OSPEEDR & 0xFFCFFFFF;

	GPIOA->MODER = (GPIOA->MODER & 0xFFFFFFCF) | (0b10 << 4); //2
	GPIOA->OTYPER = (GPIOA->OTYPER & 0xFFFFFFFB) | 0;
	GPIOA->PUPDR = GPIOA->PUPDR & 0xFFFFFFCF;
	GPIOA->OSPEEDR = GPIOA->OSPEEDR & 0xFFFFFFCF;

	GPIOA->MODER = (GPIOA->MODER & 0xFFFFFF3F) | (0b10 << 6);//3
	GPIOA->OTYPER = (GPIOA->OTYPER & 0xFFFFFFF7) | 0;
	GPIOA->PUPDR = GPIOA->PUPDR & 0xFFFFFF3F;
	GPIOA->OSPEEDR = GPIOA->OSPEEDR & 0xFFFFFF3F;

	// AF7 for pin9&10
	GPIOA->AFR[1] = (GPIOA->AFR[1] & ~(0x00000FF0)) | 0x00000770;
	//(0111 0111)000000


	GPIOA->AFR[0] = (GPIOA->AFR[0] & ~(0x0000FF00)) | 0x00007700;
	//0000(0111 0111)00

	// BUTTON: PC13
	//TM_GPIO_Init(GPIOC, 13, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Medium);
	GPIOC->MODER = (GPIOC->MODER & 0xF3FFFFFF);
	GPIOC->OTYPER = (GPIOC->OTYPER & 0xFFFFDFFF) | 0;
	GPIOC->PUPDR = GPIOC->PUPDR & 0xF3FFFFFF;
	GPIOC->OSPEEDR = (GPIOC->OSPEEDR & 0xF3FFFFFF) | (0b01 << 26);
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
	SET_REG(USART1->BRR, 0xFF, 4000000 / 115200);//0
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

//PA9 & PA10 => USART1
void USART2_Init(void) {
	/* Enable clock for USART2 */
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	// CR1 /* Receiver Enable & Transmitter Enable */
	//MODIFY_REG(USART1->CR1, USART_CR1_M | USART_CR1_PS | USART_CR1_PCE | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8, USART_CR1_TE | USART_CR1_RE);
	SET_REG(USART2->CR1, USART_CR1_M, USART_CR1_M0);//00: 1 Start bit, 8+1 data bits, n stop bits
	SET_REG(USART2->CR1, USART_CR1_PS, 0);//0: Even parity
	SET_REG(USART2->CR1, USART_CR1_PCE, USART_CR1_PCE);//1: the computed parity is inserted at the MSB position(9th bit if M=1)
	SET_REG(USART2->CR1, USART_CR1_TE, 0);
	SET_REG(USART2->CR1, USART_CR1_RE, USART_CR1_RE);//1
	//recv interrupt enable bit
	SET_REG(USART2->CR1, USART_CR1_TXEIE, 0);//0
	SET_REG(USART2->CR1, USART_CR1_TCIE, 0);//0
	SET_REG(USART2->CR1, USART_CR1_RXNEIE, USART_CR1_RXNEIE);//1
	SET_REG(USART2->CR1, USART_CR1_OVER8, 0);//0: Oversampling by 16; 1: Oversampling by 8

	// CR2
	//MODIFY_REG(USART1->CR2, USART_CR2_STOP, 0x0); // 1-bit stop
	SET_REG(USART2->CR2, USART_CR2_STOP, 0);//00: 1 stop bit

	// CR3
	//MODIFY_REG(USART1->CR3, (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT), 0x0); // none hwflow control
	SET_REG(USART2->CR3, USART_CR3_RTSE, 0);//0
	SET_REG(USART2->CR3, USART_CR3_CTSE, 0);//0
	SET_REG(USART2->CR3, USART_CR3_ONEBIT, 0);//0
	// BRR
	//MODIFY_REG(USART1->BRR, 0xFF, 4000000L/115200);
	SET_REG(USART2->BRR, 0xFF, 4000000 / 115200);//0
	//clk rate = 4MHz (MSI)

	/*
	In asynchronous mode, the following bits must be kept cleared:
	- LINEN and CLKEN bits in the USART_CR2 register,
	- SCEN, HDSEL and IREN bits in the USART_CR3 register.
	*/
	USART2->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
	USART2->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);

	// Finally, enable UART

	USART2->CR1 |= (USART_CR1_UE);
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
	SET_REG(USART2->CR1, USART_CR1_TE, USART_CR1_TE);
	int i;
	while ((USART2->ISR & USART_ISR_TXE) == 0);
	for (i = 0; i < size; i++) {
		USART2->TDR = arr[i];
		while ((USART2->ISR & USART_ISR_TXE) == 0);
	}
	while ((USART2->ISR & USART_ISR_TC) == 0);

	return i;
}

void EXTI15_10_IRQHandler(void)
{

	if ((EXTI->PR1 & EXTI_PR1_PIF13) == EXTI_PR1_PIF13)
	{
		unsigned char *s = "Hello World!";
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
	int cmp = 1;
	if (len >= 100){
		if (c == 127 || c == 8){ //backspace available
			if (len > 0){
				UART_Transmit(&c, 1);
				len--;
			}
		}
		else if (c == 13){//handle 100
			UART_Transmit("\r\n", 4);
			len = 0;
		}
	}
	else{
		if (c == 13){ //\n
			UART_Transmit("\r\n", 4);
			cmd[len] = 0; //remind :\0
			end = 1;
			cmp = strcmp(cmd, "1");
			if (cmp == 0) {

				//Runinterface
			}
			len = 0;
		}
		else if (c == 127 || c == 8){ //back delete
			if (len > 0){
				UART_Transmit(&c, 1);
				cmd[len--] = 0;
			}
		}
		else{
			UART_Transmit(&c, 1);
			cmd[len++] = c;
		}
	}
	if (end == 1){
		UART_Transmit(">", 1);
	}
	end = 0;
	NVIC_ClearPendingIRQ(USART1_IRQn);
}


/* USER CODE END 0 */

/**
* @brief  The application entry point.
* @retval int
*/
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */


	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */
	GPIO_Init();
	EXTI_config();
	NVIC_config();
	USART1_Init();
	USART2_Init();

	UART_Transmit(">", 1);
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_CRC_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_X_CUBE_AI_Init();
	/* USER CODE BEGIN 2 */


	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		MX_X_CUBE_AI_Process();
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
* @brief System Clock Configuration
* @retval None
*/
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 10;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
		| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure the main internal regulator output voltage
	*/
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
* @brief CRC Initialization Function
* @param None
* @retval None
*/
static void MX_CRC_Init(void)
{

	/* USER CODE BEGIN CRC_Init 0 */

	/* USER CODE END CRC_Init 0 */

	/* USER CODE BEGIN CRC_Init 1 */

	/* USER CODE END CRC_Init 1 */
	hcrc.Instance = CRC;
	hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
	hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
	hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
	if (HAL_CRC_Init(&hcrc) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN CRC_Init 2 */

	/* USER CODE END CRC_Init 2 */

}

/**
* @brief USART1 Initialization Function
* @param None
* @retval None
*/
static void MX_USART1_UART_Init(void)
{

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
* @brief USART2 Initialization Function
* @param None
* @retval None
*/
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
* @brief GPIO Initialization Function
* @param None
* @retval None
*/
static void MX_GPIO_Init(void)
{

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
* @brief  This function is executed in case of error occurrence.
* @retval None
*/
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(char *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
