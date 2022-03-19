#ifndef _CFGPARAM_H_
#define _CFGPARAM_H_

#include "FLASH.h"

//�����ֽڵ�д���ַ
#define RESERVE_ADDR                0x4000
#define RESERVE_DATA                0xCCCCCCCC

//��ǰ�豸��ַд��ĵ�ַ
#define MODBUS_ADDR_WRITE_ADDR      0x4004
//�豸Ĭ�ϵ�ַΪ0x01
#define MODBUS_DEFAULT_ADDR         0x01

//�����ʶ�ȡ�ĵ�ַ
#define UART_BAUD_ADDR              0x4008
//Ĭ�ϲ�����Ϊ115200
#define UART_BAUD_DEFAULT           115200

//��·��·����ȡ�ĵ�ַ
#define BOART_PORT_NUM_ADDR         0x400C
//Ĭ�ϵ�·��·��Ϊ5
#define BOART_PORT_NUM_DEFAULT      0x1

extern UINT8 ModbusAddr;
extern UINT32 UartBuad;

void SaveModbusAddr(UINT8 Addr);
void CfgInit(void);

#endif
