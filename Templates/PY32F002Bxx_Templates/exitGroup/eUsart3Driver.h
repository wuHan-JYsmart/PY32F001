#ifndef __eUSART3_DRIVER_H
#define __eUSART3_DRIVER_H
#include"eTypedef.h"

#define   MUSART3RLEN   128

#define USART3_RxPin          GPIO_PIN_11
#define USART3_TxPin          GPIO_PIN_10
#define USART3_GPIO           GPIOB

#define USART3_DR_Base        (UART3_BASE + 0x04)

#define USART3_Tx_DMA_Channel DMA_CH3
#define USART3_Rx_DMA_Channel DMA_CH4


typedef struct
{
	uint8_t  rBuf[MUSART3RLEN];
	uint8_t  tBuf[MUSART3RLEN];
	uint16_t  rLen:15;
	uint16_t  reveceFlag :1; /* 接收标记 */
}is_usart3_rbuf;

void   eUsart3DmaInit(void);
void  eUsart3DmaSend(uint8_t *inBuf, uint16_t  len);
uint16_t eUart3ReadDat(uint8_t *outBuf,uint16_t len);


#endif



