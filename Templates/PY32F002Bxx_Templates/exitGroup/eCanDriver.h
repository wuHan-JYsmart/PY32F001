#ifndef __eCAN_DRIVER_H
#define __eCAN_DRIVER_H
#include"eTypedef.h"


typedef enum
{
    Fail   = (uint8_t)0,
    Pass   = (uint8_t)1
} Slave_Return;


#define  CAN_STD_ID_LIST_32BIT_H(STDID)       ((uint16_t) (((uint32_t)STDID<<21)>>16))
#define  CAN_STD_ID_LIST_32BIT_L(STDID)       ((uint16_t) (0x0000))

#define  CAN_EXT_ID_LIST_32BIT_H(EXTID)       ((uint16_t) ((((uint32_t)EXTID<<3)|0x04)>>16))
#define  CAN_EXT_ID_LIST_32BIT_L(EXTID)       ((uint16_t) (( (uint32_t)EXTID<<3)|0x04))

#define  CAN_FILTER_STDID(STDID)        ((STDID&0x7FF)<<5)

#define  CAN_FILTER_EXTID_H(EXTID)      ((uint16_t)  (((EXTID<<3)|0x04)>>16) )
#define  CAN_FILTER_EXTID_L(EXTID)      ((uint16_t)  (( EXTID<<3)|0x04) )

#define  CAN_STD_ID_H_MASK_CARE         0xFFE0
#define  CAN_STD_ID_L_MASK_CARE         0x0000
#define  CAN_STD_ID_H_MASK_DONT_CARE    0x0000
#define  CAN_STD_ID_L_MASK_DONT_CARE    0x0000

#define  CAN_EXT_ID_H_MASK_CARE         0xFFFF
#define  CAN_EXT_ID_L_MASK_CARE         0xFFF8
#define  CAN_EXT_ID_H_MASK_DONT_CARE    0x0000
#define  CAN_EXT_ID_L_MASK_DONT_CARE    0x0000

void  CanDriverInit(void);
#endif


