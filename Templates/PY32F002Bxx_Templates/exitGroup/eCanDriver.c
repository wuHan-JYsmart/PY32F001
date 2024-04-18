#include"eCanDriver.h"

CanTxMessage CAN_TxMessage;
CanRxMessage CAN_RxMessage;
uint16_t Rx_Flag = DISABLE;

void CAN_NVIC_Configuration(void)
{
  NVIC_InitType NVIC_InitStructure;
  
  /* NVIC priority group set */
  NVIC_Priority_Group_Set(NVIC_PER2_SUB2_PRIORITYGROUP);
  
  /* NVIC configure */
  NVIC_InitStructure.NVIC_IRQChannel                   = CAN_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x1;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Initializes(&NVIC_InitStructure);
}


void CAN_GPIO_Configuration(void)
{
  GPIO_InitType GPIO_InitStructure;
  
  /* Initializes the GPIO */
  GPIO_Structure_Initialize(&GPIO_InitStructure);
  
  /* configure CAN pin */
  RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB);
  RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
  
  GPIO_InitStructure.Pin            = GPIO_PIN_8;
  GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_CAN;
  GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin             = GPIO_PIN_9;
  GPIO_InitStructure.GPIO_Alternate  = GPIO_AF6_CAN;
  GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
}

void  CanDriverInit(void)
{
  
  
  CAN_InitType        CAN_InitStructure;
  CAN_FilterInitType  CAN_FilterInitStructure;
  
  RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_CAN);
  
  /* Initializes CAN*/
  CAN_NVIC_Configuration();
  CAN_GPIO_Configuration();
  
  CAN_Reset(CAN);
  CAN_Structure_Initializes(&CAN_InitStructure);
  
  /* CAN cell init */
  CAN_InitStructure.TTCM          = DISABLE;
  CAN_InitStructure.ABOM          = DISABLE;
  CAN_InitStructure.AWKUM         = DISABLE;
  CAN_InitStructure.NART          = DISABLE;
  CAN_InitStructure.RFLM          = DISABLE;
  CAN_InitStructure.TXFP          = ENABLE;
  CAN_InitStructure.OperatingMode =CAN_NORMAL_MODE;// CAN_LOOPBACK_MODE; CAN_NORMAL_MODE
  /* Baud rate calculation = CAN clock/(BaudRatePrescaler*(TBS1+TBS2+1)) */
  CAN_InitStructure.RSJW          = CAN_RSJW_1TQ;
  CAN_InitStructure.TBS1          = CAN_TBS1_8TQ;
  CAN_InitStructure.TBS2          = CAN_TBS2_7TQ; 
  CAN_InitStructure.BaudRatePrescaler = 4;
  
  /* Initializes the CAN */
  CAN_Initializes(CAN, &CAN_InitStructure); 
  
  /* CAN filter init */
  CAN_FilterInitStructure.Filter_Num            = 0;
  CAN_FilterInitStructure.Filter_Mode           = CAN_FILTER_IDLISTMODE;
  CAN_FilterInitStructure.Filter_Scale          = CAN_FILTER_32BITSCALE;
  CAN_FilterInitStructure.Filter_HighId         = CAN_STD_ID_LIST_32BIT_H(0x00000400);
  CAN_FilterInitStructure.Filter_LowId          = CAN_STD_ID_LIST_32BIT_L(0x00000400);
  CAN_FilterInitStructure.FilterMask_HighId     = CAN_STD_ID_LIST_32BIT_H(0x00000200);
  CAN_FilterInitStructure.FilterMask_LowId      = CAN_STD_ID_LIST_32BIT_L(0x00000200);
  CAN_FilterInitStructure.Filter_FIFOAssignment = CAN_FIFO0;
  CAN_FilterInitStructure.Filter_Act            = ENABLE;
  CAN_Filter_Initializes(&CAN_FilterInitStructure);
  /* IT Configuration for CAN */  
  CAN_Config_Interrupt_Enable(CAN, CAN_INT_FMP0);
  
  
}

void CAN_RX0_IRQHandler(void)
{
  /* Receives a message */
  CAN_Message_Receive(CAN,CAN_FIFO0, &CAN_RxMessage);
  	  PCout(15) =~ PCout(15);

}




uint8_t Check_CanRecData(CanRxMessage* RxMessage, uint32_t StdId, uint32_t ExtId, uint8_t IDE, uint8_t RTR, uint8_t DLC,
                         uint8_t Data0, uint8_t Data1, uint8_t Data2, uint8_t Data3,
                         uint8_t Data4, uint8_t Data5, uint8_t Data6, uint8_t Data7, uint8_t FMI)
{
  /* Receive */
  
  /* ID */
  if(IDE == CAN_EXTENDED_ID)
  {
    if(RxMessage->ExtId   != ExtId)          
    {
      return Fail;
    }
  }
  else if(IDE == CAN_STANDARD_ID)
  {
    if(RxMessage->StdId   != StdId)  
    {
      return Fail;
    }
  }
  /* **** */
  
  /* IDE/RTR/DLC */
  if( (RxMessage->IDE     != IDE)   ||         /* CAN_ID_STD / CAN_ID_EXT */
     (RxMessage->RTR     != RTR)   ||         /* CAN_RTR_DATA / CAN_RTR_REMOTE */
       (RxMessage->DLC     != DLC)              /* 0 to 8 */
         )
  {
    return Fail;
  }
  /* **** */
  
  /* DATA */
  if(RTR == CAN_RTRQ_DATA)
  {
    if(DLC >= 1)
    {
      if(RxMessage->Data[0] != Data0)
      {
        return Fail;
      }
    }
    if(DLC >= 2)
    {
      if(RxMessage->Data[1] != Data1)
      {
        return Fail;
      }
    }
    if(DLC >= 3)
    {
      if(RxMessage->Data[2] != Data2)
      {
        return Fail;
      }
    }
    if(DLC >= 4)
    {
      if(RxMessage->Data[3] != Data3)
      {
        return Fail;
      }
    }
    if(DLC >= 5)
    {
      if(RxMessage->Data[4] != Data4)
      {
        return Fail;
      }
    }
    if(DLC >= 6)
    {
      if(RxMessage->Data[5] != Data5)
      {
        return Fail;
      }
    }
    if(DLC >= 7)
    {
      if(RxMessage->Data[6] != Data6)
      {
        return Fail;
      }
    }
    if(DLC == 8)
    {
      if(RxMessage->Data[7] != Data7)
      {
        return Fail;
      }
    }
    if(DLC > 8)
    {
      return Fail;
    }
  }
  else if(RTR == CAN_RTRQ_REMOTE)
  {
    
  }
  
  /* FMI */
  if(RxMessage->FMI != FMI)           
  {
    return Fail;
  }
  
  return Pass;
}



void  CanTest(void)
{
  
  uint32_t wait_slak;
  /* Transmit assign */
  CAN_TxMessage.StdId   = 0x0400;        
  CAN_TxMessage.ExtId   = 0x00;        
  CAN_TxMessage.IDE     = CAN_STANDARD_ID;           /* CAN_ID_STD / CAN_ID_EXT */
  CAN_TxMessage.RTR     = CAN_RTRQ_DATA;           /* CAN_RTR_DATA / CAN_RTR_REMOTE */
  CAN_TxMessage.DLC     = 8;           /* 0 to 8 */
  CAN_TxMessage.Data[0] = 0x00;
  CAN_TxMessage.Data[1] = 0x01;
  CAN_TxMessage.Data[2] = 0x02;
  CAN_TxMessage.Data[3] = 0x03;
  CAN_TxMessage.Data[4] = 0x04;
  CAN_TxMessage.Data[5] = 0x05;
  CAN_TxMessage.Data[6] = 0x06;
  CAN_TxMessage.Data[7] = 0x07;
 // CAN_Transmit_Message_initializes(CAN,&CAN_TxMessage);
  while(1)
  {
    
    /* Transmit */
    CAN_Transmit_Message_initializes(CAN,&CAN_TxMessage);
 //   while(Rx_Flag == DISABLE)
//    {
//      Rx_Flag = Check_CanRecData(&CAN_RxMessage, 0x0400, 0x00, CAN_STANDARD_ID, CAN_RTRQ_DATA, 8,
//                                 0x00,0x01, 0x02, 0x03, 
//                                 0x04,0x05, 0x06, 0x07, 0);
//	  if(Rx_Flag ==Pass)
//	
//
//    }
    
    /* Delay */
    wait_slak = 0xFFFF;
    while(wait_slak>0)
    {
      wait_slak--;
    }
	  PCout(14) =~ PCout(14);
    Rx_Flag = DISABLE;
  }
}

