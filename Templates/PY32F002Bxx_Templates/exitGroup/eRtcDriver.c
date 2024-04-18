#include"eRtcDriver.h"

uint32_t SynchPrediv, AsynchPrediv;


void RTC_CLKSource_Config(uint8_t ClkSrc, uint8_t FirstLastCfg)
{
      /* Enable the PWR clock */
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_PWR);
    RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_AFIO);
    /* Allow access to RTC */
    PWR_RTC_Backup_Access_Enable();
    
    RCC_Backup_Reset();
    
    /* Disable RTC clock */
    RCC_RTC_Clock_Disable();
        if (ClkSrc == RTC_CLK_HSE128)
    {
        if (FirstLastCfg == RTC_CLK_FIRST_CONFIG)
        {
            /* Enable HSE */
            RCC_LSI_Disable();
            RCC_HSE_Config(RCC_HSE_ENABLE);
            while (RCC_HSE_Stable_Wait() == ERROR)
            {
            }
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_HSE_DIV128);
        }
        else
        {
            RCC_LSI_Disable();
            RCC_RTC_Clock_Config(RCC_RTCCLK_SRC_HSE_DIV128);

            /* Enable HSE */
            RCC_HSE_Config(RCC_HSE_ENABLE);

            while (RCC_HSE_Stable_Wait() == ERROR)
            {
            }
        }

        SynchPrediv  = 0x1E8; // 8M/128 = 62.5KHz
        AsynchPrediv = 0x7F;  // value range: 0-7F
    }
        /* Enable the RTC Clock */
    RCC_RTC_Clock_Enable();
    RTC_Wait_For_Synchronization();
    
}

static void RTC_Prescaler_Config(RTC_InitType *RTC_InitStruct)
{
    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStruct->RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStruct->RTC_SynchPrediv  = SynchPrediv;
    RTC_InitStruct->RTC_HourFormat   = RTC_24HOUR_FORMAT;
}

ErrorStatus RTC_Date_Regulate(RTC_DateType* RTC_DateStruct)
{
// WeekDay (01-07)
      RTC_DateStruct->WeekDay = 1;
//(01-31) 
	   RTC_DateStruct->Date = 22;
//Month (01-12)
 RTC_DateStruct->Month =7;
//Year (00-99)
RTC_DateStruct->Year=23;
return SUCCESS;
}


ErrorStatus RTC_Time_Regulate(RTC_TimeType* RTC_TimeStruct)
{
  RTC_TimeStruct->Hours = 21;
   RTC_TimeStruct->Minutes =25;
    RTC_TimeStruct->Seconds =40;
   return SUCCESS;
}

void RTC_Date_Show(void)
{
  RTC_DateType RTC_DateStructure;

    /* Get the current Date */
    RTC_Date_Get(RTC_FORMAT_BIN, &RTC_DateStructure);
}

void RTC_Time_Show(void)
{
RTC_TimeType RTC_TimeStructure;
    RTC_Time_Get(RTC_FORMAT_BIN, &RTC_TimeStructure);
	 (void)RTC->DATE;
}

ErrorStatus RTC_Calendar_Config(FunctionalState delay_cmd)
{
  RTC_TimeType RTC_TimeStructure;
//RTC_TimeType RTC_TimeDefault;
RTC_InitType RTC_InitStructure;
  RTC_DateType RTC_DateStructure;
    /* RTC prescaler regulate */
    RTC_Prescaler_Config(&RTC_InitStructure);
    /* RTC date time and alarm regulate */
    RTC_Date_Regulate(&RTC_DateStructure);
    RTC_Time_Regulate(&RTC_TimeStructure);

    /* Initializes RTC calendar */
    if (RTC_Calendar_Initializes(RTC_FORMAT_BIN, &RTC_InitStructure, &RTC_DateStructure, &RTC_TimeStructure, delay_cmd) == ERROR)
    {
       
        return ERROR;
    }
    else{

        RTC_Date_Show();
        RTC_Time_Show();
        return SUCCESS;
    }
	    /* Enable the RTC Clock */   
//		RCC_RTC_Clock_Enable();   
//		RTC_Wait_For_Synchronization();
}


void EXTI19_RTCWKUP_Configuration(void)
{
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Configure Button EXTI line */
    EXTI_Interrupt_Status_Clear(EXTI_LINE19);
    EXTI_InitStructure.EXTI_Line    = EXTI_LINE19;
#ifdef __TEST_SEVONPEND_WFE_NVIC_DIS__
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
#else
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
#endif
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Peripheral_Initializes(&EXTI_InitStructure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel                   = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}


void  RtcAutoWakeUpInit(void)
{
    /* RTC clock source select */    
RTC_CLKSource_Config(RTC_CLK_HSE128, RTC_CLK_FIRST_CONFIG);
	    /* RTC calendar regulate */   
		RTC_Calendar_Config(DISABLE);
		 RTC_WakeUp_Clock_Select(RTC_WKUPCLK_CK_SPRE_16BITS);
		    RTC_WakeUp_Counter_Set(1);
			EXTI19_RTCWKUP_Configuration();
			        RTC_Interrupts_Enable(RTC_INT_WUT);
        RTC_WakeUp_Enable();
}


void RTC_WKUP_IRQHandler(void)
{
    EXTI_Interrupt_Status_Clear(EXTI_LINE19);
    if (RTC_Interrupt_Status_Get(RTC_INT_WUT) != RESET)
    {
        RTC_Time_Show();
        RTC_Interrupt_Status_Clear(RTC_INT_WUT);
    }
}