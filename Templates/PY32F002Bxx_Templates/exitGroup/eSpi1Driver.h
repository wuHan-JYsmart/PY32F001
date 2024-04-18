#ifndef __E_SPI1_DRIVER_H
#define __E_SPI1_DRIVER_H
#include"eTypedef.h"


void  eSpi1DriverInit(void);
void  eSpiDmaSend(uint8_t *inBuf, uint16_t  len);
#endif

