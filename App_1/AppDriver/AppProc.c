/**
 * Author Nguyen Minh Thien
 * Logic Process main program
*/
#include "AppInterface.h"



/*HeaderID*/
#define UPDATE_STATUS_IO 0xFE
#define SET_ADDRESS  0xF6
#define ACK_RESPONE  0xF7
/*Type device*/
#define RS485_DEVICE 0xF0
#define NRF24L01     0xF1

/*Action*/
#define READ_NOTI    0xF2
#define WRITE_DATA   0xF3
#define NOTI_PACKET  0xF4
#define RESET_IO     0xF5

#define SET_ON       GPIO_PIN_SET
#define SET_OFF      GPIO_PIN_RESET
#define IO_STATUS_ON    (1)
#define IO_STATUS_OFF   (0)

/*Define number Input pin*/
#define NUM_INPUT_PIN        (10)
#define NUM_OUTPUT_PIN       (10)

/*Define ACK Notify Packet*/
#define ACK_0xFE     0x01
#define ACK_NOTI     0x02
#define ACK_READ     0x03
#define ACK_WRITE    0x04
#define ACK_RESET_IO 0x05



#define MAX_TIME_COUNTER_ERROR_RS485 (2) //20s Reinit Uart connect

/*Address data*/


sModelInfo gModelInfo = {0};


sModelInfo* AppGetInfo()
{
   return &gModelInfo;
}


extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim14;


Uart485DataCommPackets RxPacket, TxPacket, DataRawRx;

// static Uart485DataCommPackets ACKTxPacket;


typedef struct _gpio
{
   GPIO_TypeDef*  IO_PORT;
   uint16_t IO_NAME;
   uint8_t IO_DATA_VALUE;

}GPIOInit;
GPIOInit INPUT_TABLE[10] = 
         {
            {IN_1_GPIO_Port, IN_1_Pin,    SET_OFF},
            {IN_2_GPIO_Port, IN_2_Pin,    SET_OFF},
            {IN_3_GPIO_Port, IN_3_Pin,    SET_OFF},
            {IN_4_GPIO_Port, IN_4_Pin,    SET_OFF},
            {IN_5_GPIO_Port, IN_5_Pin,    SET_OFF},
            {IN_6_GPIO_Port, IN_6_Pin,    SET_OFF},
            {IN_7_GPIO_Port, IN_7_Pin,    SET_OFF},
            {IN_8_GPIO_Port, IN_8_Pin,    SET_OFF},
            {IN_9_GPIO_Port, IN_9_Pin,    SET_OFF},
            {IN_10_GPIO_Port, IN_10_Pin,  SET_OFF},

         };

GPIOInit OUTPUT_TABLE[10] = 
         {
            {OUT_1_GPIO_Port, OUT_1_Pin,     SET_OFF},
            {OUT_2_GPIO_Port, OUT_2_Pin,     SET_OFF},
            {OUT_3_GPIO_Port, OUT_3_Pin,     SET_OFF},
            {OUT_4_GPIO_Port, OUT_4_Pin,     SET_OFF},
            {OUT_5_GPIO_Port, OUT_5_Pin,     SET_OFF},
            {OUT_6_GPIO_Port, OUT_6_Pin,     SET_OFF},
            {OUT_7_GPIO_Port, OUT_7_Pin,     SET_OFF},
            {OUT_8_GPIO_Port, OUT_8_Pin,     SET_OFF},
            {OUT_9_GPIO_Port, OUT_9_Pin,     SET_OFF},
            {OUT_10_GPIO_Port, OUT_10_Pin,   SET_OFF},
         };



strBootPinHandle BootPinAction = {0};
/*Counter Check ACK respone from Uart*/
uint8_t u8WaitACKFlag = 0, u8WaitCnt = 0;




void AppReadDataOption()
{
   sModelInfo* pmodel = AppGetInfo();
   sModelInfo u32ReadDataFromFl;
   ReadBackupData(ADD_PAGE_63_SECTOR_15_START, (uint32_t *)&u32ReadDataFromFl);

   if ((u32ReadDataFromFl.TYPE != MASTER) && (u32ReadDataFromFl.TYPE != SLAVE))
   {
      /*Init info for first startup*/
      pmodel->LAKE_ADDRESS = 0x01;
      pmodel->TYPE         = SLAVE;
      pmodel->VERSION      = 23;
      pmodel->MODEL        = RS485_NRF24;
      WriteBackupData(ADD_PAGE_63_SECTOR_15_START, (uint32_t *)pmodel);
   }

   *pmodel = u32ReadDataFromFl;
   if ((pmodel->TYPE == SLAVE) || (pmodel->TYPE == MASTER))
   {
      ReInitUartConnect();
   }
   

}



/*Check Data change varible*/
volatile uint8_t u8IO_Updated = DISABLE_READ_DATA_UART;
uint8_t Get_0xFE_DataReceivedFromPanel()
{
   return u8IO_Updated;
}



void Set_0xFE_DataReceivedFromPanel(uint8_t u8Value)
{
   u8IO_Updated = u8Value;
}





strBootPinHandle * GetBootPinAction()
{
   return &BootPinAction;
}

void AppCycleCheckBootPin()
{
  
    sModelInfo* pModel = AppGetInfo();
    
   if ((HAL_GPIO_ReadPin(BOOT_GPIO_Port, BOOT_Pin) == 0) && (GetLockTimer() == TIMER_UNLOCK))
   {
      /* Check Boot Pin Timing */
      TimerBootPinAction(&htim6, TIMER_START);

   }
   else if((HAL_GPIO_ReadPin(BOOT_GPIO_Port, BOOT_Pin) == 0) && (GetLockTimer() == TIMER_LOCK))
   {
      if (BootPinAction.u8CheckCount >= 3 )
      {

         SetErrorCode(SET_MASTER_SLAVE);

         if (pModel->TYPE == SLAVE)
         {
            BootPinAction.eMode = SET_MASTER;
            
         }
         else
         {
             BootPinAction.eMode = SET_SLAVE;
         }   
      }
      if (BootPinAction.u8CheckCount <= 1)
      {
         BootPinAction.eMode = SYSTEM_REBOOT;
      }
      
   }
   else
   {
      TimerBootPinAction(&htim6, TIMER_STOP);
      
      if (BootPinAction.eMode != 0)
      {

         switch (BootPinAction.eMode)
         {
         case  SET_MASTER:
               pModel->LAKE_ADDRESS = 0x00;
               pModel->TYPE = MASTER;
               pModel->VERSION      = 23;
               pModel->MODEL        = RS485_NRF24;
               WriteBackupData(ADD_PAGE_63_SECTOR_15_START, (uint32_t *)pModel);
               
            break;
         case  SET_SLAVE:
               pModel->LAKE_ADDRESS = 0x01;
               pModel->TYPE = SLAVE;
               pModel->VERSION      = 23;
               pModel->MODEL        = RS485_NRF24;
               WriteBackupData(ADD_PAGE_63_SECTOR_15_START, (uint32_t *)pModel);
               
            break;
         case  SYSTEM_REBOOT:
               HAL_NVIC_SystemReset();
               break;
                
         default:
            break;
         }
      }
      
      

      BootPinAction.eMode = BUTTON_NONE;
      BootPinAction.u8CheckCount = 0;

      SetErrorCode(ENone);
      

   }

   
}





void UartMakeSendData(const void* pu8Data, uint8_t u8NumData)
{
   if (Get_0xFE_DataReceivedFromPanel() == DISABLE_READ_DATA_UART)
   {
      HAL_GPIO_WritePin(EN_485_GPIO_Port, EN_485_Pin, SET_ON);
      HAL_Delay(3);
      HAL_UART_Transmit(&huart1, (uint8_t *)pu8Data, (uint16_t)u8NumData, HAL_MAX_DELAY);
      HAL_GPIO_WritePin(EN_485_GPIO_Port, EN_485_Pin, SET_OFF);

   } 

}



void ClearUartTxRxBuffer(Uart485DataCommPackets* pu8Packet)
{
   for (uint8_t i = 0; i < MaxFrameUartData; i++)
   {
      pu8Packet->InternalComData[i] = 0;
   }
   
}



void Uart485SendNotiACKPacket(uint8_t address, Uart485DataCommPackets* pData, uint8_t typeData, uint8_t u8NumData)
{

   sModelInfo* pModel = AppGetInfo();
   ClearUartTxRxBuffer(pData);
   switch (typeData)
   {
   case  ACK_0xFE:
      pData->Bytes.DestAddress = address;
      pData->Bytes.SourceAddress = pModel->LAKE_ADDRESS;
      pData->Bytes.HeaderID  = ACK_RESPONE;
      pData->Bytes.sAction   = ACK_NOTI;
	  
      break;
   
   case  ACK_NOTI:
      pData->Bytes.DestAddress = address;
      pData->Bytes.SourceAddress = pModel->LAKE_ADDRESS;
      pData->Bytes.HeaderID  = ACK_RESPONE;
      pData->Bytes.sAction   = ACK_NOTI;

      break;

   case  ACK_WRITE:
      pData->Bytes.DestAddress = address;
      pData->Bytes.SourceAddress = pModel->LAKE_ADDRESS;
      pData->Bytes.HeaderID   = ACK_RESPONE;
      pData->Bytes.sAction    = ACK_WRITE;

      break;
   case  WRITE_DATA:

      pData->Bytes.DestAddress = address;
      pData->Bytes.SourceAddress = pModel->LAKE_ADDRESS;
      pData->Bytes.HeaderID   = UPDATE_STATUS_IO;
      pData->Bytes.sAction    = WRITE_DATA;
      for (uint8_t Index = 0; Index < NUM_OUTPUT_PIN; Index++)
      {
         pData->Bytes.DataIO[Index] = OUTPUT_TABLE[Index].IO_DATA_VALUE;
      }
         
         break;


   default:
      break;
   }


   UartMakeSendData((uint8_t *)pData, u8NumData);
   SetErrorCode(ENone);
   
   HAL_GPIO_TogglePin(LED_2_GPIO_Port,LED_2_Pin);
   
}




/*UART DATA PROCESS*/
_eIOData UartRs485ProcessData()
{

   //Uart485DataCommPackets*   pTxPacket = &TxPacket;
   Uart485DataCommPackets*   pRxPacket = &RxPacket;
   _eIOData OutPutChanged = IO_UNCHANGED;

   sModelInfo* pModel = AppGetInfo();
   
   if ((pModel->LAKE_ADDRESS) == (pRxPacket->Bytes.DestAddress))
   {
      /*Begin check header*/
      if (((pRxPacket->Bytes.HeaderID) == UPDATE_STATUS_IO) ||((pRxPacket->Bytes.HeaderID) == ACK_RESPONE)) 
      {
            switch (pRxPacket->Bytes.sAction)
            {
               case  ACK_NOTI:
                     ResetCommError();
                     break;
               case  ACK_WRITE:
                     u8WaitACKFlag = 0;  

                     break;

               case  WRITE_DATA:

                     for (uint8_t Idx = 0; Idx < NUM_OUTPUT_PIN; Idx++)
                     {
                        OUTPUT_TABLE[Idx].IO_DATA_VALUE = pRxPacket->Bytes.DataIO[Idx];
                     }
                     
                     OutPutChanged = IO_CHANGED;
                     /*
                     if (pModel->TYPE == SLAVE) 
                     {
                        Uart485SendNotiACKPacket(MASTER_ADDRESS,pTxPacket, ACK_WRITE, MaxFrameUartData);
                        ClearUartTxRxBuffer(pRxPacket);
                        OutPutChanged = IO_CHANGED;
                     }
                     else if (pModel->TYPE == MASTER)
                     {
                        Uart485SendNotiACKPacket(SLAVE_ADDRESS,pTxPacket, ACK_WRITE, MaxFrameUartData);
                        ClearUartTxRxBuffer(pRxPacket);
                        OutPutChanged = IO_CHANGED;
                     }
                     u8WaitACKFlag = 1;  
                     */
                       
                     

                  break;

               default:

                  break;
            }
      }
      /*End check header*/
   }
   return OutPutChanged;
      

}




/*Get status of IO from Table*/
uint8_t GetIOStatusFunc(_eIOData eType, uint8_t u8Pin)
{
   switch (eType)
      {
      case  IO_GET_PIN_INPUT_VALUE:
            return INPUT_TABLE[u8Pin].IO_DATA_VALUE;
            break;
      
      case  IO_GET_PIN_OUTPUT_VALUE:
            return OUTPUT_TABLE[u8Pin].IO_DATA_VALUE;
            break;
      default:
         break;
      }
   return 0;  
}



/*Set data of IO into table*/
void SetIOStatusFunc(_eIOData eType, uint8_t u8Pin, uint8_t u8Value)
{
   switch (eType)
      {
      case  IO_SET_PIN_INPUT_VALUE:
            INPUT_TABLE[u8Pin].IO_DATA_VALUE = u8Value;
            break;

      case  IO_SET_PIN_OUTPUT_VALUE:
            OUTPUT_TABLE[u8Pin].IO_DATA_VALUE = u8Value;
            break;

      default:
         	break;
      }
}



/*Set IO Pin data into hardware*/

void AppUartCycleUpdateIOValue()
{

   if (Get_0xFE_DataReceivedFromPanel() == ALLOW_READ_DATA_UART)
   {

      Set_0xFE_DataReceivedFromPanel(DISABLE_READ_DATA_UART);
      if (UartRs485ProcessData() == IO_CHANGED)
      {
         for (uint8_t Idx = 0; Idx < NUM_OUTPUT_PIN; Idx++)
         {
           HAL_GPIO_WritePin(OUTPUT_TABLE[Idx].IO_PORT, OUTPUT_TABLE[Idx].IO_NAME,\
		   						(GPIO_PinState)OUTPUT_TABLE[Idx].IO_DATA_VALUE);
         }
      }

   }

   
}



/*Read IO status pin*/
uint8_t ReadIOPin()
{
  
   uint8_t IOTempStatus[10] = {0};
   uint8_t u8Updated = IO_UNCHANGED;
   uint8_t u8Ret = 0;

   for (uint8_t pin_index = 0; pin_index < NUM_INPUT_PIN; pin_index++)
   {

      u8Ret = HAL_GPIO_ReadPin(INPUT_TABLE[pin_index].IO_PORT, INPUT_TABLE[pin_index].IO_NAME);
      HAL_Delay(1);
      if (u8Ret == 1)
         {
            IOTempStatus[pin_index] = SET_ON; 
         }
      else
         {
            IOTempStatus[pin_index] = SET_OFF;   
         }
       
      /*Copy data to IO table if change status*/
      if (GetIOStatusFunc(IO_GET_PIN_INPUT_VALUE, pin_index) != IOTempStatus[pin_index])
      {
         SetIOStatusFunc(IO_SET_PIN_INPUT_VALUE, pin_index, IOTempStatus[pin_index]);
         u8Updated = IO_CHANGED;
      }
        
   }
   return u8Updated;
   
}



void AppCycleGetInputDataPin()
{
   static uint8_t u8ResendCounter = 0;
   Uart485DataCommPackets* pTxPacket = &TxPacket;
   sModelInfo* pModel = AppGetInfo();


   if (ReadIOPin() == IO_CHANGED)
   {
      /* Update data to Output table */
      for (uint8_t Idx = 0; Idx < NUM_INPUT_PIN; Idx++)
      {
         OUTPUT_TABLE[Idx].IO_DATA_VALUE = INPUT_TABLE[Idx].IO_DATA_VALUE;
         HAL_GPIO_WritePin(OUTPUT_TABLE[Idx].IO_PORT , OUTPUT_TABLE[Idx].IO_NAME, \
		 					(GPIO_PinState) OUTPUT_TABLE[Idx].IO_DATA_VALUE );
      }

      if (pModel->TYPE == MASTER)
      {

         Uart485SendNotiACKPacket(SLAVE_ADDRESS, pTxPacket, WRITE_DATA, MaxFrameUartData);
         
      }
         
   }

   
   if (pModel->TYPE == MASTER)
   {
      if (GetTimerCycleOneSecound() - u8ResendCounter == 10)
      {
         u8ResendCounter = GetTimerCycleOneSecound();
         Uart485SendNotiACKPacket(SLAVE_ADDRESS,pTxPacket, WRITE_DATA, MaxFrameUartData);
      }
      else if(GetTimerCycleOneSecound() == 0)
      {
         u8ResendCounter = 0;
      }

   }
  
   /**/
   
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
   if (huart->Instance == USART1)
   {
      for (uint8_t i = 0; i < MaxFrameUartData; i++)
      {
         RxPacket.InternalComData[i] = DataRawRx.InternalComData[i];
      }
      
      Set_0xFE_DataReceivedFromPanel(ALLOW_READ_DATA_UART);
      HAL_GPIO_TogglePin(LED_2_GPIO_Port,LED_2_Pin);
      SetErrorCode(ENone);


   }
     
}


void ReInitUartConnect()
{

   ClearUartTxRxBuffer(&DataRawRx);
   HAL_GPIO_WritePin(EN_485_GPIO_Port, EN_485_Pin, SET_OFF);
   HAL_UART_Receive_DMA(&huart1, (uint8_t*)&DataRawRx, 14);
   //HAL_UART_Receive_IT(&huart1, (uint8_t *)&DataRawRx, MaxFrameUartData);

}




void ResetAllGPIOFunc()
{
   for (uint8_t index = 1; index <= NUM_OUTPUT_PIN; index++)
   {
      HAL_GPIO_WritePin(OUTPUT_TABLE[index].IO_PORT, OUTPUT_TABLE[index].IO_NAME, SET_OFF);
   }
}



