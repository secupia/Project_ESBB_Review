/*
 * led.c
 *
 *  Created on: 2020. 12. 1.
 *      Author: Marshall
 */

#include "main.h"
#include "led.h"
#include "tmr.h"

void OnLed(void* data)
{

    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

    TmrSetT(TmrTbl, 0, 1000);
    TmrCfgFnct(TmrTbl, 0, OffLed, NULL);
    TmrStart(TmrTbl, 0);

}

void OffLed(void* data)
{

    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);

    TmrSetT(TmrTbl, 0, 1000);
    TmrCfgFnct(TmrTbl, 0, OnLed, NULL);
    TmrStart(TmrTbl, 0);

}
