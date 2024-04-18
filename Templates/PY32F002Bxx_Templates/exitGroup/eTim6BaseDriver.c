#include"eTim6BaseDriver.h"


TIM_HandleTypeDef    TimHandle14;

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


void  eTime6BaseInit(void)
{
  /* TIM1 */
  TimHandle14.Instance = TIM14;                                                  
  
  /* Period = 3000 - 1 */
  TimHandle14.Init.Period            = 10 - 1;                                     

  /* Prescaler = 8000 - 1 */
  TimHandle14.Init.Prescaler         = 2 - 1;                                 

  /* ClockDivision = 0 */
  TimHandle14.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;                  

  /* Counter direction = Up */
  TimHandle14.Init.CounterMode       = TIM_COUNTERMODE_UP;                      

  /* Repetition = 0 */
  TimHandle14.Init.RepetitionCounter = 1 - 1;                                   

  /* Auto-reload register not buffered */
  TimHandle14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;          

  /* Initialize TIM1 */
  if (HAL_TIM_Base_Init(&TimHandle14) != HAL_OK)                                
  {
    APP_ErrorHandler();
  }
   if (HAL_TIM_Base_Start_IT(&TimHandle14) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
static  int runTim;
runTim++;

}

void TIM14_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle14);
}


