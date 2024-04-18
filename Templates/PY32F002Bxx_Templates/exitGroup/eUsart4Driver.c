#include"eUsart4Driver.h"
#include<string.h>

is_usart4_rbuf  isUsart4Rbuf;


void  iUsart4RccInit(void)
{
  /* DMA clock enable */
  RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_DMA);
  /* Enable GPIO clock */
  RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB );
  RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
  RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_UART4);
}

void  iUsart4GpioInit(void)
{
  
  GPIO_InitType GPIO_InitStructure;
  
  /* Configure USART4 Tx as alternate function push-pull */
  GPIO_InitStructure.Pin			  = USART4_TxPin;
  GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;    
  GPIO_InitStructure.GPIO_Alternate = GPIO_AF7_UART4;
  GPIO_Peripheral_Initialize(USART4_GPIO, &GPIO_InitStructure);
  
  /* Configure USART4 Rx as alternate function push-pull and pull-up */
  GPIO_InitStructure.Pin			  = USART4_RxPin;
  GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;    
  GPIO_InitStructure.GPIO_Alternate = GPIO_AF7_UART4;
  GPIO_Peripheral_Initialize(USART4_GPIO, &GPIO_InitStructure); 
}



void  iUsart4DmaInit(void)
{
  
  DMA_InitType DMA_InitStructure;
  
  /* USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config */
  DMA_Reset(USART4_Tx_DMA_Channel);
  DMA_InitStructure.PeriphAddr     = USART4_DR_Base;
  DMA_InitStructure.MemAddr        = (uint32_t)(uint32_t)isUsart4Rbuf.tBuf;
  DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;
  DMA_InitStructure.BufSize        = MUSART4RLEN;
  DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_MODE_DISABLE;
  DMA_InitStructure.MemoryInc      = DMA_MEM_INC_MODE_ENABLE;
  DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_WIDTH_BYTE;
  DMA_InitStructure.MemDataSize    = DMA_MEM_DATA_WIDTH_BYTE;
  DMA_InitStructure.CircularMode   = DMA_CIRCULAR_MODE_DISABLE;
  DMA_InitStructure.Priority       = DMA_CH_PRIORITY_HIGHEST;
  DMA_InitStructure.Mem2Mem        = DMA_MEM2MEM_DISABLE;
  DMA_Initializes(USART4_Tx_DMA_Channel, &DMA_InitStructure);
  DMA_Channel_Request_Remap(USART4_Tx_DMA_Channel,DMA_REMAP_UART4_TX);
  
  
  
  
  
  /* USART4 RX DMA1 Channel (triggered by USART4 Rx event) Config */
  DMA_Reset(USART4_Rx_DMA_Channel);
  DMA_InitStructure.PeriphAddr = USART4_DR_Base;
  DMA_InitStructure.MemAddr    =  (uint32_t)isUsart4Rbuf.rBuf;
  DMA_InitStructure.Direction  = DMA_DIR_PERIPH_SRC;
  DMA_InitStructure.BufSize    = MUSART4RLEN;
  DMA_Initializes(USART4_Rx_DMA_Channel, &DMA_InitStructure);
  DMA_Channel_Request_Remap(USART4_Rx_DMA_Channel,DMA_REMAP_UART4_RX);
  
}




void  iUsart4RegInit(void)
{
  
  USART_InitType USART_InitStructure;
  USART_Reset(UART4);
  USART_InitStructure.BaudRate            = 115200;
  USART_InitStructure.WordLength          = USART_WL_8B;
  USART_InitStructure.StopBits            = USART_STPB_1;
  USART_InitStructure.Parity              = USART_PE_NO;
  USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
  USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
  USART_Initializes(UART4, &USART_InitStructure);
  
  
  /* Enable USART4 DMA Rx and TX request */
  USART_DMA_Transfer_Enable(UART4, USART_DMAREQ_RX | USART_DMAREQ_TX);
  /* Enable USART4 TX DMA Channel */
//  DMA_Channel_Enable(USART4_Tx_DMA_Channel);
  /* Enable USART4 RX DMA Channel */
  DMA_Channel_Enable(USART4_Rx_DMA_Channel);
  
  //串口中断配置
  USART_Interrput_Enable(UART4, USART_INT_IDLEF);
  USART_Interrput_Enable(UART4, USART_INT_TXC);
  
  USART_Enable(UART4);
  
  
}


void iUart4NvicInit(void)
{
  NVIC_InitType NVIC_InitStructure;
  
  NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);
  /* Enable the USARTz Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel                   = UART4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Initializes(&NVIC_InitStructure);
}

void   eUsart4DmaInit(void)
{
  iUsart4RccInit();
  iUsart4GpioInit();
  iUsart4DmaInit();
  iUsart4RegInit();
  iUart4NvicInit();
}



void hold4Time(uint32_t i)
{
  i*=5;//1US
  while(i--);
} 



//
void  eUsart4DmaSend(uint8_t *inBuf, uint16_t  len)
{
  hold4Time(10);
  memcpy(isUsart4Rbuf.tBuf ,inBuf,len);
  isUsart4Rbuf.txOver =0;
  DMA_Channel_Disable(USART4_Tx_DMA_Channel);                      //关闭DMA传输 
  
  //while (DMA_GetIntStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
  
  DMA_Buffer_Size_Config(USART4_Tx_DMA_Channel,len);          //数据传输量  
  
  DMA_Channel_Enable(USART4_Tx_DMA_Channel);                      //开启DMA传输 
  while(isUsart4Rbuf.txOver ==0);
}


void  UART4_IRQHandler(void)
{
  uint16_t len;
  uint16_t data;
  if((USART_Interrupt_Status_Get(UART4,USART_INT_IDLEF)) != RESET)
  {
    
    
    data = UART4->STS;
    data = UART4->DAT;
    data++;
    len=MUSART4RLEN-DMA_Current_Data_Transfer_Number_Get(USART4_Rx_DMA_Channel);
    isUsart4Rbuf.rLen+= len ;
    isUsart4Rbuf.reveceFlag = 1 ;
    
    
  }
  if((USART_Interrupt_Status_Get(UART4,USART_INT_TXC)) != RESET)
  {
    USART_Interrupt_Status_Clear(UART4, USART_INT_TXC);
	isUsart4Rbuf.txOver =1;
    
  }
  
}




//读取接收数据长度
uint16_t eUart4ReadDat(uint8_t *outBuf,uint16_t len)
{
  uint16_t  rData;
  if(isUsart4Rbuf.reveceFlag == 1)
  {
    DMA_Channel_Disable(USART4_Rx_DMA_Channel);                      //关闭DMA传输 
    isUsart4Rbuf.reveceFlag = 0;
    if(isUsart4Rbuf.rLen<len)
    {
      memcpy(outBuf,isUsart4Rbuf.rBuf, isUsart4Rbuf.rLen);
      rData = isUsart4Rbuf.rLen;
    }
    else
    {
      memcpy(outBuf,isUsart4Rbuf.rBuf, len);
      rData = len;
    }
    isUsart4Rbuf.rLen= 0;
    //   DMA_ClearFlag(USART4_Rx_DMA_Channel,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清除DMA2_Steam7传输完成标志
    DMA_Buffer_Size_Config(USART4_Rx_DMA_Channel, MUSART4RLEN);
    DMA_Channel_Enable(USART4_Rx_DMA_Channel);     //打开DMA,
  }
  else
  {
    rData= 0;
  }
  return rData ;
}

