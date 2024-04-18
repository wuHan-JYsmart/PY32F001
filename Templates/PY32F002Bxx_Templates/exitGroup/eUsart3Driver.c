#include"eUsart3Driver.h"
#include<string.h>

is_usart3_rbuf  isUsart3Rbuf;


void  iUsart3RccInit(void)
{
  /* DMA clock enable */
  RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA);
  /* Enable GPIO clock */
  RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB );
  RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
  RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_UART3);
}

void  iUsart3GpioInit(void)
{
  
  GPIO_InitType GPIO_InitStructure;
  
  /* Configure USART3 Tx as alternate function push-pull */
  GPIO_InitStructure.Pin			  = USART3_TxPin;
  GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;    
  GPIO_InitStructure.GPIO_Alternate = GPIO_AF10_UART3;
  GPIO_Peripheral_Initialize(USART3_GPIO, &GPIO_InitStructure);
  
  /* Configure USART3 Rx as alternate function push-pull and pull-up */
  GPIO_InitStructure.Pin			  = USART3_RxPin;
  GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;    
  GPIO_InitStructure.GPIO_Alternate = GPIO_AF10_UART3;
  GPIO_Peripheral_Initialize(USART3_GPIO, &GPIO_InitStructure); 
}



void  iUsart3DmaInit(void)
{
  
  DMA_InitType DMA_InitStructure;
  
  /* USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config */
  DMA_Reset(USART3_Tx_DMA_Channel);
  DMA_InitStructure.PeriphAddr     = USART3_DR_Base;
  DMA_InitStructure.MemAddr        = (uint32_t)(uint32_t)isUsart3Rbuf.tBuf;
  DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;
  DMA_InitStructure.BufSize        = MUSART3RLEN;
  DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;
  DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;
  DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_BYTE;
  DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_BYTE;
  DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;
  DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGHEST;
  DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;
  DMA_Initializes(USART3_Tx_DMA_Channel, &DMA_InitStructure);
  DMA_Channel_Request_Remap(USART3_Tx_DMA_Channel,DMA_REMAP_UART3_TX);
  
  
  
  
  
  /* USART3 RX DMA1 Channel (triggered by USART3 Rx event) Config */
  DMA_Reset(USART3_Rx_DMA_Channel);
  DMA_InitStructure.PeriphAddr = USART3_DR_Base;
  DMA_InitStructure.MemAddr    =  (uint32_t)isUsart3Rbuf.rBuf;
  DMA_InitStructure.Direction  = DMA_DIR_PERIPH_SRC;
  DMA_InitStructure.BufSize    = MUSART3RLEN;
  DMA_Initializes(USART3_Rx_DMA_Channel, &DMA_InitStructure);
  DMA_Channel_Request_Remap(USART3_Rx_DMA_Channel,DMA_REMAP_UART3_RX);
  
}




void  iUsart3RegInit(void)
{
  
  USART_InitType USART_InitStructure;
  USART_Reset(UART3);
  USART_InitStructure.BaudRate            = 4800;
  USART_InitStructure.WordLength          = USART_WL_8B;
  USART_InitStructure.StopBits            = USART_STPB_1_5;
  USART_InitStructure.Parity              = USART_PE_NO;
  USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
  USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
  USART_Initializes(UART3, &USART_InitStructure);
  
  
  /* Enable USART3 DMA Rx and TX request */
  USART_DMA_Transfer_Enable(UART3, USART_DMAREQ_RX | USART_DMAREQ_TX);
  /* Enable USART3 TX DMA Channel */
//  DMA_Channel_Enable(USART3_Tx_DMA_Channel);
  /* Enable USART3 RX DMA Channel */
  DMA_Channel_Enable(USART3_Rx_DMA_Channel);
  
  //串口中断配置
  USART_Interrput_Enable(UART3, USART_INT_IDLEF);
  USART_Interrput_Enable(UART3, USART_INT_TXC);
  
  USART_Enable(UART3);
  
  
}


void iUart3NvicInit(void)
{
  NVIC_InitType NVIC_InitStructure;
  
  NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);
  /* Enable the USARTz Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel                   = UART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Initializes(&NVIC_InitStructure);
}

void   eUsart3DmaInit(void)
{
  iUsart3RccInit();
  iUsart3GpioInit();
  iUsart3DmaInit();
  iUsart3RegInit();
  iUart3NvicInit();
}



void hold3Time(uint32_t i)
{
  i*=5;//1US
  while(i--);
} 



//
void  eUsart3DmaSend(uint8_t *inBuf, uint16_t  len)
{
  hold3Time(10);
  memcpy(isUsart3Rbuf.tBuf ,inBuf,len);
  DMA_Channel_Disable(USART3_Tx_DMA_Channel);                      //关闭DMA传输 
  
  //while (DMA_GetIntStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
  
  DMA_Buffer_Size_Config(USART3_Tx_DMA_Channel,len);          //数据传输量  
  
  DMA_Channel_Enable(USART3_Tx_DMA_Channel);                      //开启DMA传输 
}


void  UART3_IRQHandler(void)
{
  uint16_t len;
  uint16_t data;
  if((USART_Interrupt_Status_Get(UART3,USART_INT_IDLEF)) != RESET)
  {
    
    
    data = UART3->STS;
    data = UART3->DAT;
    data++;
    len=MUSART3RLEN-DMA_Current_Data_Transfer_Number_Get(USART3_Rx_DMA_Channel);
    isUsart3Rbuf.rLen+= len ;
    isUsart3Rbuf.reveceFlag = 1 ;
    
    
  }
  if((USART_Interrupt_Status_Get(UART3,USART_INT_TXC)) != RESET)
  {
    USART_Interrupt_Status_Clear(UART3, USART_INT_TXC);
    
  }
  
}




//读取接收数据长度
uint16_t eUart3ReadDat(uint8_t *outBuf,uint16_t len)
{
  uint16_t  rData;
  if(isUsart3Rbuf.reveceFlag == 1)
  {
    DMA_Channel_Disable(USART3_Rx_DMA_Channel);                      //关闭DMA传输 
    isUsart3Rbuf.reveceFlag = 0;
    if(isUsart3Rbuf.rLen<len)
    {
      memcpy(outBuf,isUsart3Rbuf.rBuf, isUsart3Rbuf.rLen);
      rData = isUsart3Rbuf.rLen;
    }
    else
    {
      memcpy(outBuf,isUsart3Rbuf.rBuf, len);
      rData = len;
    }
    isUsart3Rbuf.rLen= 0;
    //   DMA_ClearFlag(USART3_Rx_DMA_Channel,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清除DMA2_Steam7传输完成标志
    DMA_Buffer_Size_Config(USART3_Rx_DMA_Channel, MUSART3RLEN);
    DMA_Channel_Enable(USART3_Rx_DMA_Channel);     //打开DMA,
  }
  else
  {
    rData= 0;
  }
  return rData ;
}

