#ifndef __eUSART4_DRIVER_H
#define __eUSART4_DRIVER_H
#include"eTypedef.h"

#define   MUSART4RLEN   128

#define USART4_RxPin          GPIO_PIN_1
#define USART4_TxPin          GPIO_PIN_0
#define USART4_GPIO           GPIOB

#define USART4_DR_Base        (UART4_BASE + 0x04)

#define USART4_Tx_DMA_Channel DMA_CH7
#define USART4_Rx_DMA_Channel DMA_CH8


typedef struct
{
	uint8_t  rBuf[MUSART4RLEN];
	uint8_t  tBuf[MUSART4RLEN];
	uint16_t  rLen:14;
	uint16_t  reveceFlag :1; /* 接收标记 */
	uint16_t  txOver:1;
}is_usart4_rbuf;

void   eUsart4DmaInit(void);
void  eUsart4DmaSend(uint8_t *inBuf, uint16_t  len);
uint16_t eUart4ReadDat(uint8_t *outBuf,uint16_t len);


#endif



