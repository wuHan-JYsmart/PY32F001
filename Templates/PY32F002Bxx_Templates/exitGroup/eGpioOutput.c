#include"eGpioOutput.h"

void  eGpioOutputInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOC_CLK_ENABLE();                          /* Enable GPIOA clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();                          /* Enable GPIOA clock */

  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;            /* Push-pull output */
  GPIO_InitStruct.Pull = GPIO_PULLUP;                    /* Enable pull-up */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;          /* GPIO speed */  
  /* GPIO initialization */
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  
  GPIO_InitStruct.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  

//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

}



//继电器控制IO  为真表示打开
void  emRelaySetOnOff(uint8_t  ch ,bool  enFlag)
{
	switch(ch)
		{
		case 1:
			if(enFlag == TRUE)
				{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
				}
			else
				{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

				}
			break;

		}
}

