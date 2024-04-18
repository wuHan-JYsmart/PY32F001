#include"eI2cDriver.h"


void I2C_EE_Init(void)
{
    /** GPIO configuration and clock enable */
    GPIO_InitType GPIO_InitStructure;
    I2C_InitType I2C_InitStructure;
#if PROCESS_MODE == 1 // interrupt
    NVIC_InitType NVIC_InitStructure;
#endif
   
    /** enable peripheral clk*/
    I2Cx_peripheral_clk_en();
    I2C_Reset(I2Cx);

    I2Cx_scl_clk_en();
    I2Cx_sda_clk_en();
    
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.Pin            = I2Cx_SCL_PIN | I2Cx_SDA_PIN;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_SLOW;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_I2C1;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_Peripheral_Initialize(GPIOx, &GPIO_InitStructure);

    /** I2C periphral configuration */
    I2C_Reset(I2Cx);
    I2C_InitStructure.BusMode     = I2C_BUSMODE_I2C;
    I2C_InitStructure.DutyCycle   = I2C_FMDUTYCYCLE_2;
    I2C_InitStructure.OwnAddr1    = 0xff;
    I2C_InitStructure.AckEnable   = I2C_ACKEN;
    I2C_InitStructure.AddrMode    = I2C_ADDR_MODE_7BIT;
    I2C_InitStructure.ClkSpeed    = I2C_Speed;
    I2C_Initializes(I2Cx, &I2C_InitStructure);
#if PROCESS_MODE == 1 /* interrupt */
    /** I2C NVIC configuration */
    NVIC_Priority_Group_Set(NVIC_PER0_SUB4_PRIORITYGROUP);
    NVIC_InitStructure.NVIC_IRQChannel                   = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel                   = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);
#endif
}



void I2C1_EV_IRQHandler(void)
{
}

/**
*\*\name    I2C1_ER_IRQHandler.
*\*\fun     i2c error Interrupt service function.
*\*\param   none
*\*\return  none 
**/
void I2C1_ER_IRQHandler(void)
{
    if(I2C_Flag_Status_Get(I2C1, I2C_FLAG_ACKFAIL))
    {
            I2C_Flag_Status_Clear(I2C1, I2C_FLAG_ACKFAIL);
            I2C_Generate_Stop_Enable(I2C1); /* Send I2C1 STOP Condition. */
    }
}

