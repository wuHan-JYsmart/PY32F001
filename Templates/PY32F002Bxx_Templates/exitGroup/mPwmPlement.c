//PWM»¥²¹Êä³ö¿ØÖÆ
#include "mPwmPlement.h"
//PWM»¥²¹Êä³ö³õÊ¼»¯

TIM_HandleTypeDef    TimHandle;
TIM_OC_InitTypeDef              sPWMConfig;

void Error_Handler(void)
{
    while (1)
    {
    }
}




void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  /* Enable TIM1 Clock */
  __HAL_RCC_TIM1_CLK_ENABLE();                              
  /* Enable GPIOB Clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();             

  __HAL_RCC_GPIOB_CLK_ENABLE();                             
  /* Mode AF-PP */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                   
  /* PULLUP */
  GPIO_InitStruct.Pull = GPIO_PULLUP;                       
  /* SPEED HIGH */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  /* GPIO PA5 Initialize*/
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    /* Interrupt Configuration */
  NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
  NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,0);
  
}


void  pwmPlementInit(void)
{
uint32_t uwPrescalerValue = 0;
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;

    TimHandle.Instance = TIMx;

    TimHandle.Init.Period            = PERIOD_VALUE;
    TimHandle.Init.Prescaler         = uwPrescalerValue;
    TimHandle.Init.ClockDivision     = 0;
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }


    sPWMConfig.OCMode       = TIM_OCMODE_PWM1;
    sPWMConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sPWMConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sPWMConfig.OCIdleState  = TIM_OCIDLESTATE_SET;
    sPWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    sPWMConfig.OCFastMode   = TIM_OCFAST_DISABLE;

    /* Set the pulse value for channel 1 */
    sPWMConfig.Pulse = PULSE1_VALUE;
    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_1) != HAL_OK)
    {
        /* Configuration Error */
        Error_Handler();
    }

    /* Set the pulse value for channel 2 */
    sPWMConfig.Pulse = PULSE2_VALUE;
    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_2) != HAL_OK)
    {
        /* Configuration Error */
        Error_Handler();
    }

TIM_BreakDeadTimeConfigTypeDef sBreakConfig;
    sBreakConfig.BreakState       = TIM_BREAK_ENABLE;
    sBreakConfig.DeadTime         = 1200;
    sBreakConfig.OffStateRunMode  = TIM_OSSR_ENABLE;
    sBreakConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
    sBreakConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
    sBreakConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
    sBreakConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_ENABLE;
    if(HAL_TIMEx_ConfigBreakDeadTime(&TimHandle, &sBreakConfig) != HAL_OK)
    {
        /* Configuration Error */
        Error_Handler();
    }
	
    if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }
    /* Start channel 1N */
    if(HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }

}
