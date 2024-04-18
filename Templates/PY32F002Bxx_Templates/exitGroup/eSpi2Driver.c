#include"eSpi2Driver.h"


void   iSpi2GpioInit(void)
{
    GPIO_InitType GPIO_InitStructure;   
GPIO_Structure_Initialize(&GPIO_InitStructure);

    /* Configure master pins: NSS, SCK, MISO and MOSI */
    /* Confugure SPI pins as Alternate Function Push Pull */
    GPIO_InitStructure.Pin            = SPI2_MASTER_MOSI_PIN | SPI2_MASTER_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_InitStructure.GPIO_Alternate = SPI2_MASTER_GPIO_ALTERNATE;
    GPIO_Peripheral_Initialize(SPI2_MASTER_GPIO, &GPIO_InitStructure);
    /* If CLKPOL is HIGH, set SPI_SCK GPIO to GPIO_PULL_UP, otherwise set to GPIO_PULL_DOWN */
    GPIO_InitStructure.Pin            =  SPI2_MASTER_CLK_PIN;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_DOWN;
    GPIO_InitStructure.GPIO_Alternate = SPI2_MASTER_GPIO_ALTERNATE;
    GPIO_Peripheral_Initialize(SPI2_MASTER_GPIO, &GPIO_InitStructure);

}

void  iSpi2RegInit(void)
{
SPI_InitType SPI_InitStructure;
    /* SPI_MASTER configuration ------------------------------------------------------*/
    SPI_Initializes_Structure(&SPI_InitStructure);
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_FIRST_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;
    /* It is recommended that the SPI master mode of the C version chips should not exceed 18MHz */
    SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_64;
    SPI_InitStructure.FirstBit      = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly       = 7;
    SPI_Initializes(SPI2_MASTER, &SPI_InitStructure);
	
// SPI_I2S_Interrupts_Enable(SPI2_MASTER, SPI_I2S_INT_TE);
  SPI_NSS_Config(SPI2_MASTER,SPI_NSS_SOFT);
SPI_Set_Nss_Level(SPI2_MASTER,SPI_NSS_HIGH);

     /* Enable SPI_MASTER */
    SPI_ON(SPI2_MASTER);
}





void  eSpi2Write(void)
{
uint8_t  rd;
     SPI_I2S_Data_Transmit(SPI2, 0X55);
	 rd = SPI_I2S_Data_Get(SPI2);
         rd++;
}


void  Spi2NviciInit(void)
{
    NVIC_InitType NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_PER_PRIORITY_0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_SUB_PRIORITY_1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
}

void  eSpi2DriverInit(void)
{
     //   RCC_Pclk2_Config(RCC_HCLK_DIV2);

    /* Enable peripheral clocks --------------------------------------------------*/
    /* spi clock enable */
    RCC_APB2_Peripheral_Clock_Enable(SPI2_MASTER_PERIPH );

    /* GPIO Periph clock enable */
    RCC_AHB_Peripheral_Clock_Enable(SPI2_MASTER_PERIPH_GPIO );
iSpi2GpioInit();
//Spi2NviciInit();
iSpi2RegInit();

}
