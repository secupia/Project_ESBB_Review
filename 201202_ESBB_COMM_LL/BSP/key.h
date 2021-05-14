/*
 * key.h
 *
 *  Created on: 2020. 11. 30.
 *      Author: Marshall
 */

#ifndef KEY_H_
#define KEY_H_

#include <stdbool.h>

#define _DEBUG_KEY

#define TRUE  1
#define FALSE 0

#define KEY_BUF_SIZE  10

#define KEY_MAX_ROWS 2
#define KEY_MAX_COLS 2

#define KEY_RPT_DLY       20 /* 20: 10mSec x 20 */
#define KEY_RPT_START_DLY 100

#define KEY_SHIFT1_MSK    0x80
#define KEY_SHIFT1_OFFSET 4

#define KEY_SHIFT2_MSK    0x40
#define KEY_SHIFT2_OFFSET 8

#define KEY_ALL_ROWS    KEY_MAX_ROWS

#define KEY_STATE_UP             1
#define KEY_STATE_DEBOUNCE       2
#define KEY_STATE_RPT_START_DLY  3
#define KEY_STATE_RPT_DLY        4


typedef struct{
    uint8_t Buf[KEY_BUF_SIZE];
    uint8_t BufInIx;
    uint8_t BufOutIx;
    uint8_t NRead;

    uint8_t RptStartDlyCtr;
    uint8_t RptDlyCtr;

    uint8_t ScanState;
} KEY;

bool    KeyHit(KEY* Key);
uint8_t KeyGetKey(KEY* Key);
void    KeyInit(KEY* Key);
void    KeyScanTask(KEY* Key);

#endif /* KEY_H_ */
