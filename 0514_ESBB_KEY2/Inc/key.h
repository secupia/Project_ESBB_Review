/*
 * key.h
 *
 *  Created on: 2021. 5. 14.
 *      Author: Marshall
 */

#ifndef KEY_H_
#define KEY_H_

#include <stdbool.h>

//#define _DEBUG_KEY1
//#define _DEBUG_KEY2

#define TRUE                 1
#define FALSE                0

//#define KEY_PORT             GPIOC
//#define KEY_PORT_PIN         GPIO_PIN_12
#define KEY_BUF_SIZE         10

#define KEY_MAX_ROWS         2
#define KEY_MAX_COLS         2

#define KEY_RPT_DLY          2
#define KEY_RPT_START_DLY    10

#define KEY_SHIFT1_MSK       0x80
#define KEY_SHIFT1_OFFSET    4

#define KEY_SHIFT2_MSK       0x40
#define KEY_SHIFT2_OFFSET    8

#define KEY_ALL_ROWS         KEY_MAX_ROWS

typedef struct
{
    uint8_t Buf[KEY_BUF_SIZE];
    uint8_t BufInIx;
    uint8_t BufOutIx;
    uint8_t NRead;

    uint8_t RptStartDlyCtr;
    uint8_t RptDlyCtr;

    uint8_t ScanState;
} KEY;

//void KeyScanTask(void);
void KeyScanTask(KEY* Key);

//void KeyInit(void);
void KeyInit(KEY* Key);

//bool KeyHit(void);
bool KeyHit(KEY* Key);

//uint8_t KeyGetKey(void);
uint8_t KeyGetKey(KEY* Key);


#endif /* KEY_H_ */
