#ifndef _CFGPARAM_H_
#define _CFGPARAM_H_

#include "FLASH.h"

//保留字节的写入地址
#define RESERVE_ADDR                0x4000
#define RESERVE_DATA                0xCCCCCCCC

//当前设备地址写入的地址
#define MODBUS_ADDR_WRITE_ADDR      0x4004
//设备默认地址为0x01
#define MODBUS_DEFAULT_ADDR         0x01

//波特率读取的地址
#define UART_BAUD_ADDR              0x4008
//默认波特率为115200
#define UART_BAUD_DEFAULT           115200

//电路板路数读取的地址
#define BOART_PORT_NUM_ADDR         0x400C
//默认电路板路数为5
#define BOART_PORT_NUM_DEFAULT      0x1

extern UINT8 ModbusAddr;
extern UINT32 UartBuad;

void SaveModbusAddr(UINT8 Addr);
void CfgInit(void);

#endif
