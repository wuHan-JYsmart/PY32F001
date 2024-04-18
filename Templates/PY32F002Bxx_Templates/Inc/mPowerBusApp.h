#ifndef __POWER_BUS_APP_H
#define __POWER_BUS_APP_H
#include"eTypedef.h"

typedef enum
{
rCoil =0x01,//����Ȧ	
rMreg =0x03, //����·�Ĵ���
rInputReg =0x04 , //������Ĵ���
wsCoil=0x05, //д������Ȧ
wsReg=0x06, //д��·�Ĵ���
wMCoil=0x0f, //д�����Ȧ
wMReg =0x10, //д��·�Ĵ���
}modbus_key;


//д������Ȧ
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
uint8_t  coil;// ��Ȧ
}bitOpt;

void  emModbusOpt(void);


#endif

