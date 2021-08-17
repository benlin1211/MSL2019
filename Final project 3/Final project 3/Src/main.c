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
#define SET_REG(REG,SELECT,VAL) {((REG)=((REG)&(~(SELECT)))|(VAL));};
#define SHAPE 32 * 12 * 1  //384

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
int input[SHAPE]={0.0};
char ascii_buffer[4];
int ascii_count=0;
int read=0;

char cmd[100];
int len = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int UART3_Transmit(uint8_t *arr, uint32_t size) { 
        //TODO: Send str to UART and return how many bytes are successfully transmitted.
	SET_REG(USART3->CR1, USART_CR1_TE, USART_CR1_TE);
	int i;
	while ((USART3->ISR & USART_ISR_TXE) == 0);
	for (i = 0; i < size; i++) {
		USART3->TDR = arr[i];
		while ((USART3->ISR & USART_ISR_TXE) == 0);
	}
	while ((USART3->ISR & USART_ISR_TC) == 0);
	return i;
}

void USART3_Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// CR1 /* Receiver Enable & Transmitter Enable */
	//MODIFY_REG(USART1->CR1, USART_CR1_M | USART_CR1_PS | USART_CR1_PCE | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8, USART_CR1_TE | USART_CR1_RE);
	SET_REG(USART3->CR1, USART_CR1_M, USART_CR1_M0);//00: 1 Start bit, 8+1 data bits, n stop bits
	SET_REG(USART3->CR1, USART_CR1_PS, 0);//0: Even parity
	SET_REG(USART3->CR1, USART_CR1_PCE, USART_CR1_PCE);//1: the computed parity is inserted at the MSB position(9th bit if M=1)
	SET_REG(USART3->CR1, USART_CR1_TE, USART_CR1_TE);
	SET_REG(USART3->CR1, USART_CR1_RE, USART_CR1_RE);//1
	//recv interrupt enable bit
	SET_REG(USART3->CR1, USART_CR1_TXEIE, 0);//0
	SET_REG(USART3->CR1, USART_CR1_TCIE, 0);//0
	SET_REG(USART3->CR1, USART_CR1_RXNEIE, USART_CR1_RXNEIE);//1
	SET_REG(USART3->CR1, USART_CR1_OVER8, 0);//0: Oversampling by 16; 1: Oversampling by 8
	// CR2
	//MODIFY_REG(USART1->CR2, USART_CR2_STOP, 0x0); // 1-bit stop
	SET_REG(USART3->CR2, USART_CR2_STOP, 0);//00: 1 stop bit

	// CR3
	SET_REG(USART3->CR3, USART_CR3_RTSE, 0);//0
	SET_REG(USART3->CR3, USART_CR3_CTSE, 0);//0
	SET_REG(USART3->CR3, USART_CR3_ONEBIT, 0);//0
	// BRR
	//MODIFY_REG(USART3->BRR, 0xFF, 4000000L/115200);
	SET_REG(USART3->BRR, 0xFF, 4000000 / 9600);//0
	//clk rate = 4MHz (MSI)
	/*
	In asynchronous mode, the following bits must be kept cleared:
	- LINEN and CLKEN bits in the USART_CR2 register,
	- SCEN, HDSEL and IREN bits in the USART_CR3 register.
	*/
	USART3->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
	USART3->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
	// Finally, enable UART
	USART3->CR1 |= (USART_CR1_UE);
}
void UART5_Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// CR1 /* Receiver Enable & Transmitter Enable */
	//MODIFY_REG(USART1->CR1, USART_CR1_M | USART_CR1_PS | USART_CR1_PCE | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8, USART_CR1_TE | USART_CR1_RE);
	SET_REG(UART5->CR1, USART_CR1_M, USART_CR1_M0);//00: 1 Start bit, 8+1 data bits, n stop bits
	SET_REG(UART5->CR1, USART_CR1_PS, 0);//0: Even parity
	SET_REG(UART5->CR1, USART_CR1_PCE, USART_CR1_PCE);//1: the computed parity is inserted at the MSB position(9th bit if M=1)
	SET_REG(UART5->CR1, USART_CR1_TE, USART_CR1_TE);
	SET_REG(UART5->CR1, USART_CR1_RE, USART_CR1_RE);//1
	//recv interrupt enable bit
	SET_REG(UART5->CR1, USART_CR1_TXEIE, 0);//0
	SET_REG(UART5->CR1, USART_CR1_TCIE, 0);//0
	SET_REG(UART5->CR1, USART_CR1_RXNEIE, USART_CR1_RXNEIE);//1
	SET_REG(UART5->CR1, USART_CR1_OVER8, 0);//0: Oversampling by 16; 1: Oversampling by 8
	// CR2
	//MODIFY_REG(USART1->CR2, USART_CR2_STOP, 0x0); // 1-bit stop
	SET_REG(UART5->CR2, USART_CR2_STOP, 0);//00: 1 stop bit

	// CR3
	SET_REG(UART5->CR3, USART_CR3_RTSE, 0);//0
	SET_REG(UART5->CR3, USART_CR3_CTSE, 0);//0
	SET_REG(UART5->CR3, USART_CR3_ONEBIT, 0);//0
	// BRR
	//MODIFY_REG(UART5->BRR, 0xFF, 4000000L/115200);
	SET_REG(UART5->BRR, 0xFF, 4000000 / 9600);//0
	//clk rate = 4MHz (MSI)
	/*
	In asynchronous mode, the following bits must be kept cleared:
	- LINEN and CLKEN bits in the USART_CR2 register,
	- SCEN, HDSEL and IREN bits in the USART_CR3 register.
	*/
	UART5->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
	UART5->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
	
	//UART5->CR1 |= (USART_CR1_UE);
}

void NVIC_config()
{
     NVIC_EnableIRQ(USART3_IRQn);
     NVIC_EnableIRQ(UART5_IRQn);
}

void USART3_IRQHandler(void){ //communicate interface
    char c;
    while ((USART3->ISR & USART_ISR_RXNE) == USART_ISR_RXNE){ 
	c = USART3->RDR;
    }
    int cmp=1;
    if (c == 13){ //\n
	UART3_Transmit("\r\n", 4);
	cmd[len]= 0; //remind :\0
	cmp = strcmp(cmd, "1");
	if (cmp == 0) { //yes
            UART5->CR1 |= (USART_CR1_UE);
	}
    }
    else if (c == 127 || c == 8){ //back delete
        if (len > 0){	
            UART3_Transmit(&c, 1);
            cmd[len--] = 0;
        }
    }
    else{
        UART3_Transmit(&c, 1);
        cmd[len++] = c;
    }
    UART3_Transmit("mode>", 5); 
}

void show( int max ){
    switch(max){
    case 0:
      UART3_Transmit("palm",4);
      break;
    case 1:
      UART3_Transmit("L",1);
      break;
    case 2:
      UART3_Transmit("fist",4);
      break;
    case 3:
      UART3_Transmit("fist_moved",10);
      break;
    case 4:
      UART3_Transmit("thumb",5);
      break;
    case 5:
      UART3_Transmit("index",5);
      break;
    case 6:
      UART3_Transmit("ok",2);
      break;    
    case 7:
      UART3_Transmit("palm_moved",10);
      break;
    case 8:
      UART3_Transmit("c",1);
      break;    
    case 9:
      UART3_Transmit("down",4);
      break;
    }
    UART3_Transmit("\r\n",4);
}

void RunInference(int *input, size_t input_size)
{
    float output[10];
    int res = aiRun(input, output);
    
    if (res !=0 ) {
        return;
    }
    int max=0;
    for (int i = 0; i < 10; i++) {
        if( output[max] < output[i] ) max = i;
    }
    show(max);
    UART5->CR1 &= ~(USART_CR1_UE);
}

void UART5_IRQHandler(void){ //only recv data
    
    while ((UART5->ISR & USART_ISR_RXNE) == USART_ISR_RXNE){ 
	ascii_buffer[ascii_count] = UART5->RDR;
        ascii_count++;
        if(ascii_count==4){ //get a float
            input[read]=(int)(ascii_buffer);
            ascii_count=0;
            read++;
        }
        if(read == SHAPE ){ // SHAPE = 12*32*1, flatten
            UART3_Transmit("Waiting for recognizing...",26);
            read = 0;
            RunInference(input, SHAPE);
            SET_REG(USART3->CR1, USART_CR1_RE, USART_CR1_RE);
            SET_REG(UART5->CR1, USART_CR1_RE, 0); 
        }
    }
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_USART2_UART_Init();
  MX_X_CUBE_AI_Init();
  /* USER CODE BEGIN 2 */
  USART3_Init();
  UART5_Init();
  UART3_Transmit("mode>", 5);
  
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2;
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
void MX_USART1_UART_Init(void)
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
