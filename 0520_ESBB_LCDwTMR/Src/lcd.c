/*
 * lcd.c
 *
 *  Created on: 2021. 5. 20.
 *      Author: Marshall
 */

#include "main.h"
#include "lcd.h"

#define DISP_CMD_CLS       0x01
#define DISP_CMD_FNCT      0x3B
#define DISP_CMD_MODE      0x06
#define DISP_CMD_ON_OFF    0x0C

#define DISP_MAX_ROWS      2
#define DISP_MAX_COLS      16

static void LCD_DispSel(uint8_t sel);
static void LCD_DispDataWr(uint8_t data);
static void LCD_DispCursorSet(uint8_t row, uint8_t col);

static void LCD_DispSel(uint8_t sel)
{

    if( sel == DISP_SEL_CMD_REG )
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
    }

}

static void LCD_DispDataWr(uint8_t data)
{

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

    if( data & 0x80)    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

    if( data & 0x40)    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);

    if( data & 0x20)    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);

    if( data & 0x10)    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);

    if( data & 0x08)    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);

    if( data & 0x04)    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);

    if( data & 0x02)    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

    if( data & 0x01)    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);

}

static void LCD_DispCursorSet(uint8_t row, uint8_t col)
{

    LCD_DispSel(DISP_SEL_CMD_REG);

    switch( row )
    {
        case 0:
            LCD_DispDataWr(0x80 + col);    // 0x80: B7<-1
            break;

        case 1:
            LCD_DispDataWr(0xC0 + col);    // 0xC0: B7<-1 | 0x40
            break;
    }
}

void LCD_DispInit(void)
{

    LCD_DispSel(DISP_SEL_CMD_REG);
    HAL_Delay(50);

    LCD_DispDataWr(DISP_CMD_FNCT);
    HAL_Delay(2);
    LCD_DispDataWr(DISP_CMD_FNCT);
    HAL_Delay(2);
    LCD_DispDataWr(DISP_CMD_FNCT);
    HAL_Delay(2);
    LCD_DispDataWr(DISP_CMD_FNCT);
    HAL_Delay(2);

    LCD_DispDataWr(DISP_CMD_ON_OFF);
    HAL_Delay(2);

    LCD_DispDataWr(DISP_CMD_MODE);
    HAL_Delay(2);

    LCD_DispDataWr(DISP_CMD_CLS);
    HAL_Delay(2);

}

void LCD_DispString(uint8_t row, uint8_t col, char* s)
{

    uint8_t i;
    if( row < DISP_MAX_ROWS && col < DISP_MAX_COLS )
    {
        LCD_DispCursorSet(row, col);
        HAL_Delay(2);    ///////////
        LCD_DispSel(DISP_SEL_DATA_REG);
        i = col;
        while( i < DISP_MAX_COLS && *s )    // *s is not null
        {
            LCD_DispDataWr(*s++);
            HAL_Delay(2);    ///////////
#if 0
            asm volatile ("NOP");
            asm volatile ("NOP");
            asm volatile ("NOP");
            asm volatile ("NOP");
#endif
            i++;
        }
    }

}
