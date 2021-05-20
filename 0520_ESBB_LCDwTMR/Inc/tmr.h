/*
 * tmr.h
 *
 *  Created on: 2021. 5. 20.
 *      Author: Marshall
 */

#ifndef TMR_H_
#define TMR_H_

#include <stdbool.h>

//#define TRUE  1
//#define FALSE 0

#define TMR_MAX_TMR 20

typedef struct{
    bool TmrEn;
    uint16_t TmrCtr;
    uint16_t TmrInit;
    void (*TmrFnct)(void* );
    void* TmrFnctArg;
} TMR;

extern TMR TmrTbl[TMR_MAX_TMR];

void TmrInit(TMR* ptmr);
void TmrSetT(TMR* ptmr, uint8_t n, uint16_t tenths);
void TmrCfgFnct(TMR* ptmr, uint8_t n, void(*fnct)(void*), void* arg);
void TmrStart(TMR* ptmr, uint8_t n);
void TmrTask(TMR* ptmr);

#endif /* TMR_H_ */
