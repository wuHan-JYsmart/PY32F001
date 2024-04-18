#ifndef __POWER_BUS_APP_H
#define __POWER_BUS_APP_H
#include"eTypedef.h"

typedef enum
{
rCoil =0x01,//读线圈	
rMreg =0x03, //读多路寄存器
rInputReg =0x04 , //读输入寄存器
wsCoil=0x05, //写单个线圈
wsReg=0x06, //写单路寄存器
wMCoil=0x0f, //写多个线圈
wMReg =0x10, //写多路寄存器
}modbus_key;


//写单个线圈
typedef struct
{
uint16_t  add;
void (*wsCoil)(uint8_t * inBuf);	
}modbus_wsCoil;

typedef struct
{
	uint8_t b0:1;
	uint8_t b1:1;
	uint8_t b2:1;
	uint8_t b3:1;
	uint8_t b4:1;
	uint8_t b5:1;
	uint8_t b6:1;
	uint8_t b7:1;
	
}single_bit;


typedef union  {
single_bit  bit;
uint8_t  coil;// 线圈
}bitOpt;

void  emModbusOpt(void);


#endif

