#ifndef __eUSART2_DRIVER_H
#define __eUSART2_DRIVER_H
#include"eTypedef.h"

#define   MUSART2RLEN   128

#define USART2_RxPin          GPIO_PIN_5
#define USART2_TxPin          GPIO_PIN_4
#define USART2_GPIO           GPIOB

#define USART2_DR_Base        (USART2_BASE + 0x04)

#define USART2_Tx_DMA_Channel DMA_CH5
#define USART2_Rx_DMA_Channel DMA_CH6


typedef struct
{
	uint8_t  rBuf[MUSART2RLEN];
	uint8_t  tBuf[MUSART2RLEN];
	uint16_t  rLen:15;
	uint16_t  reveceFlag :1; /* 接收标记 */
}is_usart2_rbuf;

void   eUsart2DmaInit(void);
void  eUsart2DmaSend(uint8_t *inBuf, uint16_t  len);
uint16_t eUart2ReadDat(uint8_t *outBuf,uint16_t len);


#endif



