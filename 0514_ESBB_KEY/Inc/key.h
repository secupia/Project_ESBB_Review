/*
 * key.h
 *
 *  Created on: 2021. 5. 14.
 *      Author: Marshall
 */

#ifndef KEY_H_
#define KEY_H_

#include <stdbool.h>

#define TRUE                 1
#define FALSE                0

#define KEY_PORT             GPIOC
#define KEY_PORT_PIN         GPIO_PIN_12
#define KEY_BUF_SIZE         10

#define KEY_RPT_DLY          2
#define KEY_RPT_START_DLY    10

bool KeyHit(void);
uint8_t KeyGetKey(void);
void KeyInit(void);
void KeyScanTask(void);

#endif /* KEY_H_ */
