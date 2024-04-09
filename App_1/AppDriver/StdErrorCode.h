/**
 * Author Nguyen Minh Thien 2023
 * Logic Process Error code
*/
#ifndef __STDERRORCODE_H__
#define __STDERRORCODE_H__
#endif


typedef enum{
   ENone = 0, 
   ERROR_RS485_COMMUNICATE,//error communicate rs485
   SET_MASTER_SLAVE,

}ErrorStd;



void SetErrorCode(ErrorStd eVal);
ErrorStd GetErrorCode();
void ResetCommError();

void AppCycleSetLedStatus();

