#include "ModbusDeal.h"
#include "../Flash/CfgParam.h"
#include "../IO/GPIO.h"

extern UINT8 BoardPortNum;
extern UINT8 ModbusAddr;
UINT8 ModbusSendBufffer[20];

//���õ�ַ������
BOOL ModbusCommonDecode_SETADDR(UINT8 Port, UINT8* Data, UINT16 Len)
{
  UINT8 Addr;
  
  Addr = *(Data+8);
  SaveModbusAddr(Addr);
  UART1IntSend(Data, (UINT8)Len);           //����ָ���
  
  return TRUE;
}

//��ȡ��ַ������
BOOL ModbusCommonDecode_GETADDR(UINT8 Port, UINT8* Data, UINT16 Len)
{
  UINT16 CRC = 0;
  
  ModbusSendBufffer[0] = 0x00;
  ModbusSendBufffer[1] = GET_ADDR_CODE;
  ModbusSendBufffer[2] = 0x02;
  ModbusSendBufffer[3] = 0x00;
  ModbusSendBufffer[4] = ModbusAddr;
  CRC = crc16(ModbusSendBufffer, 5);
  ModbusSendBufffer[5] = (CRC >> 8) & 0xFF;
  ModbusSendBufffer[6] = CRC & 0xFF;
  UART1IntSend(ModbusSendBufffer, 7);
  
  return TRUE;
}

ModbusResDealType ModbusCommonDeal[MODBUS_COMMON_NUM] = 
{
  SET_ADDR_CODE, ModbusCommonDecode_SETADDR,
  GET_ADDR_CODE, ModbusCommonDecode_GETADDR,
};


//Modbus������ַ��������0x00��ַ
BOOL ModbusCommonDecode(UINT8 Port, UINT8* Data, UINT16 Len)
{
  UINT8 i = 0;
  
  for(i=0; i<MODBUS_COMMON_NUM; i++)
  {
    if(*(Data+1) == ModbusCommonDeal[i].ModbusRecvHead)
    {
      ModbusCommonDeal[i].ModbusDecode(Port, Data, Len);
    }
  }
  
  return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//��ǰ��ַ��������
//���������̵�������
BOOL ModbusAddrDecode_CtrlSignalDelay(UINT8 Port, UINT8* Data, UINT16 Len)
{
  UINT16 CtrlPort;      //���Ƶļ̵����˿�
  UINT8 Status;
  UINT8 Option;     //��ò�����
//  UINT8 i = 0;
  
  CtrlPort = (*(Data+2) << 8) | *(Data+3);
  Option = *(Data+4);
  
  if(Option == 0xFF)
  {
    Status = DOUT_ON;     //ȫ��
  }
  else
  {
    Status = DOUT_OFF;    //ȫ��
  }
//  Status = DOUT_OFF;    //ȫ��
  
//  for(i=0; i<5; i++)
//    IODOUTCtrl(i, Status);
  IODOUTCtrl(CtrlPort, Status);
  UART1IntSend(Data, Len);
  
  return TRUE;
}

//�������м̵�������r
BOOL ModbusAddrDecode_CtrlAllDelay(UINT8 Port, UINT8* Data, UINT16 Len)
{
  UINT8 i = 0;
  UINT8 Option;     //��ò�����
  UINT8 Status;
  UINT16 CRC = 0;
  
  Option = *(Data+8);
  
  if(Option == 0xFF)
  {
    Status = DOUT_ON;     //ȫ��
  }
  else
  {
    Status = DOUT_OFF;    //ȫ��
  }
  
  for(i=0; i<BoardPortNum; i++)
  {
    IODOUTCtrl(i, Status);
  }
  
  ModbusSendBufffer[0] = ModbusAddr;
  ModbusSendBufffer[1] = CTRL_ALL_DELAY_CODE;
  ModbusSendBufffer[2] = 0x00;
  ModbusSendBufffer[3] = 0x00;
  ModbusSendBufffer[4] = 0x00;
  ModbusSendBufffer[5] = 0x08;
  CRC = crc16(ModbusSendBufffer, 6);
  ModbusSendBufffer[6] = (CRC >> 8) & 0xFF;
  ModbusSendBufffer[7] = CRC & 0xFF;
  UART1IntSend(ModbusSendBufffer, 8);
  
  return TRUE;
}

//��ȡ���м̵���״̬����
BOOL ModbusAddrDecode_ReadAllDelay(UINT8 Port, UINT8* Data, UINT16 Len)
{
  UINT8 Status;
  UINT16 CRC = 0;
  
  Status = GetALLDOUTPortStatus(BoardPortNum);
  ModbusSendBufffer[0] = ModbusAddr;
  ModbusSendBufffer[1] = 0x01;
  ModbusSendBufffer[2] = 0x01;
  ModbusSendBufffer[3] = Status;
  CRC = crc16(ModbusSendBufffer, 4);
  ModbusSendBufffer[4] = (CRC >> 8) & 0xFF;
  ModbusSendBufffer[5] = CRC & 0xFF;
  
  UART1IntSend(ModbusSendBufffer, 6);
  
  return TRUE;
}

//��ȡ���й���״̬����
BOOL ModbusAddrDecode_ReadOptoCoupler(UINT8 Port, UINT8* Data, UINT16 Len)
{
  UINT8 Status;
  UINT16 CRC = 0;
  
  Status = GetALLDINPortStatus(BoardPortNum);
  ModbusSendBufffer[0] = ModbusAddr;
  ModbusSendBufffer[1] = 0x02;
  ModbusSendBufffer[2] = 0x01;
  ModbusSendBufffer[3] = Status;
  CRC = crc16(ModbusSendBufffer, 4);
  ModbusSendBufffer[4] = (CRC >> 8) & 0xFF;
  ModbusSendBufffer[5] = CRC & 0xFF;
  
  UART1IntSend(ModbusSendBufffer, 6);
  
  return TRUE;
}


ModbusResDealType ModbusAddrDeal[MODBUS_ADDR_NUM] = 
{
  CTRL_SIGNAL_DELAY_CODE,     ModbusAddrDecode_CtrlSignalDelay,
  CTRL_ALL_DELAY_CODE,        ModbusAddrDecode_CtrlAllDelay,
  READ_ALL_DELAY_CODE,        ModbusAddrDecode_ReadAllDelay,
  READ_OPTOCOUPLER_CODE,      ModbusAddrDecode_ReadOptoCoupler,
};


//Modbus��ǰ��ַ��������Modbus��ַ
BOOL ModbusAddrDecode(UINT8 Port, UINT8* Data, UINT16 Len)
{
  UINT8 i = 0;
  
  for(i=0; i<MODBUS_ADDR_NUM; i++)
  {
    if(*(Data+1) == ModbusAddrDeal[i].ModbusRecvHead)
    {
      ModbusAddrDeal[i].ModbusDecode(Port, Data, Len);
    }
  }
  
  return TRUE;
}

//Modbus���ݴ�����
void ModbusDataDealFunc(UINT8* Data, UINT16 Len)
{
  ModbusAddrDecode(0, Data, Len);
  ModbusCommonDecode(0, Data, Len);
}







