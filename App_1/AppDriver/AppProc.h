
/**
 * Author Nguyen Minh Thien 2023
 * Logic Process main program
*/
#ifndef __APPPROC_H__
#define __APPPROC_H__
#endif
/*Define info flash address*/


#define  MASTER         0xA0
#define  SLAVE          0xA1
#define  RS485_MODEL    0xA2
#define  NRF24_MODEL    0xA3
#define  RS485_NRF24    0xA4

#define  MASTER_ADDRESS    0x00
#define  SLAVE_ADDRESS     0x01

/*Define Flag when Uart Received completed*/
#define ALLOW_READ_DATA_UART     (1)
#define DISABLE_READ_DATA_UART   (0)



typedef enum
{
   IO_GET_PIN_INPUT_VALUE = 0,   /*Read Input data*/
   IO_GET_PIN_OUTPUT_VALUE,      /*Read Output data*/
   IO_SET_PIN_INPUT_VALUE,       /*Set Input data*/
   IO_SET_PIN_OUTPUT_VALUE,      /*Set Input data*/
   IO_CHANGED,
   IO_UNCHANGED,

}_eIOData;



#define MaxFrameUartData (14)
typedef union
{
   uint8_t InternalComData[MaxFrameUartData];
   struct
   {
    uint8_t DestAddress;
    uint8_t SourceAddress;
    uint8_t HeaderID;// packet from device from Uart 
    uint8_t sAction; // Command
    uint8_t DataIO[MaxFrameUartData - 4];
   } Bytes;
} Uart485DataCommPackets;

typedef enum
{
   BUTTON_NONE = 0,
   BOOT_MODE,
   SET_ADDRESS_MODE,
   SET_MASTER,
   SET_SLAVE,
   SYSTEM_REBOOT,

} eBootPinMode;

typedef struct
{
   uint8_t u8CheckCount;
   eBootPinMode eMode;
}strBootPinHandle;

strBootPinHandle * GetBootPinAction();

typedef struct
{
   uint8_t LAKE_ADDRESS;
   uint8_t TYPE;
   uint8_t VERSION; //year
   uint8_t MODEL;
}sModelInfo;




sModelInfo* AppGetInfo();

void AppReadDataOption();


uint8_t Get_0xFE_DataReceivedFromPanel();
void Set_0xFE_DataReceivedFromPanel(uint8_t u8Value);
_eIOData UartRs485ProcessData();
uint8_t GetIOStatusFunc(_eIOData eType, uint8_t u8Pin);
void SetIOStatusFunc(_eIOData eType,uint8_t u8Pin, uint8_t u8Value);

uint8_t ReadIOPin();

void AppCycleCheckBootPin();
void AppUartCycleUpdateIOValue();
void AppCycleGetInputDataPin();

void ResetAllGPIOFunc();

void ClearUartTxRxBuffer(Uart485DataCommPackets* pu8Packet);

void UartMakeSendData(const void* pu8Data, uint8_t u8NumData);
void Uart485SendNotiACKPacket(uint8_t address, Uart485DataCommPackets* pData, uint8_t typeData,uint8_t u8NumData);

void ReInitUartConnect();

