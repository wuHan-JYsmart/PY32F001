#ifndef __eUSART1_DRIVER_H
#define __eUSART1_DRIVER_H
#include"eTypedef.h"

#define   MUSART1RLEN   128




typedef struct
{
	uint8_t  rBuf[MUSART1RLEN];
	uint8_t  tBuf[MUSART1RLEN];
	uint16_t  rLen:14;
	uint16_t  reveceFlag :1; /* 接收标记 */
	uint8_t  txBusy:1; //发送忙
}is_usart1_rbuf;

void   eUsart1DmaInit(void);
void  eUsart1DmaSend(uint8_t *inBuf, uint16_t  len);
uint16_t eUart1ReadDat(uint8_t *outBuf,uint16_t len);


#endif



