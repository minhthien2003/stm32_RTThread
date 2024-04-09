/**
 * Author Nguyen Minh Thien 2023
 * Logic Process Timer program
*/

#include "AppInterface.h"

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim14;

uint8_t u8TimerCycleOneSecond = 0;
uint8_t u8LockTimer = TIMER_UNLOCK;
uint8_t u8gTimeCallBackFunc = 0;

/*  6048 = 7(day)*24(h)*3600(second)/100(counter)  */
#define TIME_REBOOT_CONTER  (6048)

uint16_t ResetSystemCounter = 0;

void CheckTimeEnterReboot()
{
    if (ResetSystemCounter == TIME_REBOOT_CONTER)
    {
        HAL_NVIC_SystemReset();
    }
    
}

uint8_t GetTimerCycleOneSecound()
{
   return u8TimerCycleOneSecond;
}


void ResetTimerCycleOneSecond()
{
    u8TimerCycleOneSecond = 0;
}

void SetLockTimer(uint8_t u8Data)
{
   u8LockTimer = u8Data;
}


uint8_t GetLockTimer()
{
   return u8LockTimer;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  
   if (htim->Instance == TIM6)
   {

    strBootPinHandle* pBootPin = GetBootPinAction();
      pBootPin->u8CheckCount++;
   }

   
   else if(htim->Instance == TIM14)
   {

    u8TimerCycleOneSecond++;
    if (GetTimerCycleOneSecound() >=100)
    {
        ResetSystemCounter++;
        ResetTimerCycleOneSecond();
    }
      
   }
}


void TimerBootPinAction(TIM_HandleTypeDef *htim, uint8_t u8Type)
{
    if (u8Type == TIMER_START)
    {
        HAL_TIM_Base_Start_IT(htim);
        SetLockTimer(TIMER_LOCK);
    }
    else{
        HAL_TIM_Base_Stop_IT(htim);
        SetLockTimer(TIMER_UNLOCK);
    }
    
    
}
void TimerAction(TIM_HandleTypeDef *htim, uint8_t u8Type, uint8_t u8Action)
{
    if (u8Action == TIMER_START)
    {
        if (u8Type == TIMER_POLL)
        {
            HAL_TIM_Base_Start(htim);
        }
        else
        {
            HAL_TIM_Base_Start_IT(htim);
        }
    }
    else
    {
        if (u8Type == TIMER_INTERRUPT)
        {
            HAL_TIM_Base_Stop_IT(htim);
        }
        else
        {
            HAL_TIM_Base_Stop_IT(htim);
        }
    }
    
    
    
}
