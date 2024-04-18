#ifndef __E_I2C_DRIVER_H
#define __E_I2C_DRIVER_H
#include"eTypedef.h"

#define I2Cx I2C1
#define I2Cx_SCL_PIN GPIO_PIN_6
#define I2Cx_SDA_PIN GPIO_PIN_7
#define GPIOx        GPIOB
#define PROCESS_MODE 0


#define I2Cx_peripheral_clk_en() RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_I2C1)
#define I2Cx_scl_clk_en()        RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB)
#define I2Cx_sda_clk_en()        RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB)

#define I2C_Speed        400000

#endif

