#include "CfgParam.h"

UINT8 ModbusAddr;
UINT32 UartBuad;
UINT8 BoardPortNum = 0;       //电路板路数

void SaveModbusAddr(UINT8 Addr)
{
//  __eeprom_program_byte(MODBUS_ADDR_WRITE_ADDR, Addr);
  EEPROM_WRITE_WORD(MODBUS_ADDR_WRITE_ADDR, Addr);
}

void SaveDefaultData(void)
{
//  __eeprom_program_byte(RESERVE_ADDR, RESERVE_DATA);
  EEPROM_WRITE_WORD(RESERVE_ADDR, RESERVE_DATA);
}

void SaveDefaultUartBaud(UINT32 Baud)
{
  EEPROM_WRITE_WORD(UART_BAUD_ADDR, Baud);
}

void SaveDefaultPortNum(UINT32 Port)
{
  EEPROM_WRITE_WORD(BOART_PORT_NUM_ADDR, Port);
}

//参数初始化
void CfgInit(void)
{
  if(Flash_ReadByte(RESERVE_ADDR) == RESERVE_DATA)
  {
    ModbusAddr = Flash_ReadByte(MODBUS_ADDR_WRITE_ADDR);
    UartBuad = Flash_ReadByte(UART_BAUD_ADDR);
    BoardPortNum = Flash_ReadByte(BOART_PORT_NUM_ADDR);
  }
  else
  {
    SaveDefaultData();
    SaveModbusAddr(MODBUS_DEFAULT_ADDR);
    SaveDefaultUartBaud(UART_BAUD_DEFAULT);
    SaveDefaultPortNum(BOART_PORT_NUM_DEFAULT);
    ModbusAddr = MODBUS_DEFAULT_ADDR;
    UartBuad = UART_BAUD_DEFAULT;
    BoardPortNum = BOART_PORT_NUM_DEFAULT;
  }
}