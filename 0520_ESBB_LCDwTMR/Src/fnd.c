/*
 * fnd.c
 *
 *  Created on: 2021. 5. 18.
 *      Author: Marshall
 */

#include "main.h"
#include "fnd.h"

//static uint8_t DispDigMsk;
//static uint8_t DispSegTbl[DISP_N_DIG];
//static uint8_t DispSegTblIx;

const   uint8_t DispASCIItoSegTbl[] = {
        0x00,
        0x00,
        0x44,
        0x00,
        0x00,
        0x00,
        0x00,
        0x40,
        0x9C,
        0xF0,
        0x00,
        0x00,
        0x00,
        0x02,
        0x00,
        0x00,
        0xFC,
        0x60,
        0xDA,
        0xF2,
        0x66,
        0xB6,
        0xBE,
        0xE0,
        0xFE,
        0xF6,
        0x00,
        0x00,
        0x00,
        0x12,
        0x00,
        0xCA,
        0x00,
        0xEE,
        0x3E,
        0x9C,
        0x7A,
        0x9E,
        0x8E,
        0xBC,
        0x6E,
        0x60,
        0x78,
        0x00,
        0x1C,
        0x00,
        0x2A,
        0xFC,
        0xCE,
        0x00,
        0x0A,
        0xB6,
        0x1E,
        0x7C,
        0x00,
        0x00,
        0x00,
        0x76,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0xFA,
        0x3E,
        0x1A,
        0x7A,
        0xDE,
        0x8E,
        0xBC,
        0x2E,
        0x20,
        0x78,
        0x00,
        0x1C,
        0x00,
        0x2A,
        0x3A,
        0xCE,
        0x00,
        0x0A,
        0xB6,
        0x1E,
        0x38,
        0x00,
        0x00,
        0x00,
        0x76,
        0x00
};

static void DispOutDig(uint8_t msk);
static void DispOutSeg(uint8_t seg);

static void DispOutDig(uint8_t msk)
{
    switch( msk )
    {
        case 0x80:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);      // COM1
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);    // COM2
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);     // COM3
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);     // COM4
            break;

        case 0x40:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);    // COM1
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);      // COM2
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);     // COM3
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);     // COM4
            break;

        case 0x20:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);    // COM1
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);    // COM2
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);       // COM3
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);     // COM4
            break;

        case 0x10:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);    // COM1
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);    // COM2
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);     // COM3
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);       // COM4
            break;
    }
}

static void DispOutSeg(uint8_t seg)
{
    //0x80 0x40 0x20 0x10 0x08 0x04  0x02  0x01
    //PA0, PA1, PA2, PA3, PB5, PC13, PC11, PA8
    if( seg & 0x80 )    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

    if( seg & 0x40 )    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

    if( seg & 0x20 )    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);

    if( seg & 0x10 )    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

    if( seg & 0x08 )    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

    if( seg & 0x04 )    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    if( seg & 0x02 )    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);

    if( seg & 0x01 )    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    else                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}

//void DispInit(void)
void DispInit(DISP* Disp)
{
    Disp->DigMsk = 0x80;
    Disp->SegTblIx = 0;
}

//void DispMuxHandler(void)
void DispMuxHandler(DISP* Disp)
{
    DispOutSeg(0x00);
    DispOutDig(Disp->DigMsk);
    DispOutSeg(Disp->SegTbl[Disp->SegTblIx]);
    if( Disp->SegTblIx == (DISP_N_DIG - 1))
    {
        Disp->SegTblIx = 0;
        Disp->DigMsk = 0x80;
    }
    else
    {
        Disp->SegTblIx++;
        Disp->DigMsk >>= 1;
    }
}

void DispStr(DISP* Disp, uint8_t dig, char *s)
{
    uint8_t stat;

    while( *s && dig < DISP_N_DIG )
    {
        stat = Disp->SegTbl[dig] & 0x01;

        Disp->SegTbl[dig++] = DispASCIItoSegTbl[*s++ - 0x20] | stat;    // 0x20: from Space
    }
}
