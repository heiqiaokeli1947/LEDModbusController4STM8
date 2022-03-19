#ifndef _MODBUS_H_
#define _MODBUS_H_

#include "crc16.h"
#include "../Interrupt/int.h"

//定义Modbus接收的最大字节数
#define MODBUS_RECV_DATA_NUM    15

//定义公共的modbus地址
#define MODBUS_DEFAULT_COMMON_ADDR    0x00 
//#define MODBUS_DEFAULT_COMMON_ADDR    0xFF 

typedef struct TypeModbusData
{
  UINT8 Data[MODBUS_RECV_DATA_NUM];       //用于存储接收到的数据
  UINT8 DataCount;                        //用于记录当前帧的字节数
  UINT8 DataLen;                          //当前帧的长度
}ModbusDataStruct;

void ModbusInit(UINT32 Baud);
void ModbusDataDeal(void);
void UART1IntSend(UINT8* Data, UINT8 Len);
void ModbusRecvDataDeal(void);
//void UART1SendByte(UINT8 Data);

#endif
