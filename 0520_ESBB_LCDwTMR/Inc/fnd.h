/*
 * fnd.h
 *
 *  Created on: 2021. 5. 18.
 *      Author: Marshall
 */

#ifndef FND_H_
#define FND_H_

#define DISP_N_DIG         4
#define DISP_N_SS          7    // SS: Seven Segment

typedef struct{
    uint8_t DigMsk;
    uint8_t SegTbl[DISP_N_DIG];
    uint8_t SegTblIx;
} DISP;

//void DispInit(void);
void DispInit(DISP* Disp);

//void DispMuxHandler(void);
void DispMuxHandler(DISP* Disp);

//void DispStr(uint8_t dig, char *s);
void DispStr(DISP* Disp, uint8_t dig, char *s);

#endif /* FND_H_ */
