/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
#include "PID.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define PWM_Period 800

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi3_rx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim16;


extern uint64_t m1Enc ,m2Enc, m3Enc, m4Enc;
GPIO_PinState m1A, m1B, m2A, m2B, m3A, m3B, m4A, m4B;
float M1Out, M2Out, M3Out, M4Out;

extern bool m1Homed, m2Homed , m3Homed , m4Homed ;

PID_Config M1 ;
PID_Config M2 ;
PID_Config M3 ;
PID_Config M4 ;

int64_t M1SetPoint, M2SetPoint, M3SetPoint, M4SetPoint;
int16_t _TXData[6];
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM16_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM7_Init(void);
void runControllers(void);
void HomeMotors(int motor);
int PCLK1_Freq;

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
	MX_DMA_Init();
	MX_TIM1_Init();
	MX_TIM16_Init();
	MX_SPI3_Init();
	MX_TIM7_Init();
	PCLK1_Freq = HAL_RCC_GetPCLK2Freq();
	if (HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_TIM_SET_AUTORELOAD(&htim1, PCLK1_Freq / PWM_Period);

	HAL_TIM_Base_Start(&htim16);
	HAL_TIM_Base_Start(&htim1);

	HAL_TIM_MspPostInit(&htim1);
	HAL_TIM_MspPostInit(&htim16);

	__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);
	/* USER CODE BEGIN 2 */

	//TODO: Init controller variables here

	/* USER CODE END 2 */
	HAL_SPI_Receive_DMA(&hspi3, _TXData, 6);

	MX_TIM7_Init();
	HAL_TIM_Base_Start(&htim7);

	__HAL_TIM_SET_COUNTER(&htim7, 1);
	__HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE);

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
		HAL_Delay(100);
		/* USER CODE END WHILE */

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
	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief SPI3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI3_Init(void)
{

	/* USER CODE BEGIN SPI3_Init 0 */

	/* USER CODE END SPI3_Init 0 */

	/* USER CODE BEGIN SPI3_Init 1 */

	/* USER CODE END SPI3_Init 1 */
	/* SPI3 parameter configuration*/
	hspi3.Instance = SPI3;
	hspi3.Init.Mode = SPI_MODE_SLAVE;
	hspi3.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
	hspi3.Init.DataSize = SPI_DATASIZE_16BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi3.Init.NSS = SPI_NSS_SOFT;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi3.Init.CRCPolynomial = 7;
	hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	if (HAL_SPI_Init(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI3_Init 2 */

	/* USER CODE END SPI3_Init 2 */

}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void)
{

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 0;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
	HAL_TIM_MspPostInit(&htim1);

}

/**
 * @brief TIM7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM7_Init(void)
{

	/* USER CODE BEGIN TIM7_Init 0 */

	/* USER CODE END TIM7_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM7_Init 1 */

	/* USER CODE END TIM7_Init 1 */
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 0;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 1000;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM7_Init 2 */

	/* USER CODE END TIM7_Init 2 */

}

/**
 * @brief TIM16 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM16_Init(void)
{

	/* USER CODE BEGIN TIM16_Init 0 */

	/* USER CODE END TIM16_Init 0 */

	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	/* USER CODE BEGIN TIM16_Init 1 */

	/* USER CODE END TIM16_Init 1 */
	htim16.Instance = TIM16;
	htim16.Init.Prescaler = 0;
	htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim16.Init.Period = 0;
	htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim16.Init.RepetitionCounter = 0;
	htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim16) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim16, &sBreakDeadTimeConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM16_Init 2 */

	/* USER CODE END TIM16_Init 2 */
	HAL_TIM_MspPostInit(&htim16);

}

/** 
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) 
{
	/* DMA controller clock enable */
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA2_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, led_Pin|m1Dir_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, m2Dir_Pin|m4Dir_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(m3Dir_GPIO_Port, m3Dir_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : led_Pin m1Dir_Pin */
	GPIO_InitStruct.Pin = led_Pin|m1Dir_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : m1EncA_Pin m1EncB_Pin m2EncA_Pin m2EncB_Pin
                           m3EncA_Pin m3EncB_Pin m4EncA_Pin m4EncB_Pin 
                           ls1_Pin cs_Pin */
	GPIO_InitStruct.Pin = m1EncA_Pin|m1EncB_Pin|m2EncA_Pin|m2EncB_Pin
			|m3EncA_Pin|m3EncB_Pin|m4EncA_Pin|m4EncB_Pin
			|ls1_Pin|cs_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : m2Dir_Pin m4Dir_Pin */
	GPIO_InitStruct.Pin = m2Dir_Pin|m4Dir_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : m3Dir_Pin */
	GPIO_InitStruct.Pin = m3Dir_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(m3Dir_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : ls2_Pin ls3_Pin */
	GPIO_InitStruct.Pin = ls2_Pin|ls3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : ls4_Pin */
	GPIO_InitStruct.Pin = ls4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ls4_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);M1SetPoint

	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}





//                           _______         _______
//               Pin1 ______|       |_______|       |______ Pin1
// negative <---         _______         _______         __      --> positive
//               Pin2 __|       |_______|       |_______|   Pin2

//	new	     new	old	     old
//	pin2	pin1	pin2	pin1	Result
//	----	----	----	----	------
//	 0	      0	      0	      0	   no movement
//	 0	      0	      0	      1	     +1
//	 0	      0	      1	      0	     -1
//	 0        0	      1	      1		 +2  (assume pin1 edges only)
//	 0        1	      0	      0		 -1
//	 0        1	      0	      1		 no movement
//	 0        1	      1	      0		 -2  (assume pin1 edges only)
//	 0        1	      1	      1		 +1
//	 1        0	      0	      0		 +1
//	 1        0	      0	      1		 -2  (assume pin1 edges only)
//	 1        0	      1	      0		 no movement
//	 1        0	      1	      1		 -1
//	 1        1	      0	      0		 +2  (assume pin1 edges only)
//	 1        1	      0	      1		 -1
//	 1        1	      1	      0		 +1
//	 1        1	      1	      1		 no movement
/*
 *
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){


	//homing interrupts
	if(GPIO_Pin == GPIO_PIN_12){
		m1Homed = true;
		m1Enc = 0;
	}
	else if(GPIO_Pin == GPIO_PIN_6){
		m2Homed = true;
		m2Enc = 0;
	}
	else if(GPIO_Pin == GPIO_PIN_7){
		m3Homed = true;
		m3Enc = 0;
	}
	else if(GPIO_Pin == GPIO_PIN_3){
		m4Homed = true;
		m4Enc = 0;
	}

	//Encoder interrupts

	else if(GPIO_Pin == GPIO_PIN_0||GPIO_Pin == GPIO_PIN_1){//9cs
		int encA = m1EncA_GPIO_Port->IDR;
		int encB = m1EncB_GPIO_Port->IDR;
		if ((m1A == GPIO_PIN_RESET && m1B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET )||
				(m1A == GPIO_PIN_RESET && m1B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET )||
				(m1A == GPIO_PIN_SET && m1B == GPIO_PIN_RESET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET )||
				(m1A == GPIO_PIN_SET && m1B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_SET )){
			m1Enc+=0;
		}
		else if ((m1A == GPIO_PIN_RESET && m1B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET)||
				(m1A == GPIO_PIN_RESET && m1B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_SET)||
				(m1A == GPIO_PIN_SET && m1B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET)||
				(m1A == GPIO_PIN_SET && m1B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET)){

			m1Enc+=1;
			m1A = encA;
			m1B = encB;

		}
		else if ((m1A == GPIO_PIN_RESET && m1B == GPIO_PIN_RESET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET)||
				(m1A == GPIO_PIN_RESET && m1B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET)||
				(m1A == GPIO_PIN_SET && m1B == GPIO_PIN_RESET &&encA == GPIO_PIN_SET && encB == GPIO_PIN_SET)||
				(m1A == GPIO_PIN_SET && m1B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET)){

			m1Enc-=1;
			m1A = encA;
			m1B = encB;

		}

	}
	else if(GPIO_Pin == GPIO_PIN_0||GPIO_Pin == GPIO_PIN_1){
		int encA = m2EncA_GPIO_Port->IDR;
		int encB = m2EncB_GPIO_Port->IDR;
		if ((m1A == GPIO_PIN_RESET && m2B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET )||
				(m2A == GPIO_PIN_RESET && m2B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET )||
				(m2A == GPIO_PIN_SET && m2B == GPIO_PIN_RESET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET )||
				(m2A == GPIO_PIN_SET && m2B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_SET )){
			m2Enc+=0;
		}
		else if ((m2A == GPIO_PIN_RESET && m2B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET)||
				(m2A == GPIO_PIN_RESET && m2B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_SET)||
				(m2A == GPIO_PIN_SET && m2B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET)||
				(m2A == GPIO_PIN_SET && m2B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET)){

			m2Enc+=1;
			m2A = encA;
			m2B = encB;

		}
		else if ((m2A == GPIO_PIN_RESET && m2B == GPIO_PIN_RESET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET)||
				(m2A == GPIO_PIN_RESET && m2B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET)||
				(m2A == GPIO_PIN_SET && m2B == GPIO_PIN_RESET &&encA == GPIO_PIN_SET && encB == GPIO_PIN_SET)||
				(m2A == GPIO_PIN_SET && m2B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET)){

			m2Enc-=1;
			m2A = encA;
			m2B = encB;

		}

	}
	else if(GPIO_Pin == GPIO_PIN_0||GPIO_Pin == GPIO_PIN_1){
		int encA = m3EncA_GPIO_Port->IDR;
		int encB = m3EncB_GPIO_Port->IDR;
		if ((m3A == GPIO_PIN_RESET && m3B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET )||
				(m3A == GPIO_PIN_RESET && m3B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET )||
				(m3A == GPIO_PIN_SET && m3B == GPIO_PIN_RESET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET )||
				(m3A == GPIO_PIN_SET && m3B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_SET )){
			m3Enc+=0;
		}
		else if ((m3A == GPIO_PIN_RESET && m3B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET)||
				(m3A == GPIO_PIN_RESET && m3B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_SET)||
				(m3A == GPIO_PIN_SET && m3B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET)||
				(m3A == GPIO_PIN_SET && m3B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET)){

			m3Enc+=1;
			m3A = encA;
			m3B = encB;

		}
		else if ((m3A == GPIO_PIN_RESET && m3B == GPIO_PIN_RESET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET)||
				(m3A == GPIO_PIN_RESET && m3B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET)||
				(m3A == GPIO_PIN_SET && m3B == GPIO_PIN_RESET &&encA == GPIO_PIN_SET && encB == GPIO_PIN_SET)||
				(m3A == GPIO_PIN_SET && m3B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET)){

			m3Enc-=1;
			m3A = encA;
			m3B = encB;

		}

	}
	else if(GPIO_Pin == GPIO_PIN_0||GPIO_Pin == GPIO_PIN_1){
		int encA = m4EncA_GPIO_Port->IDR;
		int encB = m4EncB_GPIO_Port->IDR;
		if ((m4A == GPIO_PIN_RESET && m4B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET )||
				(m4A == GPIO_PIN_RESET && m4B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET )||
				(m4A == GPIO_PIN_SET && m4B == GPIO_PIN_RESET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET )||
				(m4A == GPIO_PIN_SET && m4B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_SET )){
			m4Enc+=0;
		}
		else if ((m4A == GPIO_PIN_RESET && m4B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET)||
				(m4A == GPIO_PIN_RESET && m4B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_SET)||
				(m4A == GPIO_PIN_SET && m4B == GPIO_PIN_SET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET)||
				(m4A == GPIO_PIN_SET && m4B == GPIO_PIN_RESET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET)){

			m4Enc+=1;
			m4A = encA;
			m4B = encB;

		}
		else if ((m4A == GPIO_PIN_RESET && m4B == GPIO_PIN_RESET && encA == GPIO_PIN_SET && encB == GPIO_PIN_RESET)||
				(m4A == GPIO_PIN_RESET && m4B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_RESET)||
				(m4A == GPIO_PIN_SET && m4B == GPIO_PIN_RESET &&encA == GPIO_PIN_SET && encB == GPIO_PIN_SET)||
				(m4A == GPIO_PIN_SET && m4B == GPIO_PIN_SET && encA == GPIO_PIN_RESET && encB == GPIO_PIN_SET)){

			m4Enc-=1;
			m4A = encA;
			m4B = encB;

		}

	}
	else if (GPIO_Pin == GPIO_PIN_15){
		HAL_SPI_Receive_DMA(&hspi3, _TXData, 6);
		if(_TXData[0] == 0x22){
			//home

		}
		else if (_TXData[0] == 0x33){
			//tx[1] motor to set position in 2, 3, 4, 5
			if(_TXData[1] == 1){
				if(m1Homed){
					M1SetPoint = _TXData[2]<<48|_TXData[3]<<32|_TXData[4]<<16|_TXData[5];
				}
				else{
					return;
				}
			}

			else if(_TXData[1] == 2){
				if(m2Homed){
					M3SetPoint = _TXData[2]<<48|_TXData[3]<<32|_TXData[4]<<16|_TXData[5];
				}
				else{
					return;
				}
			}

			else if(_TXData[1] == 3){
				if(m3Homed){
					M3SetPoint = _TXData[2]<<48|_TXData[3]<<32|_TXData[4]<<16|_TXData[5];
				}
				else{
					return;
				}
			}

			else if(_TXData[1] == 4){
				if(m4Homed){
					M4SetPoint = _TXData[2]<<48|_TXData[3]<<32|_TXData[4]<<16|_TXData[5];
				}
				else{
					return;
				}
			}
		}
		else if(_TXData[0]==0xAA){
			//Set PIDS
			if(_TXData[1] == 1){
				setPID(M1, (float)(_TXData[2]/100), (float)(_TXData[3]/100), (float)(_TXData[4]/100));
			}

			else if(_TXData[1] == 2){
				setPID(M2, (float)(_TXData[2]/100), (float)(_TXData[3]/100), (float)(_TXData[4]/100));

			}

			else if(_TXData[1] == 3){
				setPID(M3, (float)(_TXData[2]/100), (float)(_TXData[3]/100), (float)(_TXData[4]/100));

			}

			else if(_TXData[1] == 4){
				setPID(M4, (float)(_TXData[2]/100), (float)(_TXData[3]/100), (float)(_TXData[4]/100));

			}
		}
		else if(_TXData[0]==0xAB){
			//Set grav
			if(_TXData[1] == 1){
				setGrav(M1, (float)(_TXData[2]/1000));
			}

			else if(_TXData[1] == 2){
				setGrav(M2, (float)(_TXData[2]/1000));

			}

			else if(_TXData[1] == 3){
				setGrav(M3, (float)(_TXData[2]/1000));

			}

			else if(_TXData[1] == 4){
				setGrav(M4, (float)(_TXData[2]/1000));

			}
		}
		else if(_TXData[0]==0xAC){
			//Set coriolis
			if(_TXData[1] == 1){
				setCor(M1, (float)(_TXData[2]/1000));
			}

			else if(_TXData[1] == 2){
				setCor(M2, (float)(_TXData[2]/1000));

			}

			else if(_TXData[1] == 3){
				setCor(M3, (float)(_TXData[2]/1000));

			}

			else if(_TXData[1] == 4){
				setCor(M4, (float)(_TXData[2]/1000));

			}
		}



	}

}

void runControllers(){

	setSetpoint(M1, M1SetPoint);
	setPos(M1, m1Enc);
	M1Out = Compute(M1);

	setSetpoint(M2, M1SetPoint);
	setPos(M2, m2Enc);
	M3Out = Compute(M2);

	setSetpoint(M3, M3SetPoint);
	setPos(M3, m3Enc);
	M3Out = Compute(M3);

	setSetpoint(M4, M4SetPoint);
	setPos(M4, m4Enc);
	M4Out = Compute(M4);



	if(M1Out>0){
		m1Dir_GPIO_Port->ODR |= m1Dir_Pin; //Set m1Dir pin high

	}
	else if (M1Out<0){
		m1Dir_GPIO_Port->ODR &= ~m1Dir_Pin; //Set m1Dir pin Low

	}
	else if(M1Out == 0){

	}
	__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, PCLK1_Freq / PWM_Period * M1Out );

	if(M2Out>0){
		m2Dir_GPIO_Port->ODR |= m2Dir_Pin; //Set m1Dir pin high

	}
	else if (M2Out<0){
		m2Dir_GPIO_Port->ODR &= ~m2Dir_Pin; //Set m1Dir pin Low

	}
	else if(M2Out == 0){

	}
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PCLK1_Freq / PWM_Period * M2Out );

	if(M3Out>0){
		m3Dir_GPIO_Port->ODR |= m3Dir_Pin; //Set m1Dir pin high

	}
	else if (M3Out<0){
		m3Dir_GPIO_Port->ODR &= ~m3Dir_Pin; //Set m1Dir pin Low

	}
	else if(M3Out == 0){

	}
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PCLK1_Freq / PWM_Period * M3Out );

	if(M4Out>0){
		m4Dir_GPIO_Port->ODR |= m4Dir_Pin; //Set m1Dir pin high

	}
	else if (M4Out<0){
		m4Dir_GPIO_Port->ODR &= ~m4Dir_Pin; //Set m1Dir pin Low

	}
	else if(M4Out == 0){

	}
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PCLK1_Freq / PWM_Period * M4Out );


}

void HomeMotors(int motor){

	if(motor == 1){
		while(!m1Homed){
			m1Dir_GPIO_Port->ODR |= m1Dir_Pin; //this may have to be inverted at test time
			__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, PCLK1_Freq / PWM_Period * 0.1 );

		}
	}
	else if(motor == 2){
		while(!m2Homed){
			m2Dir_GPIO_Port->ODR |= m2Dir_Pin; //this may have to be inverted at test time
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PCLK1_Freq / PWM_Period * 0.1 );

		}
	}
	else if(motor == 3){
		while(!m3Homed){
			m3Dir_GPIO_Port->ODR |= m3Dir_Pin; //this may have to be inverted at test time
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PCLK1_Freq / PWM_Period * 0.1 );

		}
	}
	else if(motor == 4){
		while(!m4Homed){
			m4Dir_GPIO_Port->ODR |= m4Dir_Pin; //this may have to be inverted at test time
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PCLK1_Freq / PWM_Period * 0.1 );

		}
	}

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
