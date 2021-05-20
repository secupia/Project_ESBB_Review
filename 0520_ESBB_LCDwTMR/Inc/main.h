/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define FND_F_Pin GPIO_PIN_13
#define FND_F_GPIO_Port GPIOC
#define KEY_ROW1_Pin GPIO_PIN_14
#define KEY_ROW1_GPIO_Port GPIOC
#define KEY_ROW2_Pin GPIO_PIN_15
#define KEY_ROW2_GPIO_Port GPIOC
#define CLCD_D0_Pin GPIO_PIN_0
#define CLCD_D0_GPIO_Port GPIOC
#define CLCD_D1_Pin GPIO_PIN_1
#define CLCD_D1_GPIO_Port GPIOC
#define CLCD_D2_Pin GPIO_PIN_2
#define CLCD_D2_GPIO_Port GPIOC
#define CLCD_D3_Pin GPIO_PIN_3
#define CLCD_D3_GPIO_Port GPIOC
#define FND_A_Pin GPIO_PIN_0
#define FND_A_GPIO_Port GPIOA
#define FND_B_Pin GPIO_PIN_1
#define FND_B_GPIO_Port GPIOA
#define FND_C_Pin GPIO_PIN_2
#define FND_C_GPIO_Port GPIOA
#define FND_D_Pin GPIO_PIN_3
#define FND_D_GPIO_Port GPIOA
#define KEY_COL1_Pin GPIO_PIN_4
#define KEY_COL1_GPIO_Port GPIOA
#define KEY_COL2_Pin GPIO_PIN_5
#define KEY_COL2_GPIO_Port GPIOA
#define FND_DIG3_Pin GPIO_PIN_6
#define FND_DIG3_GPIO_Port GPIOA
#define FND_DIG4_Pin GPIO_PIN_7
#define FND_DIG4_GPIO_Port GPIOA
#define CLCD_D4_Pin GPIO_PIN_4
#define CLCD_D4_GPIO_Port GPIOC
#define CLCD_D5_Pin GPIO_PIN_5
#define CLCD_D5_GPIO_Port GPIOC
#define CLCD_D6_Pin GPIO_PIN_6
#define CLCD_D6_GPIO_Port GPIOC
#define CLCD_D7_Pin GPIO_PIN_7
#define CLCD_D7_GPIO_Port GPIOC
#define CLCD_EN_Pin GPIO_PIN_8
#define CLCD_EN_GPIO_Port GPIOC
#define CLCD_RW_Pin GPIO_PIN_9
#define CLCD_RW_GPIO_Port GPIOC
#define FND_DP_Pin GPIO_PIN_8
#define FND_DP_GPIO_Port GPIOA
#define FND_DIG1_Pin GPIO_PIN_11
#define FND_DIG1_GPIO_Port GPIOA
#define FND_DIG2_Pin GPIO_PIN_12
#define FND_DIG2_GPIO_Port GPIOA
#define CLCD_RS_Pin GPIO_PIN_10
#define CLCD_RS_GPIO_Port GPIOC
#define FND_G_Pin GPIO_PIN_11
#define FND_G_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_12
#define KEY1_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_2
#define KEY2_GPIO_Port GPIOD
#define FND_E_Pin GPIO_PIN_5
#define FND_E_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_8
#define LED3_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_9
#define LED4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
