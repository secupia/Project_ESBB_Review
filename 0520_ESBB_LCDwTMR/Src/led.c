/*
 * led.c
 *
 *  Created on: 2021. 5. 14.
 *      Author: Marshall
 */

#include "main.h"
#include "led.h"
#include "tmr.h"

void OnLed(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);    // Low -> LED On

    TmrSetT(TmrTbl, 0, 1000);
    TmrCfgFnct(TmrTbl, 0, OffLed, NULL);
    TmrStart(TmrTbl,0);

}

void OffLed(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);     // High -> LED Off

    TmrSetT(TmrTbl, 0, 1000);
    TmrCfgFnct(TmrTbl, 0, OnLed, NULL);
    TmrStart(TmrTbl,0);
}
