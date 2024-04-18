#ifndef __E_RTC_DRIVER_H
#define __E_RTC_DRIVER_H
#include"eTypedef.h"

#define     RTC_CLK_HSE128      1
#define     RTC_CLK_LSE         2
#define     RTC_CLK_LSI         3

#define     RTC_CLK_FIRST_CONFIG    0
#define     RTC_CLK_LAST_CONFIG     1

void  RtcAutoWakeUpInit(void);
#endif

