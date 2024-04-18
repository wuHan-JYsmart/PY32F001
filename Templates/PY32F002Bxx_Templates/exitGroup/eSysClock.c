#include"eSysClock.h"


void SetSysClockToHSE(void)
{
ErrorStatus HSEStartUpStatus;
#define HSE_Value   (8000000)
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration
     * -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_Reset();

    /* Enable HSE */
    RCC_HSE_Config(RCC_HSE_ENABLE);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_HSE_Stable_Wait();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_Prefetch_Buffer_Enable();

        if (HSE_Value <= 18000000)
        {
            /* Flash 0 wait state */
            FLASH_Latency_Set(FLASH_LATENCY_0);
        }
        else
        {
            /* Flash 1 wait state */
            FLASH_Latency_Set(FLASH_LATENCY_1);
        }

        /* HCLK = SYSCLK */
        RCC_Hclk_Config(RCC_SYSCLK_DIV1);

        /* PCLK2 = HCLK */
        RCC_Pclk2_Config(RCC_HCLK_DIV1);

        /* PCLK1 = HCLK */
        RCC_Pclk1_Config(RCC_HCLK_DIV1);

        /* Select HSE as system clock source */
        RCC_Sysclk_Config(RCC_SYSCLK_SRC_HSE);
       
        /* Wait till HSE is used as system clock source */
        while (RCC_Sysclk_Source_Get() != RCC_CFG_SCLKSTS_HSE)
        {
        }
    }
    else
    {
        /* If HSE fails to start-up, the application will have wrong clock
           configuration. User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1)
        {
        }
    }
}


void  eSysClockInit(void)
{
RCC_ClocksType  RCC_ClockFreq;

//SetSysClockToHSE();
RCC_Clocks_Frequencies_Value_Get(&RCC_ClockFreq);
//SysTick_Config(RCC_ClockFreq.SysclkFreq/ 1000);   /* 1ms */
}


void SysTick_Handler1(void)
	{    
	PCout(13) =~ PCout(13);

}

