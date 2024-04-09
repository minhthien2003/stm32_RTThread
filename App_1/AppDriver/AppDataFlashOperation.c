/**
 * Author Nguyen Minh Thien 2023
 * Logic Process Flash operation
*/

#include "AppInterface.h"

FLASH_EraseInitTypeDef flTypeInit;
uint32_t FlError = 0;

void WriteBackupData(uint32_t address, uint32_t *u32Data)
{
  
   HAL_FLASH_Unlock();
   flTypeInit.TypeErase = FLASH_TYPEERASE_PAGES;
   flTypeInit.PageAddress = address;
   flTypeInit.NbPages = 1;
   
   if(HAL_FLASHEx_Erase(&flTypeInit, &FlError) != HAL_OK)
   {
      ;//return HAL_FLASH_GetError();
   }
   
   if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *u32Data) != HAL_OK)
   {
      ;//   return HAL_FLASH_GetError();
   }
   HAL_FLASH_Lock();
}



void ReadBackupData(uint32_t address, uint32_t *u32Data)
{
   *u32Data = *(volatile uint32_t *)(address);
}

