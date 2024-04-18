#include"eUsart1Driver.h"
#include<string.h>
#include "py32f002b_hal_uart.h"

is_usart1_rbuf  isUsart1Rbuf;
uint8_t  rByte[1];
UART_HandleTypeDef UartHandle;

  void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();
  
  /* GPIO initialization
  PB04：TX,
  PB05：RX
  */
  GPIO_InitStruct.Pin       = GPIO_PIN_3;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* USART1 interrupt enable */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART1_IRQn);

}




void  iUsart1RegInit(void)
{

    /* USART initialization */
  UartHandle.Instance          = USART1;
  UartHandle.Init.BaudRate     = 9600;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  HAL_UART_Init(&UartHandle);
  HAL_UART_Receive_IT(&UartHandle, (uint8_t *)rByte, 1) ;
  __HAL_UART_ENABLE_IT(&UartHandle,UART_IT_IDLE);
 eUsart1DmaSend("hellow word",10);
}


void iUart1NvicInit(void)
{
}

void   eUsart1DmaInit(void)
{
iUsart1RegInit();

}



void holdTime(uint32_t i)
{
  i*=5;//1US
  while(i--);
} 



//
void  eUsart1DmaSend(uint8_t *inBuf, uint16_t  len)
{
HAL_UART_Transmit_IT(&UartHandle, (uint8_t *)inBuf, len);
}


void HAL_UART_IdleFrameDetectCpltCallback(UART_HandleTypeDef *huart)
{
isUsart1Rbuf.reveceFlag =1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Receiving data through interruption */
HAL_UART_Receive_IT(UartHandle, (uint8_t *)rByte, 1) ;
  isUsart1Rbuf.rBuf[isUsart1Rbuf.rLen++] = rByte[0];
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
isUsart1Rbuf.txBusy = 0; //清除发送busy 标志
}



//读取接收数据长度
uint16_t eUart1ReadDat(uint8_t *outBuf,uint16_t len)
{

	uint16_t  rData =0;
	   if(isUsart1Rbuf.reveceFlag==1)
		   {
		   if(isUsart1Rbuf.rLen <len)
			   {
		   memcpy(outBuf,isUsart1Rbuf.rBuf,isUsart1Rbuf.rLen);
		   rData = isUsart1Rbuf.rLen;
			   }
		   isUsart1Rbuf.rLen =0;
		   isUsart1Rbuf.reveceFlag=0;
		   }
	   return rData;

}

