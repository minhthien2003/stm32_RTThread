/**
 * Author Nguyen Minh Thien 2023
 * Logic Process Timer program
*/
#ifndef __TIMERCONTROL_H__
#define __TIMERCONTROL_H__
#endif


#define TIMER_START (1)
#define TIMER_STOP  (0)
#define TIMER_LOCK   (1)
#define TIMER_UNLOCK (0)
#define TIMER_POLL  (0)
#define TIMER_INTERRUPT (1)


void CheckTimeEnterReboot();
uint8_t GetTimerCycleOneSecound();
void ResetTimerCycleOneSecond();
void SetLockTimer(uint8_t u8Data);
uint8_t GetLockTimer();
void TimerBootPinAction(TIM_HandleTypeDef *htim, uint8_t u8Type);
void TimerAction(TIM_HandleTypeDef *htim, uint8_t u8Type, uint8_t u8Action);



