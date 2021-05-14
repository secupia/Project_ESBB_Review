/*
 * key.c
 *
 *  Created on: 2020. 11. 30.
 *      Author: Marshall
 */

#include "main.h"
#include "key.h"


static void KeySelRow(uint8_t row);
static uint8_t KeyGetCol(void);
static bool KeyIsKeyDown(void);
static uint8_t KeyDecode(void);
static void KeyBufIn(KEY* Key, uint8_t code);


static void KeySelRow(uint8_t row)
{
    switch(row)
    {
        case 0:
            HAL_GPIO_WritePin(KEY_ROW1_GPIO_Port, KEY_ROW1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(KEY_ROW2_GPIO_Port, KEY_ROW2_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            HAL_GPIO_WritePin(KEY_ROW1_GPIO_Port, KEY_ROW1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(KEY_ROW2_GPIO_Port, KEY_ROW2_Pin, GPIO_PIN_SET);
            break;
        case KEY_ALL_ROWS:
            HAL_GPIO_WritePin(KEY_ROW1_GPIO_Port, KEY_ROW1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(KEY_ROW2_GPIO_Port, KEY_ROW2_Pin, GPIO_PIN_RESET);
            break;
    }
}

static uint8_t KeyGetCol(void)
{

    uint8_t col = 0;

    if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)    // KEY1, Shift
        col |= 0x80;
    if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)    // KEY2, Shift
        col |= 0x40;
    if(HAL_GPIO_ReadPin(KEY_COL1_GPIO_Port, KEY_COL1_Pin) == GPIO_PIN_RESET)
        col |= 0x02;
    if(HAL_GPIO_ReadPin(KEY_COL2_GPIO_Port, KEY_COL2_Pin) == GPIO_PIN_RESET)
        col |= 0x01;
#ifdef _DEBUG_KEY
    if(col != 0)
        //printf("KeyGetCol:%d 0x%X\r\n", col, col);
#endif
    return col;
}

static bool KeyIsKeyDown(void)
{

    if( KeyGetCol() & ((1<<KEY_MAX_COLS) -1))    // 0x03 Output
        return TRUE;
    else
        return FALSE;

}

static uint8_t KeyDecode(void)
{

    uint8_t col;
    uint8_t row;
    uint8_t offset;
    bool    done;
    uint8_t col_id;
    uint8_t msk;

    done = FALSE;
    row  = 0;

    while( row < KEY_MAX_ROWS && !done)
    {
        KeySelRow(row);
        if( KeyIsKeyDown())
            done = TRUE;
        else
            row++;
    }

    col = KeyGetCol();
    offset = 0;

    if( col & KEY_SHIFT1_MSK )
        offset += KEY_SHIFT1_OFFSET;
    if( col & KEY_SHIFT2_MSK )
        offset += KEY_SHIFT2_OFFSET;

    msk = 0x01;
    col_id = 0;
    done = FALSE;

    while((col_id < KEY_MAX_COLS) && (!done))
    {
        if(col & msk)
        {
            done = TRUE;
        }
        else
        {
            col_id++;
            msk <<= 1;
        }
    }
#ifdef _DEBUG_KEY
    printf("col:%d col_id:%d row:%d offset:%d \r\n", col, col_id, row, offset);
#endif
    return(col_id + row*KEY_MAX_COLS + offset);
}

static void KeyBufIn(KEY* Key, uint8_t code)
{
    __disable_irq();
    if(Key->NRead < KEY_BUF_SIZE)
    {
        Key->NRead++;
        Key->Buf[Key->BufInIx++] = code;

        if(Key->BufInIx >= KEY_BUF_SIZE)
        {
            Key->BufInIx = 0;
        }
        __enable_irq();
    }
    else
    {
        __enable_irq();
    }

}



bool KeyHit(KEY* Key)
{

    bool hit;

    hit = (bool)(Key->NRead > 0) ? true : false;

    return hit;

}

uint8_t KeyGetKey(KEY* Key)
{

    uint8_t code;

    __disable_irq();       /* start critical section */
    if(Key->NRead > 0)
    {
        Key->NRead--;
        code = Key->Buf[Key->BufOutIx];
        Key->BufOutIx++;

        if(Key->BufOutIx >= KEY_BUF_SIZE)
            Key->BufOutIx = 0;
        __enable_irq();    /* end critical section */
        return code;
    }
    else
    {
        __enable_irq();    /* end critical section */
        return 0xFF;
    }

}

void    KeyInit(KEY* Key)
{

     KeySelRow(KEY_ALL_ROWS);

     Key->ScanState = KEY_STATE_UP;
     Key->NRead = 0;
     Key->BufInIx = 0;
     Key->BufOutIx = 0;

}

void KeyScanTask(KEY* Key)
{
    uint8_t code;

    switch(Key->ScanState)
    {
        case KEY_STATE_UP:
            if( KeyIsKeyDown() )
            {
                Key->ScanState = KEY_STATE_DEBOUNCE;
            }
            break;

        case KEY_STATE_DEBOUNCE:
            if( KeyIsKeyDown() )
            {
                code = KeyDecode();
                KeyBufIn(Key, code);
                Key->RptStartDlyCtr = KEY_RPT_START_DLY;
                Key->ScanState = KEY_STATE_RPT_START_DLY;
            }
            else
            {
                KeySelRow(KEY_ALL_ROWS);
                Key->ScanState = KEY_STATE_UP;
            }
            break;

        case KEY_STATE_RPT_START_DLY:
            if( KeyIsKeyDown() )
            {
                if(Key->RptStartDlyCtr > 0)
                {
                    Key->RptStartDlyCtr--;
                    if(Key->RptStartDlyCtr == 0)
                    {
                        code = KeyDecode();
                        KeyBufIn(Key, code);
                        Key->RptDlyCtr = KEY_RPT_DLY;
                        Key->ScanState = KEY_STATE_RPT_DLY;
                    }
                }
            }
            else
            {
                Key->ScanState = KEY_STATE_DEBOUNCE;
            }
            break;

        case KEY_STATE_RPT_DLY:
            if( KeyIsKeyDown() )
            {
                if(Key->RptDlyCtr > 0)
                {
                    Key->RptDlyCtr--;
                    if(Key->RptDlyCtr == 0)
                    {
                        code = KeyDecode();
                        KeyBufIn(Key, code);
                        Key->RptDlyCtr = KEY_RPT_DLY;
                    }
                }
            }
            else
            {
                Key->ScanState = KEY_STATE_DEBOUNCE;
            }
            break;
    }

}
