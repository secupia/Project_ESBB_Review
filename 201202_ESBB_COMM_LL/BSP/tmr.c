/*
 * tmr.c
 *
 *  Created on: 2020. 12. 1.
 *      Author: Marshall
 */

#include "main.h"
#include "tmr.h"

TMR TmrTbl[TMR_MAX_TMR];

void TmrInit(TMR* ptmr)
{

    uint8_t i;

    for(i=0; i<TMR_MAX_TMR; i++)
    {
        ptmr->TmrEn = FALSE;
        ptmr->TmrCtr = 0;
        ptmr->TmrInit = 0;
        ptmr->TmrFnct = NULL;
        ptmr++;
    }

}

void TmrSetT(TMR* ptmr, uint8_t n, uint16_t tenths)
{

    if( n < TMR_MAX_TMR)
    {
        (&ptmr[n])->TmrInit = tenths;
        (&ptmr[n])->TmrCtr  = tenths;
    }

}

void TmrCfgFnct(TMR* ptmr, uint8_t n, void(*fnct)(void*), void *arg)
{

    if( n < TMR_MAX_TMR)
    {
        (&ptmr[n])->TmrFnct = fnct;
        (&ptmr[n])->TmrFnctArg  = arg;
    }

}

void TmrStart(TMR* ptmr, uint8_t n)
{

    if( n < TMR_MAX_TMR)
    {
        (&ptmr[n])->TmrEn = TRUE;
    }

}

void TmrTask(TMR* ptmr)\
{

    uint8_t i;

    void (*pfnct)(void*);
    void *parg;

    pfnct = (void(*)(void*))0;    // function pointer to NULL

    for(i = 0; i < TMR_MAX_TMR; i++)
    {
        ////////// enter critical section //////////
        __disable_irq();
        if(ptmr->TmrEn == TRUE)
        {
            if(ptmr->TmrCtr > 0)
            {
                ptmr->TmrCtr--;
                if(ptmr->TmrCtr == 0)
                {
                    ptmr->TmrEn = FALSE;
                    pfnct = ptmr->TmrFnct;
                    parg  = ptmr->TmrFnctArg;

                }
            }
        }
        __enable_irq();
        ////////// exit critical section //////////

        if(pfnct != (void(*)(void*))0)    // function pointer is not NULL
        {
            (*pfnct)(parg);
            //pfnct(parg);
            pfnct = (void(*)(void*))0;
        }

        ptmr++;
    }




}
