//power bus io控制应用
#include "mPowerBusApp.h"
#include <string.h>

#include"eUsart1Driver.h"
#include"eGpioOutput.h"

 void  wsCoil_A01_Fun(uint8_t *inBuf);

#define  mSlaveID  04 //从站地址

//写单个线圈执行
//写单个线圈 05指令
modbus_wsCoil  mWsCoil[]={
  {0 ,wsCoil_A01_Fun}, //启动
};

__no_init  bitOpt  relayFlag;


#define   mwsCoilAckLen  128 //读线圈最大长度
uint8_t swsCoilAckBuf[mwsCoilAckLen];


  uint16_t CRC16_Modbus(uint8_t *ptr,uint8_t  BufferLength)	
{
  uint16_t cCRC =0xFFFF;
  int i;
  while(BufferLength-- >0)
  {
    cCRC = cCRC ^ *ptr;;
    for(i=0;i<8;i++)
    {	
      if(cCRC&1) cCRC= ((cCRC>>1)^0xA001);
      else cCRC >>= 1;
    }
    ptr++;
  }
  return cCRC;	 
}



//设置线圈状态
void  imSetCoilData(uint8_t add , uint8_t d)
{

switch(add)
{
case 0:
relayFlag.bit.b0 =d;
break;
case 1:
relayFlag.bit.b1 =d;
break;
case 2:
relayFlag.bit.b2 =d;
break;
case 3:
relayFlag.bit.b3 =d;
break;
case 4:
relayFlag.bit.b4 =d;
break;
case 5:
relayFlag.bit.b5 =d;
break;
case 6:
relayFlag.bit.b6 =d;
break;
case 7:
relayFlag.bit.b7 =d;
break;
		}
}


uint8_t imGetCoilData(uint8_t add )
{
uint8_t  rD =0;
switch(add)
{
case 0:
rD = relayFlag.bit.b0 ;
break;
case 1:
rD = relayFlag.bit.b1 ;
break;
case 2:
rD = relayFlag.bit.b2 ;
break;
case 3:
rD = relayFlag.bit.b3 ;
break;
case 4:
rD = relayFlag.bit.b4 ;
break;
case 5:
rD = relayFlag.bit.b5 ;
break;
case 6:
rD = relayFlag.bit.b6 ;
break;
case 7:
rD = relayFlag.bit.b7 ;
break;
		}
return rD;
}





void  wsCoil_A01_Fun(uint8_t *inBuf)
{
  //	uint8_t ackBuf[1]={8};
  uint16_t  setData = inBuf[2]<<8|inBuf[3];
  
  if(setData !=0)
  {
    emRelaySetOnOff(1,TRUE);
	imSetCoilData(0,1);
  }
  else
  {
    emRelaySetOnOff(1,FALSE);
		imSetCoilData(0,0);

  }
  
}


//写单个线圈
uint8_t  imWSCoilFun(uint16_t  add, uint8_t *inBuf)
{
  uint16_t  num = sizeof(mWsCoil)/sizeof(mWsCoil[0]);
  uint16_t  i;
  for(i =0;i!= num; i++)
  {
    if(mWsCoil[i].add == add)
    {
      mWsCoil[i].wsCoil(inBuf);
      break;
    }
    
  }
  
  if(i ==num)
  {
    i++;
	return 0;
  }
  return 1;
}


void  imwsCoilSend(uint8_t len)
{
  eUsart1DmaSend(swsCoilAckBuf, len);
//  eUart1SendDats(swsCoilAckBuf, len);
  
}



//modbus协议下的数据应答
void  imwsCoilAck(uint8_t  byteCount, uint8_t *inBuf)
{
  uint8_t  offLen=3;
  uint16_t crc;
  swsCoilAckBuf[0] =mSlaveID;
  swsCoilAckBuf[1] = (uint8_t)wsCoil;
  memcpy(&swsCoilAckBuf[2],inBuf,4);
  offLen=6;
  crc = CRC16_Modbus(swsCoilAckBuf,offLen);
  swsCoilAckBuf[offLen++] = crc;
  swsCoilAckBuf[offLen++] = crc>>8;
  imwsCoilSend(offLen);
  
  
  
}



//modbus协议下的数据应答
void  imCoilAck(uint8_t  byteCount, uint8_t *inBuf)
{
  uint8_t  offLen=3;
  uint16_t crc;
  swsCoilAckBuf[0] =mSlaveID;
  swsCoilAckBuf[1] = (uint8_t)rCoil;
  swsCoilAckBuf[2] = byteCount;
  memcpy(&swsCoilAckBuf[3],inBuf,byteCount);
  offLen+=byteCount;
  crc = CRC16_Modbus(swsCoilAckBuf,offLen);
    swsCoilAckBuf[offLen++] = crc;
  swsCoilAckBuf[offLen++] = crc>>8;
  imwsCoilSend(offLen);
}





//写单个线圈
void  iW05Reg(uint16_t  add, uint8_t *inBuf)
{
uint8_t  rd;
	rd = imWSCoilFun(add ,inBuf);
	if(rd ==1)
		{
	 imwsCoilAck(1,inBuf);
		}
}




//修改线圈状态
uint8_t imChangeCoilData(uint8_t add , uint8_t d ,uint8_t oldData)
{
 bitOpt  relayFlag;
 relayFlag.coil  = oldData;

switch(add)
{
case 0:
relayFlag.bit.b0 =d;
break;
case 1:
relayFlag.bit.b1 =d;
break;
case 2:
relayFlag.bit.b2 =d;
break;
case 3:
relayFlag.bit.b3 =d;
break;
case 4:
relayFlag.bit.b4 =d;
break;
case 5:
relayFlag.bit.b5 =d;
break;
case 6:
relayFlag.bit.b6 =d;
break;
case 7:
relayFlag.bit.b7 =d;
break;
		}
return relayFlag.coil;
}




//读线圈状态起始地址和数据获取长度
uint8_t imGetRelayInf(uint8_t  startAdd ,uint8_t  num)
{
uint8_t  cAdd =startAdd;
uint8_t  rData =0;
uint8_t  bitAdd =0;
uint8_t  bit;
	for(;cAdd !=(startAdd+num);cAdd++)
		{
		bit = imGetCoilData(cAdd);
		rData =imChangeCoilData(bitAdd,bit, rData);
		bitAdd++;
		}
	
return rData;
}



//读单个线圈
void  coil_A00_Fun(uint8_t *inBuf)
{
	uint8_t  startAdd = inBuf[0]<<8|inBuf[1];
	uint8_t  len = inBuf[2]<<8|inBuf[3];
	uint8_t  ackBuf[1] ={0};
	ackBuf[0] =imGetRelayInf(startAdd,len);
	imCoilAck(1,ackBuf);
}



void  emModbusOpt(void)
{
  uint8_t  rBuf[64];
  uint16_t rLen;
  uint16_t crcData,xCrcData;
  modbus_key  key;
  uint16_t  add;
  
  
rLen=  eUart1ReadDat(rBuf,64);
  if(rLen !=0)
  {
    crcData = CRC16_Modbus(rBuf,rLen-2);
    xCrcData = rBuf[rLen-1]<<8|rBuf[rLen-2];
    if(rBuf[0]!= mSlaveID && rBuf[0] != 255)
      return ;
    key = (modbus_key)rBuf[1];
    add = rBuf[2]<<8|rBuf[3];
    
    //CRC校验
    if(crcData !=xCrcData)
      return ;
    
    switch(key)
    {
    case  wsCoil:
      iW05Reg(add,&rBuf[2]);

	  break;
case  rCoil:
  coil_A00_Fun(&rBuf[2]);
	break;
    }
  }
}

