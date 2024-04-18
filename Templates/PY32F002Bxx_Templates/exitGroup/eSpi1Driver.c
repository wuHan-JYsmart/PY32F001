#include"eSpi1Driver.h"


#define  MAX_WAIT 0XFFFE

SPI_HandleTypeDef spi1Handle;





void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_SPI1_CLK_ENABLE();

  /*
 * PA0   ------> Reset 
 * PA1   ------> SCL/SCK
 * PA4   ------> Busy
 * PA5   ------> DC/A0
 * PA6   ------> CSN
 * PA7   ------> SDA/MOSI
  */
  /* PA1 SCL/SCK */
  GPIO_InitStruct.Pin       = GPIO_PIN_1;
  if (hspi->Init.CLKPolarity == SPI_POLARITY_LOW)
  {
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  }
  else
  {
    GPIO_InitStruct.Pull = GPIO_PULLUP;
  }
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PA7 SDA/MOSI */
  GPIO_InitStruct.Pin       = GPIO_PIN_7;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = GPIO_PIN_2;
  if (hspi->Init.CLKPolarity == SPI_POLARITY_LOW)
  {
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  }
  else
  {
    GPIO_InitStruct.Pull = GPIO_PULLUP;
  }
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
}


//
void  eSpiDmaSend( uint8_t *inBuf, uint16_t  len)
{
  HAL_SPI_Transmit(&spi1Handle, inBuf, len, HAL_MAX_DELAY);}



void  iSpi1RegInit(void)
{
 spi1Handle.Instance               = SPI1;
  spi1Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  spi1Handle.Init.Direction         = SPI_DIRECTION_1LINE;
  spi1Handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  spi1Handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  spi1Handle.Init.DataSize          = SPI_DATASIZE_8BIT;
  spi1Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  spi1Handle.Init.NSS               = SPI_NSS_SOFT;
  spi1Handle.Init.Mode              = SPI_MODE_MASTER;
  if (HAL_SPI_DeInit(&spi1Handle) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  if (HAL_SPI_Init(&spi1Handle) != HAL_OK)
  {
    APP_ErrorHandler();
  }

}




void  eSpi1DriverInit(void)
{
iSpi1RegInit();

//eSpi1Write();
}

