#include "eTim1Capture.h"

uint16_t  capData =0;

static void iTim1CaptureGpioInit(void)
{
  LL_TIM_IC_InitTypeDef InputCaptureInit ={0};
  LL_GPIO_InitTypeDef TIM1CH1MapInit= {0};

  /* Configure PA0 as input capture channel */
  TIM1CH1MapInit.Pin        = LL_GPIO_PIN_0;
  TIM1CH1MapInit.Mode       = LL_GPIO_MODE_ALTERNATE;
  TIM1CH1MapInit.Alternate  = LL_GPIO_AF_2;

  LL_GPIO_Init(GPIOA,&TIM1CH1MapInit);

  /* Configure the capture channel using the parameters defined by InputCaptureInit structure */
  InputCaptureInit.ICActiveInput  = LL_TIM_ACTIVEINPUT_DIRECTTI;
  InputCaptureInit.ICPrescaler    = LL_TIM_ICPSC_DIV2;
  InputCaptureInit.ICPolarity     = LL_TIM_IC_POLARITY_FALLING;
  InputCaptureInit.ICFilter       = LL_TIM_IC_FILTER_FDIV1_N8;

  LL_TIM_IC_Init(TIM1,LL_TIM_CHANNEL_CH1,&InputCaptureInit);

  /* Enable input capture channel 1 interrupt */
  LL_TIM_EnableIT_CC1(TIM1);

  /* Enable TIM1 interrupt request */
  NVIC_EnableIRQ(TIM1_CC_IRQn);
  NVIC_SetPriority(TIM1_CC_IRQn,0);
}


static void iTim1CaptureRegInit(void)
{
  /* Configure TIM1 using the parameters defined by TIM1CountInit structure */
  LL_TIM_InitTypeDef TIM1CountInit = {0};

  TIM1CountInit.ClockDivision       = LL_TIM_CLOCKDIVISION_DIV1;/* divider:tDTS=tCK_INT   */
  TIM1CountInit.CounterMode         = LL_TIM_COUNTERMODE_CENTER_UP;    /* count mode：up count   */
  TIM1CountInit.Prescaler           =1-1;                   /* clock prescaler：8000  */
  TIM1CountInit.Autoreload          =  60000-1;                   /* autoreload value：1000 */
  TIM1CountInit.RepetitionCounter   =  0;                        /* recount：0             */

  /* Initialize TIM1 */
  LL_TIM_Init(TIM1,&TIM1CountInit);

  /* Enable TIM1 */
  LL_TIM_EnableCounter(TIM1);
}


void  eTim1CaptureInit(void)
{
  /* Enable TIM1 peripheral clock */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM1);

  /* Enabel GPIOA clock */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  iTim1CaptureGpioInit();
  iTim1CaptureRegInit();

//  /* Clear the update interrupt flag (UIF).  */
//  LL_TIM_ClearFlag_UPDATE(TIM1);
//
//  /*Enable update interrupt (UIE) */
//  LL_TIM_EnableIT_UPDATE(TIM1);
//
//  /*Enable TIM1 */
//  LL_TIM_EnableCounter(TIM1);
//
//  /*Enable UPDATE interrupt */
//  NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
//  NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,0);
 
}


void APP_InputcaptureCallback(void)
{
LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_1);
}

void TIM1_CC_IRQHandler(void)
{
  if(LL_TIM_IsEnabledIT_CC1(TIM1) == 1)
  {
    LL_TIM_ClearFlag_CC1(TIM1);
    capData =LL_TIM_IC_GetCaptureCH1(TIM1);
	 LL_TIM_SetCounter(TIM1,0);    // 清零计数值
//    APP_InputcaptureCallback();
  }
}


void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  if((LL_TIM_ReadReg(TIM1,SR) & LL_TIM_SR_UIF) == 1 && LL_TIM_IsEnabledIT_UPDATE(TIM1))
  {
    LL_TIM_ClearFlag_UPDATE(TIM1);
    
LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_1);
  }
}
