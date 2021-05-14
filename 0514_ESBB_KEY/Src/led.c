/*
 * led.c
 *
 *  Created on: 2021. 5. 14.
 *      Author: Marshall
 */

#include "main.h"
#include "led.h"

void OnLed(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);    // Low -> LED On
}

void OffLed(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);     // High -> LED Off
}
