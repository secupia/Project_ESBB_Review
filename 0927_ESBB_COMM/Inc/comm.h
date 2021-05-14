/*
 * comm.h
 *
 *  Created on: 2020. 9. 27.
 *      Author: Marshall
 */

#ifndef COMM_H_
#define COMM_H_

#include <stdbool.h>

#define TRUE                1
#define FALSE               0

#define COMM_RX_BUF_SIZE    128

#define NUL                0x00

typedef struct{
	uint16_t RingBufRxCtr;
	uint8_t* RingBufRxInPtr;
	uint8_t* RingBufRxOutPtr;
	uint8_t  RingBufRx[COMM_RX_BUF_SIZE];
} COMM_RING_BUF;

extern COMM_RING_BUF CommBuf;

void CommInit(COMM_RING_BUF* pbuf);
void CommPutRxChar(COMM_RING_BUF* pbuf, uint8_t c);
uint8_t CommGetRxChar(COMM_RING_BUF* pbuf);
bool CommIsEmpty(COMM_RING_BUF* pbuf);

#endif /* COMM_H_ */
