#ifndef _MODBUS_H_
#define _MODBUS_H_

#include "crc16.h"
#include "../Interrupt/int.h"

//����Modbus���յ�����ֽ���
#define MODBUS_RECV_DATA_NUM    15

//���幫����modbus��ַ
#define MODBUS_DEFAULT_COMMON_ADDR    0x00 
//#define MODBUS_DEFAULT_COMMON_ADDR    0xFF 

typedef struct TypeModbusData
{
  UINT8 Data[MODBUS_RECV_DATA_NUM];       //���ڴ洢���յ�������
  UINT8 DataCount;                        //���ڼ�¼��ǰ֡���ֽ���
  UINT8 DataLen;                          //��ǰ֡�ĳ���
}ModbusDataStruct;

void ModbusInit(UINT32 Baud);
void ModbusDataDeal(void);
void UART1IntSend(UINT8* Data, UINT8 Len);
void ModbusRecvDataDeal(void);
//void UART1SendByte(UINT8 Data);

#endif
