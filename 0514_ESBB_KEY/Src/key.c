/*
 * key.c
 *
 *  Created on: 2021. 5. 14.
 *      Author: Marshall
 */

#include "main.h"
#include "key.h"

#define KEY_STATE_UP               1
#define KEY_STATE_DEBOUNCE         2
#define KEY_STATE_RPT_START_DLY    3
#define KEY_STATE_RPT_DLY          4

static uint8_t KeyBuf[KEY_BUF_SIZE];
static uint8_t KeyBufInIx;
static uint8_t KeyBufOutIx;
static uint8_t KeyNRead;

static uint8_t KeyRptStartDlyCtr;
static uint8_t KeyRptDlyCtr;

static uint8_t KeyScanState;

static void KeyBufIn(uint8_t code);

static void KeyBufIn(uint8_t code)
{
    if( KeyNRead < KEY_BUF_SIZE )
    {
        KeyNRead++;
        KeyBuf[KeyBufInIx++] = code;
        if( KeyBufInIx >= KEY_BUF_SIZE )
        {
            KeyBufInIx = 0;
        }
    }
}

bool KeyHit(void)
{
    bool hit;

    hit = (bool)(KeyNRead > 0) ? TRUE : FALSE;

    return hit;
}

uint8_t KeyGetKey(void)
{
    uint8_t code;

    if( KeyNRead > 0 )
    {
        KeyNRead--;
        code = KeyBuf[KeyBufOutIx];
        KeyBufOutIx++;
        if( KeyBufOutIx >= KEY_BUF_SIZE )
        {
            KeyBufOutIx = 0;
        }

        return code;
    }
    else
    {
        return 0xFF;
    }
}

void KeyInit(void)
{
    KeyScanState = KEY_STATE_UP;
    KeyNRead = 0;
    KeyBufInIx = 0;
    KeyBufOutIx = 0;
}

void KeyScanTask(void)
{
    switch( KeyScanState )
    {
        case KEY_STATE_UP:
            if( HAL_GPIO_ReadPin(KEY_PORT, KEY_PORT_PIN) == GPIO_PIN_RESET )
            {
                KeyScanState = KEY_STATE_DEBOUNCE;
            }
            break;

        case KEY_STATE_DEBOUNCE:
            if( HAL_GPIO_ReadPin(KEY_PORT, KEY_PORT_PIN) == GPIO_PIN_RESET )
            {
                KeyBufIn(1);    // 1: stored key code
                KeyRptStartDlyCtr = KEY_RPT_START_DLY;
                KeyScanState = KEY_STATE_RPT_START_DLY;
            }
            else
            {
                KeyScanState = KEY_STATE_UP;
            }
            break;

        case KEY_STATE_RPT_START_DLY:
            if( HAL_GPIO_ReadPin(KEY_PORT, KEY_PORT_PIN) == GPIO_PIN_RESET )
            {
                if( KeyRptStartDlyCtr > 0)
                {
                    KeyRptStartDlyCtr--;
                    if( KeyRptStartDlyCtr == 0)
                    {
                        KeyBufIn(1);
                        KeyRptDlyCtr = KEY_RPT_DLY;
                        KeyScanState = KEY_STATE_RPT_DLY;
                    }
                }
            }
            else
            {
                KeyScanState = KEY_STATE_DEBOUNCE;
            }
            break;

        case KEY_STATE_RPT_DLY:
            if( HAL_GPIO_ReadPin(KEY_PORT, KEY_PORT_PIN) == GPIO_PIN_RESET )
            {
                if( KeyRptDlyCtr > 0)
                {
                    KeyRptDlyCtr--;
                    if( KeyRptStartDlyCtr == 0)
                    {
                        KeyBufIn(1);
                        KeyRptDlyCtr = KEY_RPT_DLY;
                    }
                }
            }
            else
            {
                KeyScanState = KEY_STATE_DEBOUNCE;
            }
            break;
    }
}
