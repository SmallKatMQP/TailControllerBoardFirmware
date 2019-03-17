/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void runControllers(void);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define led_Pin GPIO_PIN_14
#define led_GPIO_Port GPIOC
#define m1Dir_Pin GPIO_PIN_15
#define m1Dir_GPIO_Port GPIOC
#define m1EncA_Pin GPIO_PIN_0
#define m1EncA_GPIO_Port GPIOA
#define m1EncB_Pin GPIO_PIN_1
#define m1EncB_GPIO_Port GPIOA
#define m2EncA_Pin GPIO_PIN_2
#define m2EncA_GPIO_Port GPIOA
#define m2Dir_Pin GPIO_PIN_3
#define m2Dir_GPIO_Port GPIOA
#define m2EncB_Pin GPIO_PIN_4
#define m2EncB_GPIO_Port GPIOA
#define m3EncA_Pin GPIO_PIN_5
#define m3EncA_GPIO_Port GPIOA
#define m1PWM_Pin GPIO_PIN_6
#define m1PWM_GPIO_Port GPIOA
#define M2PWM_Pin GPIO_PIN_7
#define M2PWM_GPIO_Port GPIOA
#define m3PWM_Pin GPIO_PIN_0
#define m3PWM_GPIO_Port GPIOB
#define m4PWM_Pin GPIO_PIN_1
#define m4PWM_GPIO_Port GPIOB
#define m3EncB_Pin GPIO_PIN_8
#define m3EncB_GPIO_Port GPIOA
#define m4EncA_Pin GPIO_PIN_9
#define m4EncA_GPIO_Port GPIOA
#define m4EncB_Pin GPIO_PIN_10
#define m4EncB_GPIO_Port GPIOA
#define m4Dir_Pin GPIO_PIN_11
#define m4Dir_GPIO_Port GPIOA
#define ls1_Pin GPIO_PIN_12
#define ls1_GPIO_Port GPIOA
#define ls1_EXTI_IRQn EXTI15_10_IRQn
#define cs_Pin GPIO_PIN_15
#define cs_GPIO_Port GPIOA
#define cs_EXTI_IRQn EXTI15_10_IRQn
#define m3Dir_Pin GPIO_PIN_4
#define m3Dir_GPIO_Port GPIOB
#define ls2_Pin GPIO_PIN_6
#define ls2_GPIO_Port GPIOB
#define ls2_EXTI_IRQn EXTI9_5_IRQn
#define ls3_Pin GPIO_PIN_7
#define ls3_GPIO_Port GPIOB
#define ls3_EXTI_IRQn EXTI9_5_IRQn
#define ls4_Pin GPIO_PIN_3
#define ls4_GPIO_Port GPIOH
#define ls4_EXTI_IRQn EXTI3_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
