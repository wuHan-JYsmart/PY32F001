#include"eGpioNvic.h"
#include"eGpioOutput.h"



void eGpioNvicInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  __HAL_RCC_GPIOB_CLK_ENABLE();                  /* Enable GPIOA clock */
  GPIO_InitStruct.Mode  = GPIO_MODE_IT_FALLING;  /* GPIO mode is a falling edge interrupt */
  GPIO_InitStruct.Pull  = GPIO_PULLUP;           /* pull up */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  /* The speed is high */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Enable EXTI interrupt */
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  /* Configure interrupt priority */
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);

}

void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

