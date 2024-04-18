#ifndef __E_SPI22_DRIVER_H
#define __E_SPI2_DRIVER_H
#include"eTypedef.h"


#define SPI2_MASTER                SPI2
#define SPI2_MASTER_PERIPH         RCC_APB2_PERIPH_SPI2
#define SPI2_MASTER_PERIPH_GPIO    RCC_AHB_PERIPH_GPIOB
#define SPI2_MASTER_GPIO           GPIOB
#define SPI2_MASTER_GPIO_ALTERNATE GPIO_AF1_SPI2
#define SPI2_MASTER_MISO_PIN       GPIO_PIN_14
#define SPI2_MASTER_MOSI_PIN       GPIO_PIN_15
#define SPI2_MASTER_CLK_PIN        GPIO_PIN_13
#define SPI2_MASTER_NSS_PIN        GPIO_PIN_12

void  eSpi2DriverInit(void);

#endif

