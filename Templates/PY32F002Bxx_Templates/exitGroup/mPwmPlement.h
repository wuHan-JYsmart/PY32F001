#ifndef __M_PWM_PLEMENT_H
#define __M_PWM_PLEMENT_H
#include"eTypedef.h"

#define TIMx                           TIM1
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM1_CLK_ENABLE()
#define TIMx_IRQn                      TIM1_BRK_UP_TRG_COM_IRQn


#define  PERIOD_VALUE       (uint32_t)(700 - 1)  /* Period Value  */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE/2)        /* Capture Compare 1 Value  */
#define  PULSE2_VALUE       (uint32_t)(PERIOD_VALUE*37.5/100) /* Capture Compare 2 Value  */
#define  PULSE3_VALUE       (uint32_t)(PERIOD_VALUE/4)        /* Capture Compare 3 Value  */
#define  PULSE4_VALUE       (uint32_t)(PERIOD_VALUE*12.5/100) /* Capture Compare 4 Value  */

#endif

