/**
 * Author Nguyen Minh Thien 2023
 * Logic Process Error code
*/

#include "AppInterface.h"


ErrorStd ErrorCode = ENone;
uint8_t AppLedCnt = 0; uint8_t u8CommCnt = 0;


void SetErrorCode(ErrorStd eVal)
{
   ErrorCode = eVal;
}


ErrorStd GetErrorCode()
{
   return ErrorCode;
}

void ResetCommError()
{
   u8CommCnt = 0;
}


void AppCycleSetLedStatus()
{
   sModelInfo* pModel = AppGetInfo();
   if (GetErrorCode()!= ENone)
   {
      switch (GetErrorCode())
      {
      case ERROR_RS485_COMMUNICATE:
         ReInitUartConnect();
         SetErrorCode(ENone);
         break;
      
      default:
         break;
      }
   }

   if (GetTimerCycleOneSecound() == 0)
   {
      AppLedCnt = 0;
   }
   
   
   if (pModel->TYPE == MASTER)
   {
      if ((GetTimerCycleOneSecound() - AppLedCnt) >= 2)
      {
         AppLedCnt = GetTimerCycleOneSecound();
         HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
      }

   }

   else if (pModel->TYPE == SLAVE)
   {
      if ((GetTimerCycleOneSecound() - AppLedCnt) >= 1)
      {
         AppLedCnt = GetTimerCycleOneSecound();
         u8CommCnt++;
         HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
      }
      if ((u8CommCnt >=3) && (Get_0xFE_DataReceivedFromPanel() == DISABLE_READ_DATA_UART)\
      && (pModel->TYPE == SLAVE))
      {
         //ReInitUartConnect();
         u8CommCnt = 0;
      }
      
   }
   
}

