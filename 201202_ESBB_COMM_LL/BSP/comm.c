/*
 * comm.c
 *
 *  Created on: 2020. 12. 1.
 *      Author: Marshall
 */

#include "main.h"
#include "comm.h"

COMM_RING_BUF CommBuf;

void CommInit(COMM_RING_BUF* pbuf)
{
//#define _DEBUG_COMM_INIT

    pbuf->RingBufRxCtr = 0;
    pbuf->RingBufRxInPtr  = &pbuf->RingBufRx[0];
    pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];


#ifdef _DEBUG_COMM_INIT
    printf("CommInit \r\n");
    printf("&pbuf->RingBufRx[0]   = %p \r\n", &pbuf->RingBufRx[0]);
    printf("&(pbuf->RingBufRx[0]) = %p \r\n", &(pbuf->RingBufRx[0]));
#endif

}

void CommPutRxChar(COMM_RING_BUF* pbuf, uint8_t c)
{

    if(pbuf->RingBufRxCtr < COMM_RX_BUF_SIZE)
    {
        __disable_irq();
        pbuf->RingBufRxCtr++;
        *(pbuf->RingBufRxInPtr) = c;
        pbuf->RingBufRxInPtr++;
        __enable_irq();

        if(pbuf->RingBufRxInPtr == &pbuf->RingBufRx[COMM_RX_BUF_SIZE])
        {
            pbuf->RingBufRxInPtr = &pbuf->RingBufRx[0];
        }
    }

}

uint8_t CommGetRxChar(COMM_RING_BUF* pbuf)
{

    uint8_t c;

    __disable_irq();
    if(pbuf->RingBufRxCtr > 0)
    {
        pbuf->RingBufRxCtr--;
        c = *(pbuf->RingBufRxOutPtr);
        pbuf->RingBufRxOutPtr++;
        if(pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[COMM_RX_BUF_SIZE])
        {
            pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
        }
    __enable_irq();
    return c;
    }
    else
    {
        __enable_irq();

        c = NULL;
        return NULL;
    }

}

bool CommIsEmpty(COMM_RING_BUF* pbuf)
{
    bool empty;

    if(pbuf->RingBufRxCtr > 0)
    {
        empty = FALSE;
    }
    else
    {
        empty = TRUE;
    }

    return empty;
}
