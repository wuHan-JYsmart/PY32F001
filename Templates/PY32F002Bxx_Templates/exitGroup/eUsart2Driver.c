#include"eUsart2Driver.h"
#include<string.h>

is_usart2_rbuf  isUsart2Rbuf;


void  iUsart2RccInit(void)
{
  /* DMA clock enable */
  RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA);
  /* Enable GPIO clock */
  RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA );
  RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
  RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_USART2);
}

void  iUsart2GpioInit(void)
{
  
  GPIO_InitType GPIO_InitStructure;
  
  /* Configure USART2 Tx as alternate function push-pull */
  GPIO_InitStructure.Pin			  = USART2_TxPin;
  GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;    
  GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_USART2;
  GPIO_Peripheral_Initialize(USART2_GPIO, &GPIO_InitStructure);
  
  /* Configure USART2 Rx as alternate function push-pull and pull-up */
  GPIO_InitStructure.Pin			  = USART2_RxPin;
  GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;    
  GPIO_InitStructure.GPIO_Alternate = GPIO_AF7_USART2;
  GPIO_Peripheral_Initialize(USART2_GPIO, &GPIO_InitStructure); 
}



void  iUsart2DmaInit(void)
{
  
  DMA_InitType DMA_InitStructure;
  
  /* USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config */
  DMA_Reset(USART2_Tx_DMA_Channel);
  DMA_InitStructure.PeriphAddr     = USART2_DR_Base;
  DMA_InitStructure.MemAddr        = (uint32_t)(uint32_t)isUsart2Rbuf.tBuf;
  DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;
  DMA_InitStructure.BufSize        = MUSART2RLEN;
  DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;
  DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;
  DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_BYTE;
  DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_BYTE;
  DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;
  DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGHEST;
  DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;
  DMA_Initializes(USART2_Tx_DMA_Channel, &DMA_InitStructure);
  DMA_Channel_Request_Remap(USART2_Tx_DMA_Channel,DMA_REMAP_USART2_TX);
  
  
  
  
  
  /* USART2 RX DMA1 Channel (triggered by USART2 Rx event) Config */
  DMA_Reset(USART2_Rx_DMA_Channel);
  DMA_InitStructure.PeriphAddr = USART2_DR_Base;
  DMA_InitStructure.MemAddr    =  (uint32_t)isUsart2Rbuf.rBuf;
  DMA_InitStructure.Direction  = DMA_DIR_PERIPH_SRC;
  DMA_InitStructure.BufSize    = MUSART2RLEN;
  DMA_Initializes(USART2_Rx_DMA_Channel, &DMA_InitStructure);
  DMA_Channel_Request_Remap(USART2_Rx_DMA_Channel,DMA_REMAP_USART2_RX);
  
}




void  iUsart2RegInit(void)
{
  
  USART_InitType USART_InitStructure;
  USART_Reset(USART2);
  USART_InitStructure.BaudRate            = 115200;
  USART_InitStructure.WordLength          = USART_WL_8B;
  USART_InitStructure.StopBits            = USART_STPB_1;
  USART_InitStructure.Parity              = USART_PE_EVEN;
  USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
  USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
  USART_Initializes(USART2, &USART_InitStructure);
  
  
  /* Enable USART2 DMA Rx and TX request */
  USART_DMA_Transfer_Enable(USART2, USART_DMAREQ_RX | USART_DMAREQ_TX);
  /* Enable USART2 TX DMA Channel */
//  DMA_Channel_Enable(USART2_Tx_DMA_Channel);
  /* Enable USART2 RX DMA Channel */
  DMA_Channel_Enable(USART2_Rx_DMA_Channel);
  
  //串口中断配置
  USART_Interrput_Enable(USART2, USART_INT_IDLEF);
  USART_Interrput_Enable(USART2, USART_INT_TXC);
  
  USART_Enable(USART2);
  
  
}


void iUart2NvicInit(void)
{
  NVIC_InitType NVIC_InitStructure;
  
  NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);
  /* Enable the USARTz Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Initializes(&NVIC_InitStructure);
}

void   eUsart2DmaInit(void)
{
  iUsart2RccInit();
  iUsart2GpioInit();
  iUsart2DmaInit();
  iUsart2RegInit();
  iUart2NvicInit();
}



void hold2Time(uint32_t i)
{
  i*=5;//1US
  while(i--);
} 


//
void  eUsart2DmaSend(uint8_t *inBuf, uint16_t  len)
{
  hold2Time(10);
  memcpy(isUsart2Rbuf.tBuf ,inBuf,len);
  DMA_Channel_Disable(USART2_Tx_DMA_Channel);                      //关闭DMA传输 
  
  //while (DMA_GetIntStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
  
  DMA_Buffer_Size_Config(USART2_Tx_DMA_Channel,len);          //数据传输量  
  
  DMA_Channel_Enable(USART2_Tx_DMA_Channel);                      //开启DMA传输 
}


void  USART2_IRQHandler(void)
{
  uint16_t len;
  uint16_t data;
  if((USART_Interrupt_Status_Get(USART2,USART_INT_IDLEF)) != RESET)
  {
    
    
    data = USART2->STS;
    data = USART2->DAT;
    data++;
    len=MUSART2RLEN-DMA_Current_Data_Transfer_Number_Get(USART2_Rx_DMA_Channel);
    isUsart2Rbuf.rLen+= len ;
    isUsart2Rbuf.reveceFlag = 1 ;
    
    
  }
  if((USART_Interrupt_Status_Get(USART2,USART_INT_TXC)) != RESET)
  {
    USART_Interrupt_Status_Clear(USART2, USART_INT_TXC);
    
  }
  
}




//读取接收数据长度
uint16_t eUart2ReadDat(uint8_t *outBuf,uint16_t len)
{
  uint16_t  rData;
  if(isUsart2Rbuf.reveceFlag == 1)
  {
    DMA_Channel_Disable(USART2_Rx_DMA_Channel);                      //关闭DMA传输 
    isUsart2Rbuf.reveceFlag = 0;
    if(isUsart2Rbuf.rLen<len)
    {
      memcpy(outBuf,isUsart2Rbuf.rBuf, isUsart2Rbuf.rLen);
      rData = isUsart2Rbuf.rLen;
    }
    else
    {
      memcpy(outBuf,isUsart2Rbuf.rBuf, len);
      rData = len;
    }
    isUsart2Rbuf.rLen= 0;
    //   DMA_ClearFlag(USART2_Rx_DMA_Channel,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清除DMA2_Steam7传输完成标志
    DMA_Buffer_Size_Config(USART2_Rx_DMA_Channel, MUSART2RLEN);
    DMA_Channel_Enable(USART2_Rx_DMA_Channel);     //打开DMA,
  }
  else
  {
    rData= 0;
  }
  return rData ;
}

