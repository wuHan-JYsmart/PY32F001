#include"ePwm1Driver.h"

TIM_HandleTypeDef    Tim1Handle;
TIM_OC_InitTypeDef   t1Sconfig;


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
if(htim->Instance==TIM1)
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
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    /* Interrupt Configuration */
  NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
  NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,0);
}
if(htim->Instance == TIM14)
{
__HAL_RCC_TIM14_CLK_ENABLE();
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(TIM14_IRQn, 3, 0);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIM14_IRQn);
}

  
}

static void APP_TimConfig(void)
{
  /* TIM1 */
  Tim1Handle.Instance = TIM1;                                                  
  
  /* Period = 2000 - 1  */
  Tim1Handle.Init.Period            = 20-1;    //×°ÔØÖµ                                 

  /* Prescaler = 1200 - 1 */
  Tim1Handle.Init.Prescaler         =1;                                 

  /* ClockDivision = 0  */
  Tim1Handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;                  

  /* Counter direction = Up */
  Tim1Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;                      

  /* Repetition = 0 */
  Tim1Handle.Init.RepetitionCounter = 1 - 1;                                   

  /* Auto-reload register not buffered  */
  Tim1Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;          

  if (HAL_TIM_Base_Init(&Tim1Handle) != HAL_OK)                                
  {
    APP_ErrorHandler();
  }

}


static void APP_TimPwmConfig(void)
{
  /* Set output compare mode: PWM1 */
  t1Sconfig.OCMode       = TIM_OCMODE_PWM1;                                     

  /* OC channel output high level effective */
  t1Sconfig.OCPolarity   = TIM_OCPOLARITY_HIGH;                                 

  /* Disable OC FastMode */
  t1Sconfig.OCFastMode   = TIM_OCFAST_DISABLE;                                  

  /* OCN channel output high level effective */
  t1Sconfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;                                

  /* Idle state OC1N output low level */
  t1Sconfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;                              

  /* Idle state OC1 output low level*/
  t1Sconfig.OCIdleState  = TIM_OCIDLESTATE_RESET;                               




  t1Sconfig.Pulse = 0;                                              

  /* Channel 3 configuration */
  if (HAL_TIM_PWM_ConfigChannel(&Tim1Handle, &t1Sconfig, TIM_CHANNEL_3) != HAL_OK)
  {
    APP_ErrorHandler();
  }

}

void  Time1PwmInit(void)
{
APP_TimConfig();
APP_TimPwmConfig();
HAL_TIM_PWM_Start(&Tim1Handle, TIM_CHANNEL_3); 
eTim1PwmCh3Set(5);
  /* TIM Start and Enable IT */
// HAL_TIM_Base_Start_IT(&Tim1Handle);

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
ePwmSetChange();
eButtonThreadRun();
}


void  eTim1PwmCh3Set(uint16_t pwm)
{
if(pwm != 0)
{
__HAL_TIM_SET_COMPARE(&Tim1Handle,TIM_CHANNEL_3,pwm);
//LL_TIM_OC_SetCompareCH3(pwm);
}else
{
//HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_3); 

}
__HAL_TIM_SET_COMPARE(&Tim1Handle,TIM_CHANNEL_3,pwm);

}

