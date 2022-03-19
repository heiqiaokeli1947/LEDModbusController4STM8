#ifndef _MODBUSDEAL_H_
#define _MODBUSDEAL_H_

#include "../Sys.h"
#include "Modbus.h"

typedef BOOL  (*ModbusDealFunction)(UINT8 Port, UINT8* Data, UINT16 len);

typedef struct MODBUS_RES_DEAL_TYPE
{
	UINT8 ModbusRecvHead;				//Modbusָ��
	ModbusDealFunction ModbusDecode;		//Modbusָ��ش�����
}ModbusResDealType;

//���õ�ַ������
#define SET_ADDR_CODE       0x10
//�õ���ַ������
#define GET_ADDR_CODE       0x03

//�����̵���������
#define CTRL_SIGNAL_DELAY_CODE     0x05
//���м̵���������
#define CTRL_ALL_DELAY_CODE        0x0F
//��ȡ�̵���������
#define READ_ALL_DELAY_CODE        0x01
//��ȡ���������
#define READ_OPTOCOUPLER_CODE      0x02

enum
{
  MODBUS_COMMON_SETADDR = 0,          //���õ�ַ
  MODBUS_COMMON_GETADDR,              //��ȡ��ַ
  MODBUS_COMMON_NUM,
};

enum
{
  MODBUS_ADDR_CTRLSIGNALDELAY = 0,       //�����̵�������
  MODBUS_ADDR_CTRLALLDELAY,              //���м̵�������
  MODBUS_ADDR_READALLDELAY,              //��ȡ�̵�������
  MODBUS_ADDR_READOPTOCOUPLER,           //��ȡ�������
  
  MODBUS_ADDR_NUM,
};

void ModbusDataDealFunc(UINT8* Data, UINT16 Len);

#endif

