/*
 * lcd.h
 *
 *  Created on: 2021. 5. 20.
 *      Author: Marshall
 */

#ifndef LCD_H_
#define LCD_H_

#define DISP_SEL_CMD_REG     0
#define DISP_SEL_DATA_REG    1


void LCD_DispInit(void);
void LCD_DispString(uint8_t row, uint8_t col, char* s);

#endif /* LCD_H_ */
