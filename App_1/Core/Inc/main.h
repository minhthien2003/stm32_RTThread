/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f0xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_1_Pin GPIO_PIN_13
#define LED_1_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_14
#define LED_2_GPIO_Port GPIOC
#define BOOT_Pin GPIO_PIN_15
#define BOOT_GPIO_Port GPIOC
#define IN_1_Pin GPIO_PIN_0
#define IN_1_GPIO_Port GPIOA
#define IN_2_Pin GPIO_PIN_1
#define IN_2_GPIO_Port GPIOA
#define SPI_CSN_Pin GPIO_PIN_4
#define SPI_CSN_GPIO_Port GPIOA
#define SPI_CE_Pin GPIO_PIN_0
#define SPI_CE_GPIO_Port GPIOB
#define IN_3_Pin GPIO_PIN_1
#define IN_3_GPIO_Port GPIOB
#define IN_4_Pin GPIO_PIN_2
#define IN_4_GPIO_Port GPIOB
#define IN_5_Pin GPIO_PIN_10
#define IN_5_GPIO_Port GPIOB
#define IN_6_Pin GPIO_PIN_11
#define IN_6_GPIO_Port GPIOB
#define IN_7_Pin GPIO_PIN_12
#define IN_7_GPIO_Port GPIOB
#define IN_8_Pin GPIO_PIN_13
#define IN_8_GPIO_Port GPIOB
#define IN_9_Pin GPIO_PIN_14
#define IN_9_GPIO_Port GPIOB
#define IN_10_Pin GPIO_PIN_15
#define IN_10_GPIO_Port GPIOB
#define EN_485_Pin GPIO_PIN_8
#define EN_485_GPIO_Port GPIOA
#define OUT_1_Pin GPIO_PIN_11
#define OUT_1_GPIO_Port GPIOA
#define OUT_2_Pin GPIO_PIN_12
#define OUT_2_GPIO_Port GPIOA
#define OUT_3_Pin GPIO_PIN_15
#define OUT_3_GPIO_Port GPIOA
#define OUT_4_Pin GPIO_PIN_3
#define OUT_4_GPIO_Port GPIOB
#define OUT_5_Pin GPIO_PIN_4
#define OUT_5_GPIO_Port GPIOB
#define OUT_6_Pin GPIO_PIN_5
#define OUT_6_GPIO_Port GPIOB
#define OUT_7_Pin GPIO_PIN_6
#define OUT_7_GPIO_Port GPIOB
#define OUT_8_Pin GPIO_PIN_7
#define OUT_8_GPIO_Port GPIOB
#define OUT_9_Pin GPIO_PIN_8
#define OUT_9_GPIO_Port GPIOB
#define OUT_10_Pin GPIO_PIN_9
#define OUT_10_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM14_Init(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
