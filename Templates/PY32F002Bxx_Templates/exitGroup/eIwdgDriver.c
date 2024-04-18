#include"eIwdgDriver.h"


void  IwdgResetInfCheck(void)
{
    /* Debug mode IWDG stopped */
  //  DBG_Peripheral_ON(DBG_IWDG_STOP);

    /* Check if the system has resumed from IWDG reset */
    if(RCC_Flag_Status_Get(RCC_FLAG_IWDGRST) != RESET)
    {
        /* IWDGRST flag set */

        
        /* Clear reset flags */
        RCC_Reset_Flag_Clear();
    }
}

void  IwdgInit(void)
{
uint32_t LsiFreq     = 40000;
    IWDG_Write_Protection_Disable();
    /* IWDG counter clock: LSI/32 */
    IWDG_Prescaler_Division_Set(IWDG_CONFIG_PRESCALER_DIV256);
    /* Set counter reload value to obtain 250ms IWDG TimeOut.
       Counter Reload Value = 250ms/IWDG counter clock period
                            = 250ms / (LSI/32)
                            = 0.25s / (LsiFreq/32)
                            = LsiFreq/(32 * 4)
                            = LsiFreq/128 */
    IWDG_Counter_Reload(LsiFreq );
    /* Reload IWDG counter */
    IWDG_Key_Reload();
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
	
}
