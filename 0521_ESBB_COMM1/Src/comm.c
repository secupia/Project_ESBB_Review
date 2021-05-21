/*
 * comm.c
 *
 *  Created on: 2021. 5. 21.
 *      Author: Marshall
 */

#include "main.h"
#include "comm.h"

COMM_RING_BUF CommBuf;

extern UART_HandleTypeDef huart1;

void CommInit(COMM_RING_BUF* pbuf)
{
    pbuf->RingBufRxCtr = 0;
    pbuf->RingbufRxInPtr = &pbuf->RingBufRx[0];
    pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
}

void CommPutRxChar(COMM_RING_BUF* pbuf, uint8_t c)
{
    if( pbuf->RingBufRxCtr < COMM_RX_BUF_SIZE)
    {
        pbuf->RingBufRxCtr++;
        *(pbuf->RingbufRxInPtr) = c;    // data in
        pbuf->RingbufRxInPtr++;
        if( pbuf->RingbufRxInPtr == &pbuf->RingBufRx[COMM_RX_BUF_SIZE] )
        {
            pbuf->RingbufRxInPtr = &pbuf->RingBufRx[0];
        }
    }
}

uint8_t CommGetRxChar(COMM_RING_BUF* pbuf)
{

    uint8_t c;

    __HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE);    ////////// Critical Section
    if( pbuf->RingBufRxCtr > 0 )
    {
        pbuf->RingBufRxCtr--;
        c = *(pbuf->RingBufRxOutPtr);
        pbuf->RingBufRxOutPtr++;
        if( pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[COMM_RX_BUF_SIZE] )
        {
            pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
        }
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);    ////////// Critical Section
        return c;
    }
    else
    {
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);    ////////// Critical Section

        c = NULL;

        return c;
    }

}

bool CommIsEmpty(COMM_RING_BUF* pbuf)
{
    bool empty;

    __HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE);    ////////// Critical Section
    if( pbuf->RingBufRxCtr > 0 )
    {
        empty = FALSE;
    }
    else
    {
        empty = TRUE;
    }
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);    ////////// Critical Section
    return empty;

}
