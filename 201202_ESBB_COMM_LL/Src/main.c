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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include "comm.h"
#include "key.h"
#include "led.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//volatile uint KeyScanFlag = RESET;
uint KeyScanFlag = RESET;
uint8_t RxData;


int _write(int file, char *ptr, int len)
{
  for(int i = 0; i < len; i++)
  {
    while(LL_USART_IsActiveFlag_TXE(USART1) == RESET)
    {
      //We can put some timeout here
    }
    LL_USART_TransmitData8(USART1, *(ptr+i));
  }
  return len;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    volatile static uint32_t Tick1m = 0;

    Tick1m++;

    if( (Tick1m % 10) == 0 )
    //if( (Tick1m % 100) == 0 )
    {
        //HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        KeyScanFlag = SET;
    }

    TmrTask(TmrTbl);

}
#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    //HAL_UART_Transmit(&huart1, &RxData, sizeof(RxData), 0xFFFFFFFF);

    CommPutRxChar(&CommBuf, RxData);
    HAL_UART_Receive_IT(&huart1, &RxData, sizeof(RxData));


}
#endif
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    uint8_t key;

    KEY stKey;
    uint8_t data;
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
  KeyInit(&stKey);

  TmrInit(TmrTbl);

  TmrSetT(TmrTbl, 0, 1000);
  TmrCfgFnct(TmrTbl, 0, OffLed, NULL);
  TmrStart(TmrTbl, 0);

  //HAL_UART_Receive_IT(&huart1, &RxData, sizeof(RxData));    // USART Rx interrupt enable
  LL_USART_EnableIT_RXNE(USART1);

  CommInit(&CommBuf);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#if 0
      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
      printf("Key scan module test \r\n");
      HAL_Delay(250);
#endif
      ////////// Critical section start //////////
      //__disable_irq();
      __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);
      if(KeyScanFlag == SET)
      {
          KeyScanFlag = RESET;
          // Do KeyScanTask()
          //HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
          KeyScanTask(&stKey);
      }
      //__enable_irq();
      __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
      ////////// Critical section end //////////

      if( KeyHit(&stKey) )
      {
          key = KeyGetKey(&stKey);
          printf("KEY : %d \r\n", key);

          HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
      }

      while( CommIsEmpty(&CommBuf) == FALSE)
      {
          data = CommGetRxChar(&CommBuf);

          //HAL_UART_Transmit(&huart1, &data, sizeof(data), 0xFFFFFFFF);

          /////////// Need to timeout condition ///////////
          USART1->DR = data;
          while(!(USART1->SR & USART_SR_TXE));
          /////////////////////////////////////////////////
      }
      //HAL_Delay(5);

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
