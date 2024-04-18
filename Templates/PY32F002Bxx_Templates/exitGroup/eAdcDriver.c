#include"eAdcDriver.h"

ADC_HandleTypeDef             hadc;
ADC_ChannelConfTypeDef        sConfig;
volatile uint16_t             aADCxConvertedData;



static void iAdcGpioInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();                          /* Enable GPIOA clock */

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;            /* Push-pull output */
  GPIO_InitStruct.Pull = GPIO_NOPULL;                    /* Enable pull-up */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;          /* GPIO speed */  
  /* GPIO initialization */
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);                
}

/*
对PB0 --ADC7/PB1-ADC1 进行初始化
*/
 void eAdcRegInit(void)
{
iAdcGpioInit();
  __HAL_RCC_ADC_CLK_ENABLE();                                                /* Enable ADC clock */

  hadc.Instance = ADC1;
  
  hadc.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV32;               /* Set ADC clock*/
  hadc.Init.Resolution            = ADC_RESOLUTION_12B;                      /* 12-bit resolution for converted data */
  hadc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;                     /* Right-alignment for converted data */
  hadc.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;              /* Scan sequence direction: forward */
  hadc.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;                     /* Single Conversion */
  hadc.Init.LowPowerAutoWait      = DISABLE;                                 /* Auto-delayed conversion feature disabled */
  hadc.Init.ContinuousConvMode    = DISABLE;                                 /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  hadc.Init.DiscontinuousConvMode = DISABLE;                                 /* Disable discontinuous mode */
  hadc.Init.ExternalTrigConv      = ADC_SOFTWARE_START;                      /* Software start to trig the 1st conversion manually, without external event */
  hadc.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;           /* Parameter discarded because software trigger chosen */
  hadc.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;                /* DR register is overwritten with the last conversion result in case of overrun */
  hadc.Init.SamplingTimeCommon    = ADC_SAMPLETIME_41CYCLES_5;               /* The channel sampling time is 41.5 ADC clock cycles */
  if (HAL_ADC_Init(&hadc) != HAL_OK)                                         /* ADC initialization */
  {
    APP_ErrorHandler();
  }

  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;                             
  sConfig.Channel      = ADC_CHANNEL_0;                                      
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)                       /* Configure ADC Channel */
  {
    APP_ErrorHandler();
  }
    
  if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)                           /* ADC Calibration */
  {
    APP_ErrorHandler();
  }
}


uint16_t eAdcGetData(uint8_t  ch)
{

  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;                             
  sConfig.Channel      = ch;                                      
  HAL_ADC_ConfigChannel(&hadc, &sConfig);                       /* Configure ADC Channel */
    /* ADC Start */
    HAL_ADC_Start(&hadc);  
    
    /* Polling for ADC Conversion */
    HAL_ADC_PollForConversion(&hadc, 1000000);        
    
    /* Get ADC Value */
    aADCxConvertedData = HAL_ADC_GetValue(&hadc);   


	HAL_ADC_Stop(&hadc);
	return aADCxConvertedData;
}
