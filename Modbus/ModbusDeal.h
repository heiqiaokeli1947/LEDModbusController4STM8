#ifndef _MODBUSDEAL_H_
#define _MODBUSDEAL_H_

#include "../Sys.h"
#include "Modbus.h"

typedef BOOL  (*ModbusDealFunction)(UINT8 Port, UINT8* Data, UINT16 len);

typedef struct MODBUS_RES_DEAL_TYPE
{
	UINT8 ModbusRecvHead;				//Modbus指令
	ModbusDealFunction ModbusDecode;		//Modbus指令返回处理函数
}ModbusResDealType;

//设置地址操作码
#define SET_ADDR_CODE       0x10
//得到地址操作码
#define GET_ADDR_CODE       0x03

//单个继电器操作码
#define CTRL_SIGNAL_DELAY_CODE     0x05
//所有继电器操作码
#define CTRL_ALL_DELAY_CODE        0x0F
//读取继电器操作码
#define READ_ALL_DELAY_CODE        0x01
//读取光耦操作码
#define READ_OPTOCOUPLER_CODE      0x02

enum
{
  MODBUS_COMMON_SETADDR = 0,          //设置地址
  MODBUS_COMMON_GETADDR,              //读取地址
  MODBUS_COMMON_NUM,
};

enum
{
  MODBUS_ADDR_CTRLSIGNALDELAY = 0,       //单个继电器操作
  MODBUS_ADDR_CTRLALLDELAY,              //所有继电器操作
  MODBUS_ADDR_READALLDELAY,              //读取继电器操作
  MODBUS_ADDR_READOPTOCOUPLER,           //读取光耦操作
  
  MODBUS_ADDR_NUM,
};

void ModbusDataDealFunc(UINT8* Data, UINT16 Len);

#endif

