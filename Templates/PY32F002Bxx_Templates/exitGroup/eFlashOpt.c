#include"eFlashOpt.h"

#define FLASH_USER_START_ADDR     0x08005000

void FlashInit(void)
{
    /* Unlocks the FLASH Program Erase Controller */   
  HAL_FLASH_Unlock();
}


//É¾³ýÒ³²Ù×÷
void EraseSector(uint8_t n)
{
 uint32_t PAGEError = 0;
HAL_FLASH_Unlock();
 
  FLASH_EraseInitTypeDef EraseInitStruct;

  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGEERASE;                                     /* Page Erase */
  EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;                                         /* Erase Start Address */
  EraseInitStruct.NbPages  =n;                                   /* Erase Page Number */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)                               /* Page Erase */
  {
    APP_ErrorHandler();
  }
		
}


void APP_FlashBlank(void)
{
  uint32_t addr = 0;

  while (addr <FLASH_PAGE_SIZE)
  {
    if (0xFFFFFFFF != HW32_REG(FLASH_USER_START_ADDR + addr))
    {
      APP_ErrorHandler();
    }
    addr += 4;
  }
}

//¶Á°ë¸ö×Ö½Ú
uint32_t ReadWord(uint32_t n)
{

return 	HW32_REG(FLASH_USER_START_ADDR + n);
}
//¶Á°ë¸ö×Ö½Ú
void WriteWord(uint32_t Address,uint32_t *Data)
{
uint32_t  writeAdd = FLASH_USER_START_ADDR+Address;
HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE, writeAdd, Data);
		
	

}

void APP_FlashVerify(uint32_t *DATA)
{
  uint32_t addr = 0;

  while (addr < FLASH_PAGE_SIZE)
  {
     DATA[addr / 4] = HW32_REG(FLASH_USER_START_ADDR + addr);
    addr += 4;
  }
}

uint32_t  rBuf[64]={0};
uint32_t  wBuf[64] ={0};

void  FlashFunTest(void)
{
uint8_t  i;
EraseSector(2);
APP_FlashBlank();

for(i =0; i != 64;i++)
{
wBuf[i] = 10+i;
}
WriteWord(0,wBuf);

APP_FlashVerify(rBuf);

//rBuf[0] = ReadWord(1);
}

