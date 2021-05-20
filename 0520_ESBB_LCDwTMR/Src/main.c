/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include "led.h"
#include "key.h"
#include "fnd.h"
#include "lcd.h"
#include "tmr.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint32_t Tick1m;
uint32_t KeyScanFlag = RESET;
uint8_t  DispScanFlag = RESET;

DISP Disp;    // GLobal variable

//TMR TmrTbl[TMR_MAX_TMR];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    Tick1m++;

    if( (Tick1m % 100) == 0)
    {
        //KeyScanTask();
        KeyScanFlag = SET;
    }

    if(  (Tick1m % 5) == 0 )
    {
        //DispMuxHandler();
        //DispScanFlag = SET;
        DispMuxHandler(&Disp);
    }

    TmrTask(TmrTbl);
}

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 0xFFFFFFFF);

    return len;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  //uint8_t key = 0;
  //uint8_t key = 0x2F;
  int8_t key = -10;

  KEY stKey;
  KEY* Key = &stKey ;

  char dispString[4] = "    ";

  //DISP Disp;    // Local variable
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
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);

  //KeyInit(&Key);
  KeyInit(Key);

  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);    // LED1 OFF
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);    // LED2 OFF
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);    // LED3 OFF
  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);    // LED4 OFF

  //DispInit();
  DispInit(&Disp);

  TmrInit(TmrTbl);

  LCD_DispInit();
  //LCD_DispString(0, 5, "Key Code");
  LCD_DispString(0, 0, "STM32F103");


#if 0
  TmrSetT(TmrTbl, 0, 1000);
  TmrCfgFnct(TmrTbl, 0, OnLed, NULL);
  TmrStart(TmrTbl,0);
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);        /*** start critical section ***/
      if ( KeyScanFlag == SET)
      {
          KeyScanFlag = RESET;
          __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);     /*** end critical section ***/
          //KeyScanTask();
          //KeyScanTask(&Key);
          KeyScanTask(Key);
      }
      __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);         /*** end critical section ***/

      //if( KeyHit() )
      //if( KeyHit(&Key) )
      if( KeyHit(Key) )
      {

          //key = KeyGetKey();
          //key = KeyGetKey(&Key);
          key = KeyGetKey(Key);

          switch(key)
          {
              case 0:
              case 4:
              case 8:
                  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                  break;
              case 1:
              case 5:
              case 9:
                  HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
                  break;
              case 2:
              case 6:
              case 10:
                  HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
                  break;
              case 3:
              case 7:
              case 11:
                  HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
                  break;
          }
          printf("KEY : %d \r\n", key);

          if(key >= 10){
              dispString[2] = key / 10 + 0x30;
              dispString[3] = key % 10 + 0x30;
          }
          else {
              dispString[2] = 0x20;          // 0x20: Space
              dispString[3] = key + 0x30;    // ASCII Code of '0' : 0x30
          }
          DispStr(&Disp, 0, dispString);
          LCD_DispString(1, 0, dispString);
      }
#if 0
      __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);
      if ( DispScanFlag == SET )
      {
          DispScanFlag = RESET;
          __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
          //DispMuxHandler();
          DispMuxHandler(&Disp);
      }
      __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
#endif

#if 0
      if(key >= 10){
          dispString[2] = key / 10 + 0x30;
          dispString[3] = key % 10 + 0x30;
      }
      else {
          dispString[2] = 0x20;          // 0x20: Space
          dispString[3] = key + 0x30;    // ASCII Code of '0' : 0x30
      }
      DispStr(&Disp, 0, dispString);
#endif

#if 0
      DispStr(&Disp, 0, "abcd");
#endif

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 8-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, FND_F_Pin|KEY_ROW1_Pin|KEY_ROW2_Pin|CLCD_D0_Pin 
                          |CLCD_D1_Pin|CLCD_D2_Pin|CLCD_D3_Pin|CLCD_D4_Pin 
                          |CLCD_D5_Pin|CLCD_D6_Pin|CLCD_D7_Pin|CLCD_EN_Pin 
                          |CLCD_RW_Pin|CLCD_RS_Pin|FND_G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, FND_A_Pin|FND_B_Pin|FND_C_Pin|FND_D_Pin 
                          |FND_DIG3_Pin|FND_DIG4_Pin|FND_DP_Pin|FND_DIG1_Pin 
                          |FND_DIG2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, FND_E_Pin|LED1_Pin|LED2_Pin|LED3_Pin 
                          |LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : FND_F_Pin KEY_ROW1_Pin KEY_ROW2_Pin CLCD_D0_Pin 
                           CLCD_D1_Pin CLCD_D2_Pin CLCD_D3_Pin CLCD_D4_Pin 
                           CLCD_D5_Pin CLCD_D6_Pin CLCD_D7_Pin CLCD_EN_Pin 
                           CLCD_RW_Pin CLCD_RS_Pin FND_G_Pin */
  GPIO_InitStruct.Pin = FND_F_Pin|KEY_ROW1_Pin|KEY_ROW2_Pin|CLCD_D0_Pin 
                          |CLCD_D1_Pin|CLCD_D2_Pin|CLCD_D3_Pin|CLCD_D4_Pin 
                          |CLCD_D5_Pin|CLCD_D6_Pin|CLCD_D7_Pin|CLCD_EN_Pin 
                          |CLCD_RW_Pin|CLCD_RS_Pin|FND_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : FND_A_Pin FND_B_Pin FND_C_Pin FND_D_Pin 
                           FND_DIG3_Pin FND_DIG4_Pin FND_DP_Pin FND_DIG1_Pin 
                           FND_DIG2_Pin */
  GPIO_InitStruct.Pin = FND_A_Pin|FND_B_Pin|FND_C_Pin|FND_D_Pin 
                          |FND_DIG3_Pin|FND_DIG4_Pin|FND_DP_Pin|FND_DIG1_Pin 
                          |FND_DIG2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_COL1_Pin KEY_COL2_Pin */
  GPIO_InitStruct.Pin = KEY_COL1_Pin|KEY_COL2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY1_Pin */
  GPIO_InitStruct.Pin = KEY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY2_Pin */
  GPIO_InitStruct.Pin = KEY2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : FND_E_Pin LED1_Pin LED2_Pin LED3_Pin 
                           LED4_Pin */
  GPIO_InitStruct.Pin = FND_E_Pin|LED1_Pin|LED2_Pin|LED3_Pin 
                          |LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
